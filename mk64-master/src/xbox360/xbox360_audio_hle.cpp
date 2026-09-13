#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ultra64.h>
#include "xbox360/platform.h"
#include "xbox360_audio_mixer.h"

extern "C" unsigned x360_audio_command_count = 0;
extern "C" unsigned x360_audio_opcode_mask = 0;
extern "C" unsigned x360_audio_generated_frames = 0;
extern "C" unsigned x360_audio_unknown_commands = 0;

/* B17G5A: inspect PCM before XAudio/silent-channel rescue can alter it. */
/* B19.4R6 ADPCM guard + HLE telemetry */
/* B19.4R7 XENIA AUDIO TRIGGER TRACE
 * Diagnostic only. One suspicious task is described, then __debugbreak()
 * executes once so Xenia can stop with guest context preserved. */
static bool x360_r7_triggered;
static bool x360_r7_candidate;
static unsigned x360_r7_candidate_l;
static unsigned x360_r7_candidate_r;

static unsigned x360_hle_tasks, x360_hle_reports;
static bool x360_hle_trace;
static unsigned x360_hle_adpcm_peak, x360_hle_resample_peak, x360_hle_env_count;
static uint16_t x360_hle_stereo_out, x360_hle_stereo_bytes;
static unsigned x360_hle_peak(unsigned addr, unsigned bytes) {
    if (addr >= BUF_SIZE || bytes > BUF_SIZE - addr) return 0;
    unsigned peak = 0;
    for (unsigned i = 0; i < bytes / 2; ++i) {
        int sample = BUF_S16(addr)[i];
        unsigned mag = sample < 0 ? -sample : sample;
        if (mag > peak) peak = mag;
    }
    return peak;
}

/* All RAM addresses are native pointers: VIRTUAL_TO_PHYSICAL2 is identity
 * in this port. Saves fill the game's AI buffer; osAiSetNextBuffer submits it.
 * ABI encodings come from include/PR/abi.h, not the original SM64 ABI.
 */
static void x360_audio_command(uint32_t a, uintptr_t b) {
    unsigned op=a>>24, flags=(a>>16)&255;
    unsigned lo=a&65535, hi=(b>>16)&65535, n=b&65535;
    switch(op) {
    case A_SPNOOP: case A_SEGMENT: break; /* MK64 emits segment zero only. */
    case A_CLEARBUFF: if(n) aClearBufferImpl(lo,n); break;
    case A_SETBUFF: aSetBufferImpl(flags,lo,hi,n); break;
    case A_LOADBUFF: if(flags) aLoadBufferImpl((void*)b,lo,flags<<4); break;
    case A_SAVEBUFF:
        if(flags) {
            aSaveBufferImpl(lo,(int16_t*)b,flags<<4);
            if (lo == x360_hle_stereo_out && (flags << 4) == x360_hle_stereo_bytes)
                x360_audio_generated_frames += flags * 4;
        }
        break;
    case A_LOADADPCM: aLoadADPCMImpl(lo,(int16_t*)b); break;
    case A_SETLOOP: aSetLoopImpl((ADPCM_STATE*)b); break;
    case A_ADPCM:
        aADPCMdecImpl(flags,(int16_t*)b);
        if (x360_hle_trace || !x360_r7_triggered) {
            unsigned peak = x360_hle_peak(rspa.out + 32, ROUND_UP_32(rspa.nbytes));
            if (peak > x360_hle_adpcm_peak) x360_hle_adpcm_peak = peak;
        }
        break;
    case A_RESAMPLE:
        if(rspa.nbytes) aResampleImpl(flags,lo,(int16_t*)b);
        if (x360_hle_trace || !x360_r7_triggered) {
            unsigned peak = x360_hle_peak(rspa.out, ROUND_UP_16(rspa.nbytes));
            if (peak > x360_hle_resample_peak) x360_hle_resample_peak = peak;
        }
        break;
    case A_DMEMMOVE: if(n) aDMEMMoveImpl(lo,hi,n); break;
    case A_DMEMMOVE2: if(n) aDMEMMove2Impl(flags,lo,hi,n); break;
    case A_DOWNSAMPLE_HALF: if(lo) aDownsampleHalfImpl(lo,hi,n); break;
    case A_MIXER: if(flags) aMixImpl((int16_t)lo,hi,n,flags<<4); break;
    case A_INTERLEAVE:
        if(rspa.nbytes) aInterleaveImpl(rspa.out,hi,n,ROUND_UP_16(rspa.nbytes));
        x360_hle_stereo_out = rspa.out;
        x360_hle_stereo_bytes = ROUND_UP_16(rspa.nbytes) * 2;

        if (!x360_r7_triggered && x360_hle_tasks >= 1500 &&
            x360_hle_resample_peak >= 20000 && x360_hle_env_count) {
            const unsigned r7_l = x360_hle_peak(hi, rspa.nbytes);
            const unsigned r7_r = x360_hle_peak(n, rspa.nbytes);
            if (r7_l * 8U < x360_hle_resample_peak ||
                r7_r * 8U < x360_hle_resample_peak) {
                x360_r7_candidate = true;
                x360_r7_candidate_l = r7_l;
                x360_r7_candidate_r = r7_r;
            }
        }

        if (x360_hle_trace) {
            char msg[512];
            sprintf(msg,"MK64: B19.4R6 HLE task=%u adpcm=%u resample=%u env=%u envIn=%u envLR=%u/%u wet=%u vol=%04X-%04X/%04X-%04X L=%u R=%u monoBytes=%u out=%03X bytes=%u book=%u pred=%u predOOB=%u clamp=%u\n",
                x360_hle_tasks,x360_hle_adpcm_peak,x360_hle_resample_peak,x360_hle_env_count,
                x360_r6_env_in_peak,x360_r6_env_left_peak,x360_r6_env_right_peak,x360_r6_env_wet_peak,
                x360_r6_env_vol_l_min,x360_r6_env_vol_l_max,x360_r6_env_vol_r_min,x360_r6_env_vol_r_max,
                x360_hle_peak(hi,rspa.nbytes),x360_hle_peak(n,rspa.nbytes),rspa.nbytes,
                x360_hle_stereo_out,x360_hle_stereo_bytes,
                x360_r6_book_bytes_max,x360_r6_predictor_max,x360_r6_predictor_oob,x360_r6_book_clamps);
            x360_log(msg);
            x360_hle_trace = false;
        }
        break;
    case A_ENVSETUP1:
        /* MK ABI ignores the low word here; wet gain has no ramp.
         * Confirmed by mupen64plus-rsp-hle alist_nead.c ENVSETUP1_MK. */
        aEnvSetup1Impl(flags,0,hi,n); break;
    case A_ENVSETUP2: aEnvSetup2Impl(hi,n); break;
    case A_ENVMIXER:
        ++x360_hle_env_count;
        if((a>>8)&255) aEnvMixerImpl((a>>12)&0xff0,(a>>8)&255,0,
            (a&2)!=0,(a&1)!=0,(b>>20)&0xff0,(b>>12)&0xff0,(b>>4)&0xff0,(b<<4)&0xff0);
        break;
    default:
        if(x360_audio_unknown_commands++ < 4) {
            char msg[96]; sprintf(msg,"MK64: unknown audio opcode %02X words=%08X/%08X\n",op,a,(unsigned)b); x360_log(msg);
        }
        break;
    }
}

static void x360_r7_dump_suspicious_task(const Acmd *cmd, unsigned count) {
    char msg[512];

    sprintf(msg,
        "MK64: B19.4R7 TRIGGER task=%u cmds=%u adpcm=%u resample=%u env=%u "
        "envIn=%u envLR=%u/%u wet=%u finalLR=%u/%u "
        "vol=%04X-%04X/%04X-%04X book=%u pred=%u predOOB=%u clamp=%u\n",
        x360_hle_tasks, count,
        x360_hle_adpcm_peak, x360_hle_resample_peak, x360_hle_env_count,
        x360_r6_env_in_peak, x360_r6_env_left_peak, x360_r6_env_right_peak,
        x360_r6_env_wet_peak,
        x360_r7_candidate_l, x360_r7_candidate_r,
        x360_r6_env_vol_l_min, x360_r6_env_vol_l_max,
        x360_r6_env_vol_r_min, x360_r6_env_vol_r_max,
        x360_r6_book_bytes_max, x360_r6_predictor_max,
        x360_r6_predictor_oob, x360_r6_book_clamps);
    x360_log(msg);

    unsigned emitted = 0;
    unsigned dma_mismatches = 0;

    for (unsigned i = 0; i < count && emitted < 320; ++i) {
        const uint32_t w0 = (uint32_t)cmd[i].words.w0;
        const uintptr_t w1p = (uintptr_t)cmd[i].words.w1;
        const uint32_t w1 = (uint32_t)w1p;
        const unsigned op = w0 >> 24;

        switch (op) {
        case A_SETBUFF:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] SETBUFF w=%08X/%08X in=%04X out=%04X count=%u\n",
                i, w0, w1, w0 & 0xFFFFU, w1 >> 16, w1 & 0xFFFFU);
            x360_log(msg); ++emitted;
            break;

        case A_LOADBUFF:
        case A_SAVEBUFF: {
            const unsigned ref_count = (w0 >> 12) & 0xFFFU;
            const unsigned ref_dmem = w0 & 0xFFFU;
            const unsigned cur_count = ((w0 >> 16) & 0xFFU) << 4;
            const unsigned cur_dmem = w0 & 0xFFFFU;
            const unsigned mismatch =
                (ref_count != cur_count) || (ref_dmem != cur_dmem);
            if (mismatch) ++dma_mismatches;

            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] %s w=%08X/%08X "
                "refDmem=%03X refCount=%u curDmem=%04X curCount=%u "
                "dramAlign=%u dmemAlign=%u mismatch=%u\n",
                i, op == A_LOADBUFF ? "LOAD" : "SAVE",
                w0, w1, ref_dmem, ref_count, cur_dmem, cur_count,
                (unsigned)(w1p & 7U), ref_dmem & 3U, mismatch);
            x360_log(msg); ++emitted;
            break;
        }

        case A_ADPCM:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] ADPCM w=%08X/%08X flags=%02X state=%08X\n",
                i, w0, w1, (w0 >> 16) & 0xFFU, w1);
            x360_log(msg); ++emitted;
            break;

        case A_RESAMPLE:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] RESAMPLE w=%08X/%08X flags=%02X pitch=%04X state=%08X\n",
                i, w0, w1, (w0 >> 16) & 0xFFU, w0 & 0xFFFFU, w1);
            x360_log(msg); ++emitted;
            break;

        case A_MIXER:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] MIXER w=%08X/%08X gain=%04X count=%u in=%04X out=%04X\n",
                i, w0, w1, w0 & 0xFFFFU,
                ((w0 >> 16) & 0xFFU) << 4, w1 >> 16, w1 & 0xFFFFU);
            x360_log(msg); ++emitted;
            break;

        case A_INTERLEAVE:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] INTERLEAVE w=%08X/%08X left=%04X right=%04X\n",
                i, w0, w1, w1 >> 16, w1 & 0xFFFFU);
            x360_log(msg); ++emitted;
            break;

        case A_ENVSETUP1:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] ENV1 w=%08X/%08X wet=%04X stepL=%04X stepR=%04X\n",
                i, w0, w1, (w0 >> 8) & 0xFF00U, w1 >> 16, w1 & 0xFFFFU);
            x360_log(msg); ++emitted;
            break;

        case A_ENVSETUP2:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] ENV2 w=%08X/%08X volL=%04X volR=%04X\n",
                i, w0, w1, w1 >> 16, w1 & 0xFFFFU);
            x360_log(msg); ++emitted;
            break;

        case A_ENVMIXER:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] ENVMIX w=%08X/%08X in=%03X n=%u "
                "neg=%u/%u dry=%03X/%03X wet=%03X/%03X\n",
                i, w0, w1,
                (w0 >> 12) & 0xFF0U, (w0 >> 8) & 0xFFU,
                (w0 & 2U) != 0, (w0 & 1U) != 0,
                (w1 >> 20) & 0xFF0U, (w1 >> 12) & 0xFF0U,
                (w1 >> 4) & 0xFF0U, (w1 << 4) & 0xFF0U);
            x360_log(msg); ++emitted;
            break;

        case A_LOADADPCM:
        case A_SETLOOP:
        case A_CLEARBUFF:
        case A_DMEMMOVE:
        case A_DOWNSAMPLE_HALF:
            sprintf(msg,
                "MK64: B19.4R7 CMD[%u] op=%02X w=%08X/%08X\n",
                i, op, w0, w1);
            x360_log(msg); ++emitted;
            break;

        default:
            break;
        }
    }

    sprintf(msg,
        "MK64: B19.4R7 SUMMARY emitted=%u dmaDecodeMismatch=%u "
        "NOTE: next instruction is one-shot Xenia debugbreak\n",
        emitted, dma_mismatches);
    x360_log(msg);
}

extern "C" void x360_dispatch_audio_task(struct SPTask *task) {
    static bool marked;
    if (!marked) {
        marked = true;
        x360_log("MK64: B17G5A NEAD helpers; byte counts preserved; env blocks=8; stereo LR unchanged\n");
        x360_log("MK64: B19.4R6 ADPCM guard + HLE telemetry; R5 mono diagnostic removed\n");
        x360_log("MK64: B19.4R7 Xenia one-shot suspicious-audio command trace + debugbreak enabled\n");
    }
    if(!task) return;
    OSTask *t=(OSTask*)task; /* SPTask begins with OSTask. */
    const Acmd *cmd=(const Acmd*)t->t.data_ptr;
    unsigned count=t->t.data_size/sizeof(Acmd);
    if(!cmd || count>16384) return;
    ++x360_hle_tasks;
    x360_hle_trace = x360_hle_reports < 48 && (x360_hle_tasks % 128) == 0;
    if (x360_hle_trace) ++x360_hle_reports;
    x360_hle_adpcm_peak = x360_hle_resample_peak = x360_hle_env_count = 0;
    x360_hle_stereo_bytes = 0;
    x360_r6_reset_task_telemetry();
    x360_r7_candidate = false;
    x360_r7_candidate_l = x360_r7_candidate_r = 0;
    x360_audio_command_count+=count;
    for(unsigned i=0;i<count;i++) {
        unsigned op=(uint32_t)cmd[i].words.w0>>24;
        if(op<32) x360_audio_opcode_mask|=1u<<op;
        x360_audio_command((uint32_t)cmd[i].words.w0,(uintptr_t)cmd[i].words.w1);
    }

    if (x360_r7_candidate && !x360_r7_triggered) {
        x360_r7_triggered = true;
        x360_r7_dump_suspicious_task(cmd, count);
        x360_log("MK64: B19.4R7 DEBUGBREAK NOW - inspect Xenia guest context / then continue once\n");
        x360_log("MK64: B19.4R8V3 R7 trigger reached; continuing without trap\n");
    }
}
