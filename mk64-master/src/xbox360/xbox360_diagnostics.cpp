extern "C" void x360_log(const char*);
#include <xtl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "xbox360/netplay.h"
static volatile LONG logEnabled;
extern "C" int x360_logging_enabled(void) {return InterlockedCompareExchange(&logEnabled,0,0)!=0;}
extern "C" void x360_set_logging(int enabled) {
    InterlockedExchange(&logEnabled,enabled?1:0);
    HANDLE f=CreateFileA("game:\\mk64-logging.cfg",GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
    if(f!=INVALID_HANDLE_VALUE){char value=enabled?'1':'0';DWORD n;WriteFile(f,&value,1,&n,0);CloseHandle(f);}
}

extern "C" int _Printf(char *(*emit)(char*,const char*,size_t),char *dst,const char *format,va_list args) {
    va_list countArgs=args;
    int n=_vscprintf(format,countArgs);
    if(n<0 || !emit)return 0;
    char *buffer=(char*)malloc((size_t)n+1);
    if(!buffer)return 0;
    int written=_vsnprintf(buffer,(size_t)n+1,format,args);
    if(written<0){free(buffer);return 0;}
    char *result=emit(dst,buffer,(size_t)written);free(buffer);
    return result?written:0;
}
extern "C" void rmonPrintf(const char *format,...) {
    if(!x360_logging_enabled())return;
    char buffer[2048];va_list args;va_start(args,format);
    _vsnprintf(buffer,sizeof(buffer)-1,format,args);va_end(args);
    buffer[sizeof(buffer)-1]=0;x360_log(buffer);
}

/* Keep boot diagnostics accessible on consoles without a connected debugger.
 * Failure to open a read-only game folder never prevents startup. */
static CRITICAL_SECTION bootLogLock;
static volatile LONG bootLogReady;
static volatile LONG progressStage, progressSerial;
extern "C" void x360_progress(int stage) {
    if(!x360_logging_enabled())return;
    InterlockedExchange(&progressStage,stage);
    InterlockedIncrement(&progressSerial);
}
static DWORD WINAPI progress_watchdog(void*) {
    LONG last=-1; unsigned quiet=0;
    for (;;) {
        Sleep(1000);
        if(!x360_logging_enabled()){last=-1;quiet=0;continue;}
        LONG serial=InterlockedCompareExchange(&progressSerial,0,0);
        if(serial==last) ++quiet; else quiet=0;
        last=serial;
        if(quiet==5) {
            char message[160];
            _snprintf(message,sizeof(message)-1,"MK64: STALL 5 seconds stage=%ld progress=%ld (1 audio,2 transition,3 controllers,4 game,5 render,6 frame done)\n",progressStage,serial);
            message[sizeof(message)-1]=0;x360_log(message);
        }
    }
}
static void boot_log_lock(void) {
    if(InterlockedCompareExchange(&bootLogReady,1,0)==0) {
        InitializeCriticalSection(&bootLogLock);InterlockedExchange(&bootLogReady,2);
    } else while(InterlockedCompareExchange(&bootLogReady,0,0)!=2)Sleep(0);
    EnterCriticalSection(&bootLogLock);
}
extern "C" void x360_log_start(void) {
    HANDLE config=CreateFileA("game:\\mk64-logging.cfg",GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    if(config!=INVALID_HANDLE_VALUE){char value=0;DWORD n=0;ReadFile(config,&value,1,&n,0);CloseHandle(config);InterlockedExchange(&logEnabled,n==1&&value=='1');}

    boot_log_lock();
    HANDLE f=x360_logging_enabled()?CreateFileA("game:\\mk64-boot.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH,NULL):INVALID_HANDLE_VALUE;
    if(f!=INVALID_HANDLE_VALUE)CloseHandle(f);
    LeaveCriticalSection(&bootLogLock);
    HANDLE watchdog=CreateThread(NULL,0,progress_watchdog,NULL,0,NULL);
    if(watchdog)CloseHandle(watchdog);
}
extern "C" void x360_log(const char *message) {
    if(!x360_logging_enabled())return;
    OutputDebugStringA(message);
    boot_log_lock();
    HANDLE f=CreateFileA("game:\\mk64-boot.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH,NULL);
    if(f!=INVALID_HANDLE_VALUE){
        SetFilePointer(f,0,NULL,FILE_END);DWORD written;
        WriteFile(f,message,(DWORD)strlen(message),&written,NULL);CloseHandle(f);
    }
    LeaveCriticalSection(&bootLogLock);
}
