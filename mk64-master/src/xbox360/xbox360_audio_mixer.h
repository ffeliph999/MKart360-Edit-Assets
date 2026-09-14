/* B17G5A: MK/NEAD helper semantics, native byte-addressed DMEM.
 * Command words retain the B17G4 native pointer and byte/sample contracts.
 */
/* Scalar mixer adapted from sm64-port/sm64-port src/pc/mixer.c.
 * Source: https://github.com/sm64-port/sm64-port/blob/master/src/pc/mixer.c
 * MK64 uses DMEM base zero, old interleave packing and the MK wet envelope.
 * Only operations emitted by src/audio/synthesis.c are retained.
 */





#define ROUND_UP_64(v) (((v) + 63) & ~63)
#define ROUND_UP_32(v) (((v) + 31) & ~31)
#define ROUND_UP_16(v) (((v) + 15) & ~15)
#define ROUND_UP_8(v) (((v) + 7) & ~7)
#define ROUND_DOWN_16(v) ((v) & ~0xf)

#define BUF_SIZE 4096
#define BUF_U8(a) (rspa.buf.as_u8 + (a))
#define BUF_S16(a) (rspa.buf.as_s16 + (a) / sizeof(int16_t))

static struct {
    uint16_t in;
    uint16_t out;
    uint16_t nbytes;

    uint16_t vol[2];
    uint16_t rate[2];
    uint16_t vol_wet;
    uint16_t rate_wet;

    ADPCM_STATE *adpcm_loop_state;

    int16_t adpcm_table[16][2][8]; /* B19.4R6: full 4-bit predictor storage */

    uint16_t filter_count;
    int16_t filter[8];

    union {
        int16_t as_s16[BUF_SIZE / sizeof(int16_t)];
        uint8_t as_u8[BUF_SIZE];
    } buf;
} rspa;

/* B19.4R6 audio telemetry. These counters do not alter mixer behavior. */
static bool x360_mixer_diagnostics;
static unsigned x360_r6_book_bytes_max;
static unsigned x360_r6_book_loaded_bytes;
static unsigned x360_r6_book_clamps;
static unsigned x360_r6_predictor_max;
static unsigned x360_r6_predictor_oob;
static unsigned x360_r6_env_in_peak;
static unsigned x360_r6_env_left_peak;
static unsigned x360_r6_env_right_peak;
static unsigned x360_r6_env_wet_peak;
static uint16_t x360_r6_env_vol_l_min;
static uint16_t x360_r6_env_vol_l_max;
static uint16_t x360_r6_env_vol_r_min;
static uint16_t x360_r6_env_vol_r_max;

static inline unsigned x360_r6_abs16(int v) {
    return (unsigned)(v < 0 ? -v : v);
}

static void x360_r6_reset_task_telemetry(void) {
    x360_r6_env_in_peak = 0;
    x360_r6_env_left_peak = 0;
    x360_r6_env_right_peak = 0;
    x360_r6_env_wet_peak = 0;
    x360_r6_env_vol_l_min = 0xffff;
    x360_r6_env_vol_l_max = 0;
    x360_r6_env_vol_r_min = 0xffff;
    x360_r6_env_vol_r_max = 0;
}

static int16_t resample_table[64][4] = {
    {0x0c39, 0x66ad, 0x0d46, 0xffdf}, {0x0b39, 0x6696, 0x0e5f, 0xffd8},
    {0x0a44, 0x6669, 0x0f83, 0xffd0}, {0x095a, 0x6626, 0x10b4, 0xffc8},
    {0x087d, 0x65cd, 0x11f0, 0xffbf}, {0x07ab, 0x655e, 0x1338, 0xffb6},
    {0x06e4, 0x64d9, 0x148c, 0xffac}, {0x0628, 0x643f, 0x15eb, 0xffa1},
    {0x0577, 0x638f, 0x1756, 0xff96}, {0x04d1, 0x62cb, 0x18cb, 0xff8a},
    {0x0435, 0x61f3, 0x1a4c, 0xff7e}, {0x03a4, 0x6106, 0x1bd7, 0xff71},
    {0x031c, 0x6007, 0x1d6c, 0xff64}, {0x029f, 0x5ef5, 0x1f0b, 0xff56},
    {0x022a, 0x5dd0, 0x20b3, 0xff48}, {0x01be, 0x5c9a, 0x2264, 0xff3a},
    {0x015b, 0x5b53, 0x241e, 0xff2c}, {0x0101, 0x59fc, 0x25e0, 0xff1e},
    {0x00ae, 0x5896, 0x27a9, 0xff10}, {0x0063, 0x5720, 0x297a, 0xff02},
    {0x001f, 0x559d, 0x2b50, 0xfef4}, {0xffe2, 0x540d, 0x2d2c, 0xfee8},
    {0xffac, 0x5270, 0x2f0d, 0xfedb}, {0xff7c, 0x50c7, 0x30f3, 0xfed0},
    {0xff53, 0x4f14, 0x32dc, 0xfec6}, {0xff2e, 0x4d57, 0x34c8, 0xfebd},
    {0xff0f, 0x4b91, 0x36b6, 0xfeb6}, {0xfef5, 0x49c2, 0x38a5, 0xfeb0},
    {0xfedf, 0x47ed, 0x3a95, 0xfeac}, {0xfece, 0x4611, 0x3c85, 0xfeab},
    {0xfec0, 0x4430, 0x3e74, 0xfeac}, {0xfeb6, 0x424a, 0x4060, 0xfeaf},
    {0xfeaf, 0x4060, 0x424a, 0xfeb6}, {0xfeac, 0x3e74, 0x4430, 0xfec0},
    {0xfeab, 0x3c85, 0x4611, 0xfece}, {0xfeac, 0x3a95, 0x47ed, 0xfedf},
    {0xfeb0, 0x38a5, 0x49c2, 0xfef5}, {0xfeb6, 0x36b6, 0x4b91, 0xff0f},
    {0xfebd, 0x34c8, 0x4d57, 0xff2e}, {0xfec6, 0x32dc, 0x4f14, 0xff53},
    {0xfed0, 0x30f3, 0x50c7, 0xff7c}, {0xfedb, 0x2f0d, 0x5270, 0xffac},
    {0xfee8, 0x2d2c, 0x540d, 0xffe2}, {0xfef4, 0x2b50, 0x559d, 0x001f},
    {0xff02, 0x297a, 0x5720, 0x0063}, {0xff10, 0x27a9, 0x5896, 0x00ae},
    {0xff1e, 0x25e0, 0x59fc, 0x0101}, {0xff2c, 0x241e, 0x5b53, 0x015b},
    {0xff3a, 0x2264, 0x5c9a, 0x01be}, {0xff48, 0x20b3, 0x5dd0, 0x022a},
    {0xff56, 0x1f0b, 0x5ef5, 0x029f}, {0xff64, 0x1d6c, 0x6007, 0x031c},
    {0xff71, 0x1bd7, 0x6106, 0x03a4}, {0xff7e, 0x1a4c, 0x61f3, 0x0435},
    {0xff8a, 0x18cb, 0x62cb, 0x04d1}, {0xff96, 0x1756, 0x638f, 0x0577},
    {0xffa1, 0x15eb, 0x643f, 0x0628}, {0xffac, 0x148c, 0x64d9, 0x06e4},
    {0xffb6, 0x1338, 0x655e, 0x07ab}, {0xffbf, 0x11f0, 0x65cd, 0x087d},
    {0xffc8, 0x10b4, 0x6626, 0x095a}, {0xffd0, 0x0f83, 0x6669, 0x0a44},
    {0xffd8, 0x0e5f, 0x6696, 0x0b39}, {0xffdf, 0x0d46, 0x66ad, 0x0c39}
};

static inline int16_t clamp16(int32_t v) {
    if (v < -0x8000) {
        return -0x8000;
    } else if (v > 0x7fff) {
        return 0x7fff;
    }
    return (int16_t)v;
}

static inline int32_t clamp32(int64_t v) {
    if (v < -0x7fffffff - 1) {
        return -0x7fffffff - 1;
    } else if (v > 0x7fffffff) {
        return 0x7fffffff;
    }
    return (int32_t)v;
}

void aClearBufferImpl(uint16_t addr, int nbytes) {
    /* MK CLEARBUFF uses the requested byte count. */
    memset(BUF_U8(addr), 0, nbytes);
}

void aLoadBufferImpl(const void *source_addr, uint16_t dest_addr, uint16_t nbytes) {
    memcpy(BUF_U8(dest_addr), source_addr, ROUND_DOWN_16(nbytes));
}

void aSaveBufferImpl(uint16_t source_addr, int16_t *dest_addr, uint16_t nbytes) {
    memcpy(dest_addr, BUF_S16(source_addr), ROUND_DOWN_16(nbytes));
}

void aLoadADPCMImpl(int num_entries_times_16, const int16_t *book_source_addr) {
    unsigned requested = num_entries_times_16 > 0 ? (unsigned)num_entries_times_16 : 0;
    unsigned copy_bytes = requested;

    if (requested > x360_r6_book_bytes_max)
        x360_r6_book_bytes_max = requested;

    if (copy_bytes > sizeof(rspa.adpcm_table)) {
        copy_bytes = sizeof(rspa.adpcm_table);
        ++x360_r6_book_clamps;
    }

    x360_r6_book_loaded_bytes = copy_bytes;
    if (copy_bytes && book_source_addr)
        memcpy(rspa.adpcm_table, book_source_addr, copy_bytes);
}

void aSetBufferImpl(uint8_t flags, uint16_t in, uint16_t out, uint16_t nbytes) {
    rspa.in = in;
    rspa.out = out;
    rspa.nbytes = nbytes;
}


void aInterleaveImpl(uint16_t dest, uint16_t left, uint16_t right, uint16_t c) {
    int count = ROUND_UP_8(c) / sizeof(int16_t) / 4;
    int16_t *l = BUF_S16(left);
    int16_t *r = BUF_S16(right);
    int16_t *d = BUF_S16(dest);
    while (count > 0) {
        int16_t l0 = *l++;
        int16_t l1 = *l++;
        int16_t l2 = *l++;
        int16_t l3 = *l++;
        int16_t r0 = *r++;
        int16_t r1 = *r++;
        int16_t r2 = *r++;
        int16_t r3 = *r++;
        *d++ = l0;
        *d++ = r0;
        *d++ = l1;
        *d++ = r1;
        *d++ = l2;
        *d++ = r2;
        *d++ = l3;
        *d++ = r3;
        --count;
    }
}

void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes) {
    nbytes = (nbytes + 3) & ~3;
    /* RSP copies forwards: overlap is significant for waveform repetition. */
    for (int i = 0; i < nbytes; ++i) BUF_U8(out_addr)[i] = BUF_U8(in_addr)[i];
}

void aSetLoopImpl(ADPCM_STATE *adpcm_loop_state) {
    rspa.adpcm_loop_state = adpcm_loop_state;
}

void aADPCMdecImpl(uint8_t flags, ADPCM_STATE state) {
    uint8_t *in = BUF_U8(rspa.in);
    int16_t *out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_32(rspa.nbytes);
    if (flags & A_INIT) {
        memset(out, 0, 16 * sizeof(int16_t));
    } else if (flags & A_LOOP) {
        memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
    } else {
        memcpy(out, state, 16 * sizeof(int16_t));
    }
    out += 16;
    while (nbytes > 0) {
        int shift = *in >> 4;
        if (shift > 12) shift = 12; // RSP saturates the residual scale
        int table_index = *in++ & 0xf;
        if ((unsigned)table_index > x360_r6_predictor_max)
            x360_r6_predictor_max = (unsigned)table_index;

        /* One predictor occupies 2 * 8 * sizeof(int16_t) == 32 bytes. */
        if ((unsigned)table_index * 32U >= x360_r6_book_loaded_bytes)
            ++x360_r6_predictor_oob;

        int16_t (*tbl)[8] = rspa.adpcm_table[table_index];
        int i;
        for (i = 0; i < 2; i++) {
            int16_t ins[8];
            int16_t prev1 = out[-1];
            int16_t prev2 = out[-2];
            int j, k;
            for (j = 0; j < 4; j++) {
                ins[j * 2] = ((int)(*in >> 4) - ((*in & 0x80) ? 16 : 0)) * (1 << shift);
                ins[j * 2 + 1] = ((int)(*in & 15) - ((*in & 8) ? 16 : 0)) * (1 << shift); in++;
            }
            for (j = 0; j < 8; j++) {
                int32_t acc = tbl[0][j] * prev2 + tbl[1][j] * prev1 + (ins[j] * 2048);
                for (k = 0; k < j; k++) {
                    acc += tbl[1][((j - k) - 1)] * ins[k];
                }
                acc >>= 11;
                *out++ = clamp16(acc);
            }
        }
        nbytes -= 16 * sizeof(int16_t);
    }
    memcpy(state, out - 16, 16 * sizeof(int16_t));
}

void aResampleImpl(uint8_t flags, uint16_t pitch, RESAMPLE_STATE state) {
    int16_t tmp[16];
    int16_t *in_initial = BUF_S16(rspa.in);
    int16_t *in = in_initial;
    int16_t *out = BUF_S16(rspa.out);
    int nbytes = ROUND_UP_16(rspa.nbytes);
    uint32_t pitch_accumulator;
    int i;
    int16_t *tbl;
    int32_t sample;
    if (flags & A_INIT) {
        memset(tmp, 0, sizeof(tmp));
    } else {
        memcpy(tmp, state, 16 * sizeof(int16_t));
    }
    if (flags & 2) {
        memcpy(in - 8, tmp + 8, 8 * sizeof(int16_t));
        in -= tmp[5] / sizeof(int16_t);
    }
    in -= 4;
    pitch_accumulator = (uint16_t)tmp[4];
    memcpy(in, tmp, 4 * sizeof(int16_t));

    do {
        for (i = 0; i < 8; i++) {
            tbl = resample_table[pitch_accumulator * 64 >> 16];
            /* Accumulate all four taps before the Q15 shift (NEAD). */
            sample = (in[0] * tbl[0] + in[1] * tbl[1] +
                      in[2] * tbl[2] + in[3] * tbl[3]) >> 15;
            *out++ = clamp16(sample);

            pitch_accumulator += (pitch << 1);
            in += pitch_accumulator >> 16;
            pitch_accumulator %= 0x10000;
        }
        nbytes -= 8 * sizeof(int16_t);
    } while (nbytes > 0);

    state[4] = (int16_t)pitch_accumulator;
    memcpy(state, in, 4 * sizeof(int16_t));
    i = (in - in_initial + 4) & 7;
    in -= i;
    if (i != 0) {
        i = -8 - i;
    }
    state[5] = i;
    memcpy(state + 8, in, 8 * sizeof(int16_t));
}

void aEnvSetup1Impl(uint8_t initial_vol_wet, uint16_t rate_wet, uint16_t rate_left, uint16_t rate_right) {
    rspa.vol_wet = (uint16_t)(initial_vol_wet << 8);
    rspa.rate_wet = rate_wet;
    rspa.rate[0] = rate_left;
    rspa.rate[1] = rate_right;
}

void aEnvSetup2Impl(uint16_t initial_vol_left, uint16_t initial_vol_right) {
    rspa.vol[0] = initial_vol_left;
    rspa.vol[1] = initial_vol_right;
}

void aEnvMixerImpl(uint16_t in_addr, uint16_t n_samples, bool swap_reverb,
                   bool neg_left, bool neg_right,
                   uint16_t dry_left_addr, uint16_t dry_right_addr,
                   uint16_t wet_left_addr, uint16_t wet_right_addr)
{
    int16_t *in = BUF_S16(in_addr);
    int16_t *dry[2] = {BUF_S16(dry_left_addr), BUF_S16(dry_right_addr)};
    int16_t *wet[2] = {BUF_S16(wet_left_addr), BUF_S16(wet_right_addr)};
    int16_t negs[2] = {neg_left ? -1 : 0, neg_right ? -1 : 0};
    int swapped[2] = {swap_reverb ? 1 : 0, swap_reverb ? 0 : 1};
    int n = ROUND_UP_8(n_samples);
    if (!n) return;

    uint16_t vols[2] = {rspa.vol[0], rspa.vol[1]};
    uint16_t rates[2] = {rspa.rate[0], rspa.rate[1]};
    uint16_t vol_wet = rspa.vol_wet;
    uint16_t rate_wet = rspa.rate_wet;

    do {
        if(x360_mixer_diagnostics){
        if (vols[0] < x360_r6_env_vol_l_min) x360_r6_env_vol_l_min = vols[0];
        if (vols[0] > x360_r6_env_vol_l_max) x360_r6_env_vol_l_max = vols[0];
        if (vols[1] < x360_r6_env_vol_r_min) x360_r6_env_vol_r_min = vols[1];
        if (vols[1] > x360_r6_env_vol_r_max) x360_r6_env_vol_r_max = vols[1];

        }

        for (int i = 0; i < 8; i++) {
            const int16_t source = *in++;
            if(x360_mixer_diagnostics){
            unsigned source_mag = x360_r6_abs16((int)source);
            if (source_mag > x360_r6_env_in_peak)
                x360_r6_env_in_peak = source_mag;

            }

            int16_t samples[2] = {source, source};
            for (int j = 0; j < 2; j++) {
                samples[j] = (samples[j] * vols[j] >> 16) ^ negs[j];

                if(x360_mixer_diagnostics){
                unsigned dry_mag = x360_r6_abs16((int)samples[j]);
                if (j == 0) {
                    if (dry_mag > x360_r6_env_left_peak) x360_r6_env_left_peak = dry_mag;
                } else {
                    if (dry_mag > x360_r6_env_right_peak) x360_r6_env_right_peak = dry_mag;
                }

                }

                *dry[j] = clamp16(*dry[j] + samples[j]); dry[j]++;

                int wet_sample = (samples[swapped[j]] * vol_wet) >> 16;
                if(x360_mixer_diagnostics){
                unsigned wet_mag = x360_r6_abs16(wet_sample);
                if (wet_mag > x360_r6_env_wet_peak) x360_r6_env_wet_peak = wet_mag;
                }

                *wet[j] = clamp16(*wet[j] + wet_sample); wet[j]++;
            }
        }
        vols[0] += rates[0];
        vols[1] += rates[1];
        vol_wet += rate_wet;

        n -= 8;
    } while (n > 0);
    rspa.vol[0] = vols[0]; rspa.vol[1] = vols[1];
    rspa.vol_wet = vol_wet;
}

void aMixImpl(int16_t gain, uint16_t in_addr, uint16_t out_addr, uint16_t count) {
    /* count is already decoded to bytes by the command dispatcher. */
    int16_t *in = BUF_S16(in_addr);
    int16_t *out = BUF_S16(out_addr);
    for (unsigned i = 0; i < (unsigned)count / 2; ++i) {
        int32_t contribution = ((int32_t)in[i] * gain) >> 15;
        out[i] = clamp16((int32_t)out[i] + contribution);
    }
}

void aDMEMMove2Impl(uint8_t t, uint16_t in_addr, uint16_t out_addr, uint16_t count) {
    int blocks = t ? t : 1; // NEAD's block loop executes once for zero.
    int bytes = ROUND_UP_32(count);
    if (!bytes) bytes = 32;
    while (blocks-- > 0) {
        /* Each 32-byte vector is captured before being written. */
        for (int offset = 0; offset < bytes; offset += 32) {
            uint8_t vector[32];
            memcpy(vector, BUF_U8(in_addr), 32);
            memcpy(BUF_U8(out_addr), vector, 32);
            in_addr += 32; out_addr += 32;
        }
    }
}

void aDownsampleHalfImpl(uint16_t n_samples, uint16_t in_addr, uint16_t out_addr) {
    int16_t *in = BUF_S16(in_addr);
    int16_t *out = BUF_S16(out_addr);
    for (unsigned i = 0; i < n_samples; ++i) out[i] = in[i * 2];
}
