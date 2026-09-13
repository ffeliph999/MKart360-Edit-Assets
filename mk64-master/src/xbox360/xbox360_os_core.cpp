extern "C" void x360_log(const char*);
#include <xtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C" {
#include <ultra64.h>
}

/* N64 queue/thread objects retain their public ABI. Native synchronization
 * lives in side tables, never in the N64 saved-register fields. */
struct QueueState {
    OSMesgQueue *key;
    CRITICAL_SECTION lock;
    HANDLE ready, space;
    QueueState *next;
};
struct ThreadState {
    OSThread *key;
    void (*entry)(void*);
    void *arg;
    HANDLE handle;
    bool started;
    ThreadState *next;
};
static CRITICAL_SECTION registry;
static volatile LONG initialization;
static QueueState *queues;
static ThreadState *threads;
static LARGE_INTEGER frequency, epoch;
static LARGE_INTEGER counterEpoch;
static OSThread threadTail;
static u64 timeBase;
static OSMesgQueue *eventQueues[OS_NUM_EVENTS];
static OSMesg eventMessages[OS_NUM_EVENTS];

static void fail(const char *message) {
    x360_log(message);
    DebugBreak();
    abort();
}
static void initialize(void) {
    if(InterlockedCompareExchange(&initialization,1,0)==0) {
        InitializeCriticalSection(&registry);
        QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&epoch);
        counterEpoch=epoch;threadTail.priority=-1;
        InterlockedExchange(&initialization,2);
    } else while(InterlockedCompareExchange(&initialization,2,2)!=2) Sleep(0);
}
static QueueState *findQueue(OSMesgQueue *q) {
    initialize();EnterCriticalSection(&registry);
    QueueState *s=queues;
    while(s && s->key!=q)s=s->next;
    LeaveCriticalSection(&registry);
    if(!s)fail("MK64: uninitialized message queue\n");
    return s;
}
extern "C" void osCreateMesgQueue(OSMesgQueue *q,OSMesg *buffer,s32 count) {
    if(!q || !buffer || count<=0)fail("MK64: invalid message queue\n");
    initialize();EnterCriticalSection(&registry);
    QueueState *s=queues;
    while(s && s->key!=q)s=s->next;
    if(!s) {
        s=(QueueState*)calloc(1,sizeof(*s));if(!s)fail("MK64: queue allocation failed\n");
        s->key=q;InitializeCriticalSection(&s->lock);s->next=queues;queues=s;
    } else {CloseHandle(s->ready);CloseHandle(s->space);}
    s->ready=CreateSemaphore(NULL,0,count,NULL);s->space=CreateSemaphore(NULL,count,count,NULL);
    if(!s->ready || !s->space)fail("MK64: queue semaphore creation failed\n");
    q->mtqueue=NULL;q->fullqueue=NULL;q->validCount=0;q->first=0;q->msgCount=count;q->msg=buffer;
    LeaveCriticalSection(&registry);
}
extern "C" s32 osSendMesg(OSMesgQueue *q,OSMesg msg,s32 flag) {
    QueueState *s=findQueue(q);
    if(WaitForSingleObject(s->space,flag==OS_MESG_BLOCK?INFINITE:0)!=WAIT_OBJECT_0)return -1;
    EnterCriticalSection(&s->lock);
    q->msg[(q->first+q->validCount)%q->msgCount]=msg;++q->validCount;
    LeaveCriticalSection(&s->lock);ReleaseSemaphore(s->ready,1,NULL);return 0;
}
extern "C" s32 osRecvMesg(OSMesgQueue *q,OSMesg *msg,s32 flag) {
    QueueState *s=findQueue(q);
    if(WaitForSingleObject(s->ready,flag==OS_MESG_BLOCK?INFINITE:0)!=WAIT_OBJECT_0)return -1;
    EnterCriticalSection(&s->lock);
    if(msg)*msg=q->msg[q->first];q->first=(q->first+1)%q->msgCount;--q->validCount;
    LeaveCriticalSection(&s->lock);ReleaseSemaphore(s->space,1,NULL);return 0;
}
extern "C" void osSetEventMesg(OSEvent event,OSMesgQueue *q,OSMesg msg) {
    if(event>=OS_NUM_EVENTS)fail("MK64: invalid event\n");
    initialize();EnterCriticalSection(&registry);eventQueues[event]=q;eventMessages[event]=msg;LeaveCriticalSection(&registry);
}
extern "C" void x360_post_os_event(unsigned int event) {
    if(event>=OS_NUM_EVENTS)return;
    initialize();EnterCriticalSection(&registry);
    OSMesgQueue *q=eventQueues[event];OSMesg msg=eventMessages[event];
    LeaveCriticalSection(&registry);if(q)osSendMesg(q,msg,OS_MESG_NOBLOCK);
}
static int nativePriority(OSPri p) {return p>=100?THREAD_PRIORITY_HIGHEST:p>=20?THREAD_PRIORITY_ABOVE_NORMAL:THREAD_PRIORITY_NORMAL;}
static int logThreadFault(ThreadState *s,EXCEPTION_POINTERS *info) {
    char message[256];
    _snprintf(message,sizeof(message)-1,"MK64: THREAD FAULT id=%d code=%08lX address=%p access=%lu target=%08lX\n",
        s->key->id,info->ExceptionRecord->ExceptionCode,info->ExceptionRecord->ExceptionAddress,
        info->ExceptionRecord->NumberParameters>0?info->ExceptionRecord->ExceptionInformation[0]:0,
        info->ExceptionRecord->NumberParameters>1?info->ExceptionRecord->ExceptionInformation[1]:0);
    message[sizeof(message)-1]=0;x360_log(message);return EXCEPTION_EXECUTE_HANDLER;
}
static DWORD WINAPI threadEntry(void *arg) {
    ThreadState *s=(ThreadState*)arg;
    __try {s->entry(s->arg);}
    __except(logThreadFault(s,GetExceptionInformation())) {
        /* Keep the fault recorded rather than allowing an unhandled exception
         * to take down the console title without any diagnostic evidence. */
        for(;;)Sleep(1000);
    }
    return 0;
}
extern "C" void osCreateThread(OSThread *t,OSId id,void (*entry)(void*),void *arg,void *stack,OSPri priority) {
    (void)stack;
    initialize();EnterCriticalSection(&registry);
    ThreadState *s=threads;while(s && s->key!=t)s=s->next;
    if(s)fail("MK64: recreating a live native thread\n");
    s=(ThreadState*)calloc(1,sizeof(*s));if(!s)fail("MK64: thread allocation failed\n");
    s->key=t;s->entry=entry;s->arg=arg;s->next=threads;threads=s;
    memset(t,0,sizeof(*t));t->id=id;t->priority=priority;
    t->tlnext=s->next?s->next->key:&threadTail;
    s->handle=CreateThread(NULL,64*1024,threadEntry,s,CREATE_SUSPENDED,NULL);
    if(!s->handle)fail("MK64: thread creation failed\n");
    SetThreadPriority(s->handle,nativePriority(priority));LeaveCriticalSection(&registry);
}
extern "C" void osStartThread(OSThread *t) {
    initialize();EnterCriticalSection(&registry);ThreadState *s=threads;while(s && s->key!=t)s=s->next;
    if(!s)fail("MK64: starting unknown thread\n");
    if(!s->started){s->started=true;ResumeThread(s->handle);}LeaveCriticalSection(&registry);
}
extern "C" void osSetThreadPri(OSThread *t,OSPri priority) {
    initialize();EnterCriticalSection(&registry);
    HANDLE handle=GetCurrentThread();
    if(t){ThreadState *s=threads;while(s && s->key!=t)s=s->next;if(!s)fail("MK64: unknown thread priority\n");handle=s->handle;t->priority=priority;}
    SetThreadPriority(handle,nativePriority(priority));LeaveCriticalSection(&registry);
}
extern "C" OSTime osGetTime(void) {
    initialize();EnterCriticalSection(&registry);LARGE_INTEGER now;QueryPerformanceCounter(&now);
    u64 delta=(u64)(now.QuadPart-epoch.QuadPart),f=(u64)frequency.QuadPart;
    u64 ticks=timeBase+(delta/f)*46875000ULL+((delta%f)*46875000ULL)/f;
    LeaveCriticalSection(&registry);return ticks;
}
extern "C" void osSetTime(OSTime value) {initialize();EnterCriticalSection(&registry);QueryPerformanceCounter(&epoch);timeBase=value;LeaveCriticalSection(&registry);}
extern "C" u32 osGetCount(void) {
    initialize();LARGE_INTEGER now;QueryPerformanceCounter(&now);
    u64 delta=(u64)(now.QuadPart-counterEpoch.QuadPart),f=(u64)frequency.QuadPart;
    return (u32)((delta/f)*46875000ULL+((delta%f)*46875000ULL)/f);
}
extern "C" OSThread *__osGetCurrFaultedThread(void) {
    initialize();EnterCriticalSection(&registry);OSThread *head=threads?threads->key:&threadTail;LeaveCriticalSection(&registry);return head;
}
extern "C" void osInitialize(void) {initialize();}
extern "C" {
u64 osClockRate=62500000;
u32 osTvType=1;
u32 osResetType=0;
s32 osAppNmiBuffer[16]={0};
}

extern "C" void x360_sleep_ms(unsigned milliseconds) {Sleep(milliseconds);}
