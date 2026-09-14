#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ultra64.h>
#include "xbox360/platform.h"
#include "xbox360/netplay.h"
#include "xbox360_audio_mixer.h"

extern "C" unsigned x360_audio_command_count = 0;
extern "C" unsigned x360_audio_opcode_mask = 0;
extern "C" unsigned x360_audio_generated_frames = 0;
extern "C" unsigned x360_audio_unknown_commands = 0;

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
        if (x360_hle_trace) {
            unsigned peak = x360_hle_peak(rspa.out + 32, ROUND_UP_32(rspa.nbytes));
            if (peak > x360_hle_adpcm_peak) x360_hle_adpcm_peak = peak;
        }
        break;
    case A_RESAMPLE:
        if(rspa.nbytes) aResampleImpl(flags,lo,(int16_t*)b);
        if (x360_hle_trace) {
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

extern "C" void x360_dispatch_audio_task(struct SPTask *task) {
    static bool marked;
    if (!marked) {
        marked = true;
        x360_log("MK64: B17G5A NEAD helpers; byte counts preserved; env blocks=8; stereo LR unchanged\n");
        x360_log("MK64: B19.4R6 ADPCM guard + HLE telemetry; R5 mono diagnostic removed\n");
    }
    if(!task) return;
    OSTask *t=(OSTask*)task; /* SPTask begins with OSTask. */
    const Acmd *cmd=(const Acmd*)t->t.data_ptr;
    unsigned count=t->t.data_size/sizeof(Acmd);
    if(!cmd || count>16384) return;
    ++x360_hle_tasks;
    x360_hle_trace = x360_logging_enabled() && x360_hle_reports < 48 && (x360_hle_tasks % 128) == 0;
    if (x360_hle_trace) ++x360_hle_reports;
    x360_hle_adpcm_peak = x360_hle_resample_peak = x360_hle_env_count = 0;
    x360_hle_stereo_bytes = 0;
    x360_r6_reset_task_telemetry();
    x360_mixer_diagnostics = x360_hle_trace;
    x360_audio_command_count+=count;
    for(unsigned i=0;i<count;i++) {
        unsigned op=(uint32_t)cmd[i].words.w0>>24;
        if(op<32) x360_audio_opcode_mask|=1u<<op;
        x360_audio_command((uint32_t)cmd[i].words.w0,(uintptr_t)cmd[i].words.w1);
    }


}
