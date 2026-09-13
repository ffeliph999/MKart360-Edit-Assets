#include <xtl.h>
#include <xaudio2.h>
#include <string.h>
#include <stdio.h>
#include "xbox360/platform.h"

enum {
    AUDIO_RATE = 26800,
    AUDIO_CHANNELS = 2,
    AUDIO_BITS = 16,
    AUDIO_RING_COUNT = 24,
    AUDIO_RING_BYTES = 0x2000,
    AUDIO_DESIRED = 4096
};

static IXAudio2 *g_xa;
static IXAudio2MasteringVoice *g_master;
static IXAudio2SourceVoice *g_voice;
static BYTE g_ring[AUDIO_RING_COUNT][AUDIO_RING_BYTES];
static unsigned g_ring_pos;
static unsigned g_submit_count, g_starved, g_peak, g_nonzero, g_requested;
static unsigned g_peak_l, g_peak_r, g_nonzero_l, g_nonzero_r;
static unsigned g_silent_channel_rescues;
static DWORD g_last_report;
static int g_playing;
extern "C" unsigned x360_audio_command_count, x360_audio_opcode_mask;
extern "C" unsigned x360_audio_generated_frames, x360_audio_unknown_commands;
static void audio_report(HRESULT hr) {
    DWORD now=GetTickCount();
    if(now-g_last_report<1000) return;
    g_last_report=now;
    XAUDIO2_VOICE_STATE s; ZeroMemory(&s,sizeof(s));
    if(g_voice) g_voice->GetState(&s);
    char msg[512];
    sprintf(msg,"MK64: audio cmds=%u ops=%08X unknown=%u requested=%u generated=%u peak=%u nonzero=%u L=%u/%u R=%u/%u rescue=%u write=%u completed=%u submitted=%u queued=%u played=%I64u started=%d starved=%u hr=%08X\n",
        x360_audio_command_count,x360_audio_opcode_mask,x360_audio_unknown_commands,
        g_requested,x360_audio_generated_frames,g_peak,g_nonzero,
        g_peak_l,g_nonzero_l,g_peak_r,g_nonzero_r,g_silent_channel_rescues,g_ring_pos,
        (g_submit_count-s.BuffersQueued)%AUDIO_RING_COUNT,g_submit_count,s.BuffersQueued,
        s.SamplesPlayed,g_playing,g_starved,(unsigned)hr);
    x360_log(msg); g_peak=g_nonzero=g_requested=0; g_peak_l=g_peak_r=g_nonzero_l=g_nonzero_r=0;
    x360_audio_command_count=x360_audio_generated_frames=0;
}
static UINT64 g_submitted_samples;

extern "C" int x360_audio_init(void) {
    g_ring_pos = 0;
    g_submitted_samples = 0;

    HRESULT hr = XAudio2Create(&g_xa, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) { audio_report(hr); return 0; }

    hr = g_xa->CreateMasteringVoice(&g_master);
    if (FAILED(hr)) { audio_report(hr); return 0; }

    WAVEFORMATEX wf;
    ZeroMemory(&wf, sizeof(wf));
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = AUDIO_CHANNELS;
    wf.nSamplesPerSec = AUDIO_RATE;
    wf.wBitsPerSample = AUDIO_BITS;
    wf.nBlockAlign = AUDIO_CHANNELS * (AUDIO_BITS / 8);
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

    hr = g_xa->CreateSourceVoice(&g_voice, &wf, 0, 2.0f);
    if (FAILED(hr)) { audio_report(hr); return 0; }
    g_playing = 0;
    x360_log("MK64: B16a XAudio output rate=26800 Hz (matches MK64 session presets)\n");
    x360_log("MK64: B17G2 audio queue=2200 + L/R diagnostics; HLE unchanged\n");
    x360_log("MK64: B17G3 audio queue=4096 + silent-channel rescue; HLE unchanged\n");
    x360_log("MK64: B17G4 prebuffer/rebuffer=6 buffers; HLE unchanged\n");
    audio_report(S_OK);
    return 1;
}

extern "C" int x360_audio_buffered_samples(void) {
    if (!g_voice) return 0;
    XAUDIO2_VOICE_STATE s;
    g_voice->GetState(&s);
    LONGLONG q = (LONGLONG)g_submitted_samples - (LONGLONG)s.SamplesPlayed;
    return q > 0 ? (int)q : 0;
}

extern "C" int x360_audio_desired_samples(void) {
    return AUDIO_DESIRED;
}

extern "C" int x360_audio_submit_checked(const void *pcm, unsigned bytes) {
    if (!g_voice || !pcm || !bytes || bytes > AUDIO_RING_BYTES || bytes % 4) { audio_report(E_FAIL); return 0; }
    g_requested+=bytes/4;
    const short *samples=(const short*)pcm;
    unsigned buffer_nz_l=0, buffer_nz_r=0;
    for(unsigned i=0;i<bytes/2;i++) {
        int v=samples[i]; unsigned mag=v<0 ? -v : v;
        if(mag>g_peak) g_peak=mag;
        if(v) ++g_nonzero;
        if(i & 1) {
            if(mag>g_peak_r) g_peak_r=mag;
            if(v) { ++g_nonzero_r; ++buffer_nz_r; }
        } else {
            if(mag>g_peak_l) g_peak_l=mag;
            if(v) { ++g_nonzero_l; ++buffer_nz_l; }
        }
    }
    XAUDIO2_VOICE_STATE state;
    g_voice->GetState(&state);
    if (state.BuffersQueued >= AUDIO_RING_COUNT) { audio_report(E_FAIL); return 0; }
    if(!state.BuffersQueued && g_submit_count) {
        ++g_starved;
        if (g_playing) {
            g_voice->Stop(0);
            g_playing = 0;
        }
    }

    BYTE *dst = g_ring[g_ring_pos];
    memcpy(dst, pcm, bytes);

    /* Preserve normal stereo. Only mirror when one complete PCM buffer is
     * totally silent on one side while the other side has real samples. */
    if ((buffer_nz_l == 0) != (buffer_nz_r == 0)) {
        short *out = (short*)dst;
        const unsigned frames = bytes / 4;
        if (buffer_nz_l == 0) {
            for (unsigned i=0;i<frames;i++) out[i*2] = out[i*2+1];
        } else {
            for (unsigned i=0;i<frames;i++) out[i*2+1] = out[i*2];
        }
        ++g_silent_channel_rescues;
    }

    XAUDIO2_BUFFER b;
    ZeroMemory(&b, sizeof(b));
    b.AudioBytes = bytes;
    b.pAudioData = dst;

    HRESULT hr=g_voice->SubmitSourceBuffer(&b);
    if (SUCCEEDED(hr)) {
        ++g_submit_count;
        g_submitted_samples += bytes / 4;
        g_ring_pos = (g_ring_pos + 1) % AUDIO_RING_COUNT;

        if (!g_playing) {
            XAUDIO2_VOICE_STATE ready;
            ZeroMemory(&ready,sizeof(ready));
            g_voice->GetState(&ready);
            if (ready.BuffersQueued >= 6) {
                HRESULT shr = g_voice->Start(0);
                if (SUCCEEDED(shr)) g_playing = 1;
            }
        }

        audio_report(hr);
        return 1;
    }
    audio_report(hr);
    return 0;
}
extern "C" void x360_audio_submit_pcm(const void *pcm,unsigned bytes) {x360_audio_submit_checked(pcm,bytes);}
