#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "xbox360/frame_pacing.h"
#include "xbox360/diagnostic_options.h"
static unsigned checks,diskCalls,debugCalls,lockCalls,sleepCalls,hashCalls,scanCalls;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL %d: %s\n",__LINE__,#x);exit(1);}}while(0)
typedef int64_t LONGLONG;
struct LARGE_INTEGER{int64_t QuadPart;};
typedef void *HANDLE;
typedef unsigned DWORD;
static int64_t clockNow,presentCost,g_next_tick;
static LARGE_INTEGER g_clock_frequency={30000};
#define D3DCLEAR_TARGET 1
#define GENERIC_WRITE 1
#define FILE_SHARE_READ 1
#define OPEN_ALWAYS 1
#define FILE_ATTRIBUTE_NORMAL 1
#define FILE_FLAG_WRITE_THROUGH 1
#define FILE_END 1
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
static int bootLogLock;
void OutputDebugStringA(const char*){++debugCalls;}
void boot_log_lock(){++lockCalls;}
void LeaveCriticalSection(int*){++lockCalls;}
HANDLE CreateFileA(const char*,int,int,void*,int,int,void*){++diskCalls;return INVALID_HANDLE_VALUE;}
void SetFilePointer(HANDLE,int,void*,int){++diskCalls;}
void WriteFile(HANDLE,const char*,DWORD,DWORD*,void*){++diskCalls;}
void CloseHandle(HANDLE){++diskCalls;}
void Sleep(unsigned ms){++sleepCalls;clockNow+=ms*30;}
void QueryPerformanceCounter(LARGE_INTEGER *p){p->QuadPart=clockNow;}
int x360_video_is_black(){return 0;}
struct Device{void Clear(int,int,int,unsigned,float,int){} void Present(int,int,int,int){clockNow+=presentCost;}};
static Device device,*g_dev=&device;
#include "build/performance_production.inc"

struct Gfx{};
struct Texture{const unsigned char *addr;unsigned load_capture_hash;};
static unsigned char data[4096];
static struct {const unsigned char *palette;unsigned palette_bytes,palette_load_capture_hash;Texture loaded_texture[1];} rdp;
unsigned x360_texture_hash(const unsigned char*,unsigned){++hashCalls;return 123;}
Gfx *x360_preflight_bad_vtx(Gfx *p){++scanCalls;return p;}
#include "build/performance_graphics.inc"
int main(){
 CHECK(MK64_ENABLE_LOGGER_OPTIONS==0);
 x360_log_start();x360_set_logging(1);CHECK(!x360_logging_enabled());
 x360_log("must not write");x360_set_logging(0);
 CHECK(diskCalls==0&&debugCalls==0&&lockCalls==0);
 rdp.palette=data;rdp.palette_bytes=512;rdp.loaded_texture[0].addr=data;
 diagnostics();CHECK(hashCalls==0&&scanCalls==0);
 CHECK(rdp.palette_load_capture_hash==0&&rdp.loaded_texture[0].load_capture_hash==0);
 /* Native 30 Hz work budgets: all late frames return without an extra sleep. */
 for(int work=0;work<=5000;++work){
  g_next_tick=100000;clockNow=g_next_tick+work;presentCost=0;sleepCalls=0;
  x360_present_and_pace();
  if(work>=1000){CHECK(sleepCalls==0);CHECK(clockNow==100000+work);CHECK(g_next_tick==clockNow);}
  else{CHECK(clockNow>=101000&&clockNow<101030);CHECK(g_next_tick==101000);}
 }
 /* GPU Present time counts toward the same budget. */
 g_next_tick=100000;clockNow=100300;presentCost=800;sleepCalls=0;
 x360_present_and_pace();CHECK(sleepCalls==0&&clockNow==101100);
 presentCost=0;clockNow+=200;x360_present_and_pace();CHECK(g_next_tick==102100);
 /* Synthetic recurring workload, no catch-up bursts after a network stall. */
 g_next_tick=100000;clockNow=g_next_tick;
 for(int frame=0;frame<600;++frame){
  int64_t last=g_next_tick;int work=frame%60==0?4500:200;
  clockNow+=work;sleepCalls=0;x360_present_and_pace();
  CHECK(clockNow-last>=1000);CHECK(clockNow-last<=work+1000);
  if(work>1000)CHECK(sleepCalls==0);
 }
 printf("PASS logging-off and frame pacing: %u checks\n",checks);
 return 0;
}
