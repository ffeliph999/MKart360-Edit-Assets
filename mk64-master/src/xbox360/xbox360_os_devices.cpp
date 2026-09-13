extern "C" void x360_log(const char*);
#include <xtl.h>
#include <string.h>
extern "C" {
#include <ultra64.h>
#include <PR/os.h>
#include "main.h"
#include "xbox360/platform.h"
void x360_post_os_event(unsigned int);
int x360_audio_submit_checked(const void*,unsigned);
}
static volatile LONG viStarted,viInterval=1,videoBlack;
static void *currentFramebuffer;
static OSViMode currentMode;
static u32 viFeatures;
static DWORD WINAPI retraces(void*) {
    LARGE_INTEGER frequency,next,now;QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&next);
    unsigned int frame=0;
    for(;;) {
        next.QuadPart+=frequency.QuadPart/60;
        do {Sleep(1);QueryPerformanceCounter(&now);}while(now.QuadPart<next.QuadPart);
        if(now.QuadPart-next.QuadPart>frequency.QuadPart/4)next=now;
        if(++frame>=(unsigned int)InterlockedCompareExchange(&viInterval,0,0)) {frame=0;x360_post_os_event(OS_EVENT_VI);}
    }
}
extern "C" void osCreateViManager(OSPri) {
    if(InterlockedCompareExchange(&viStarted,1,0)==0) {
        HANDLE thread=CreateThread(NULL,64*1024,retraces,NULL,0,NULL);
        if(!thread){x360_log("MK64: retrace service creation failed\n");DebugBreak();return;}
        CloseHandle(thread);
    }
}
extern "C" void osViSetEvent(OSMesgQueue *q,OSMesg msg,u32 count) {InterlockedExchange(&viInterval,count?count:1);osSetEventMesg(OS_EVENT_VI,q,msg);}
extern "C" void osViSetMode(OSViMode *mode) {if(mode)currentMode=*mode;}
extern "C" void osViSetSpecialFeatures(u32 flags) {viFeatures=flags;/* Native output defaults to gamma-off; no N64 VI filter. */}
extern "C" void osViBlack(u8 black) {InterlockedExchange(&videoBlack,black?1:0);}
extern "C" int x360_video_is_black(void) {return InterlockedCompareExchange(&videoBlack,0,0)!=0;}
extern "C" void osViSwapBuffer(void *buffer) {currentFramebuffer=buffer;/* D3D presents the native render target. */}
extern "C" u32 osAiGetLength(void) {return (u32)x360_audio_buffered_samples()*4;}
extern "C" s32 osAiSetFrequency(u32 rate) {return rate?32000:-1;/* Matches the native source voice rate. */}
extern "C" s32 osAiSetNextBuffer(void *buffer,u32 bytes) {return x360_audio_submit_checked(buffer,bytes)?0:-1;}
extern "C" void osSpTaskLoad(OSTask*) {/* Synchronous HLE uses the supplied task directly; no microcode DMA. */}
extern "C" void osSpTaskStartGo(OSTask *task) {
    if(!task)return;
    /* SPTask starts with OSTask in this project's ABI. */
    if(task->t.type==M_GFXTASK){x360_exec_sp_task((struct SPTask*)task);x360_post_os_event(OS_EVENT_DP);}
    else if(task->t.type==M_AUDTASK)x360_dispatch_audio_task((struct SPTask*)task);
    else {x360_log("MK64: unsupported RSP task type\n");DebugBreak();}
    x360_post_os_event(OS_EVENT_SP);
}
extern "C" void osSpTaskYield(void) {/* HLE calls complete synchronously before returning. */}
extern "C" OSYieldResult osSpTaskYielded(OSTask*) {return 0;}
/* There is no Controller Pak device in this port configuration. Report its
 * absence consistently, allowing the game's existing no-Pak UI to handle it.
 * EEPROM progression saves are implemented separately and remain available. */
extern "C" s32 osPfsIsPlug(OSMesgQueue*,u8 *pattern){if(pattern)*pattern=0;return 0;}
extern "C" s32 osPfsInit(OSMesgQueue*,OSPfs*,int){return PFS_ERR_NOPACK;}
extern "C" s32 osPfsFreeBlocks(OSPfs*,s32 *bytes){if(bytes)*bytes=0;return PFS_ERR_NOPACK;}
extern "C" s32 osPfsNumFiles(OSPfs*,s32 *max,s32 *used){if(max)*max=0;if(used)*used=0;return PFS_ERR_NOPACK;}
extern "C" s32 osPfsAllocateFile(OSPfs*,u16,u32,u8*,u8*,int,s32 *number){if(number)*number=-1;return PFS_ERR_NOPACK;}
extern "C" s32 osPfsFindFile(OSPfs*,u16,u32,u8*,u8*,s32 *number){if(number)*number=-1;return PFS_ERR_NOPACK;}
extern "C" s32 osPfsDeleteFile(OSPfs*,u16,u32,u8*,u8*){return PFS_ERR_NOPACK;}
extern "C" s32 osPfsReadWriteFile(OSPfs*,s32,u8,int,int,u8*){return PFS_ERR_NOPACK;}
extern "C" s32 osPfsFileState(OSPfs*,s32,OSPfsState *state){if(state)memset(state,0,sizeof(*state));return PFS_ERR_NOPACK;}
