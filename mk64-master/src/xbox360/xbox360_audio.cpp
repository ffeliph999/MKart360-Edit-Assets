#include <xtl.h>
#include <xaudio2.h>
#include <string.h>
#include <stdio.h>
#include "xbox360/platform.h"
#include "xbox360/netplay.h"
#include "xbox360/audio_queue.h"

static IXAudio2 *g_xa;
static IXAudio2MasteringVoice *g_master;
static IXAudio2SourceVoice *g_voice;
static BYTE g_ring[mkaudio::RING_COUNT][mkaudio::RING_BYTES];
static unsigned g_ring_pos,g_submits,g_starved,g_rejected;
static UINT64 g_submitted_samples;
static bool g_playing,g_was_empty;
static unsigned g_rate=mkaudio::RATE;
static DWORD g_last_report;
static void report(HRESULT hr){
    if(!x360_logging_enabled())return;DWORD now=GetTickCount();if(now-g_last_report<1000)return;g_last_report=now;
    XAUDIO2_VOICE_STATE state;ZeroMemory(&state,sizeof(state));if(g_voice)g_voice->GetState(&state);
    char message[256];_snprintf(message,sizeof(message)-1,"MK64: audio rate=%u submitted=%u queued=%u samples=%u starved=%u rejected=%u started=%u hr=%08X\n",g_rate,g_submits,state.BuffersQueued,mkaudio::buffered(g_submitted_samples,state.SamplesPlayed),g_starved,g_rejected,g_playing?1:0,(unsigned)hr);message[sizeof(message)-1]=0;x360_log(message);
}
static void release_audio(){if(g_voice){g_voice->DestroyVoice();g_voice=0;}if(g_master){g_master->DestroyVoice();g_master=0;}if(g_xa){g_xa->Release();g_xa=0;}}
extern "C" int x360_audio_init(void){
    if(g_voice)return 1;
    g_ring_pos=g_submits=g_starved=g_rejected=0;g_submitted_samples=0;g_playing=g_was_empty=false;
    HRESULT hr=XAudio2Create(&g_xa,0,XAUDIO2_DEFAULT_PROCESSOR);
    if(SUCCEEDED(hr))hr=g_xa->CreateMasteringVoice(&g_master);
    WAVEFORMATEX wf;ZeroMemory(&wf,sizeof(wf));wf.wFormatTag=WAVE_FORMAT_PCM;wf.nChannels=2;wf.nSamplesPerSec=g_rate;wf.wBitsPerSample=16;wf.nBlockAlign=4;wf.nAvgBytesPerSec=g_rate*4;
    if(SUCCEEDED(hr))hr=g_xa->CreateSourceVoice(&g_voice,&wf,0,2.0f);
    if(FAILED(hr)){report(hr);release_audio();return 0;}
    x360_log("MK64: audio queue target 100ms, 3-buffer startup, continuous underrun recovery\n");return 1;
}
extern "C" int x360_audio_set_frequency(unsigned rate){
    if(!g_voice||rate<8000||rate>48000)return -1;
    if(rate==g_rate)return (int)rate;
    XAUDIO2_VOICE_STATE state;g_voice->GetState(&state);
    if(state.BuffersQueued)return -1;
    HRESULT hr=g_voice->SetSourceSampleRate(rate);if(FAILED(hr)){report(hr);return -1;}g_rate=rate;return (int)rate;
}
extern "C" int x360_audio_buffered_samples(void){
    if(!g_voice)return 0;XAUDIO2_VOICE_STATE state;g_voice->GetState(&state);
    return (int)mkaudio::buffered(g_submitted_samples,state.SamplesPlayed);
}
extern "C" int x360_audio_desired_samples(void){return mkaudio::DESIRED;}
extern "C" int x360_audio_generation_size(int nominal,unsigned queued,int minimum,int maximum){return mkaudio::generation_size(nominal,queued,minimum,maximum);}
extern "C" int x360_audio_submit_checked(const void*pcm,unsigned bytes){
    if(!g_voice||!pcm){++g_rejected;return 0;}
    XAUDIO2_VOICE_STATE state;g_voice->GetState(&state);
    if(!mkaudio::can_submit(bytes,state.BuffersQueued)){++g_rejected;report(E_FAIL);return 0;}
    const bool empty=state.BuffersQueued==0;
    if(empty&&g_playing&&!g_was_empty)++g_starved;
    g_was_empty=empty;
    // Source buffers remain alive in FIFO order until XAudio has consumed them.
    // Do not stop a running voice on starvation: the next submission resumes it.
    BYTE *destination=g_ring[g_ring_pos];memcpy(destination,pcm,bytes);
    XAUDIO2_BUFFER b;ZeroMemory(&b,sizeof(b));b.AudioBytes=bytes;b.pAudioData=destination;
    HRESULT hr=g_voice->SubmitSourceBuffer(&b);
    if(FAILED(hr)){++g_rejected;report(hr);return 0;}
    ++g_submits;g_submitted_samples+=bytes/4;g_ring_pos=(g_ring_pos+1)%mkaudio::RING_COUNT;
    if(!g_playing){XAUDIO2_VOICE_STATE ready;g_voice->GetState(&ready);if(mkaudio::should_start(g_playing,ready.BuffersQueued)){hr=g_voice->Start(0);if(SUCCEEDED(hr))g_playing=true;}}
    // Preserve stereo, including intentionally silent channels and panned effects.
    report(hr);return 1;
}
extern "C" void x360_audio_submit_pcm(const void*pcm,unsigned bytes){x360_audio_submit_checked(pcm,bytes);}
