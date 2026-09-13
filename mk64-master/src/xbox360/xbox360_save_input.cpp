#include "xbox360/netplay.h"
#include <xtl.h>
#include <string.h>
extern "C" {
#include <ultra64.h>
}

#ifndef X360_EEPROM_PATH0
#define X360_EEPROM_PATH0 "game:\\mk64-save-0.eep"
#define X360_EEPROM_PATH1 "game:\\mk64-save-1.eep"
#endif
/* Two journal slots preserve the previous valid save if a write is interrupted.
 * On disk: magic[8], generation BE32, checksum BE32, 512 EEPROM bytes. */
static CRITICAL_SECTION saveLock;
static volatile LONG saveInit;
static unsigned char eeprom[512];
static unsigned int generation;
static int activeSlot=-1;
static const char *paths[2]={X360_EEPROM_PATH0,X360_EEPROM_PATH1};
static unsigned int get32(const unsigned char *p) {return ((unsigned int)p[0]<<24)|((unsigned int)p[1]<<16)|((unsigned int)p[2]<<8)|p[3];}
static void put32(unsigned char *p,unsigned int v) {p[0]=(unsigned char)(v>>24);p[1]=(unsigned char)(v>>16);p[2]=(unsigned char)(v>>8);p[3]=(unsigned char)v;}
static unsigned int checksum(const unsigned char *data,unsigned int gen) {
    unsigned int h=2166136261U^gen;for(int i=0;i<512;++i)h=(h^data[i])*16777619U;return h;
}
static bool readSlot(int slot,unsigned char *record) {
    HANDLE f=CreateFileA(paths[slot],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
    if(f==INVALID_HANDLE_VALUE)return false;
    DWORD got=0;bool ok=GetFileSize(f,NULL)==528 && ReadFile(f,record,528,&got,NULL) && got==528;CloseHandle(f);
    return ok && memcmp(record,"MK64EEP1",8)==0 && get32(record+12)==checksum(record+16,get32(record+8));
}
static void initializeSave(void) {
    if(InterlockedCompareExchange(&saveInit,1,0)==0) {
        InitializeCriticalSection(&saveLock);memset(eeprom,0xFF,sizeof(eeprom));
        unsigned char a[528],b[528];bool va=!x360_net_active() && readSlot(0,a),vb=!x360_net_active() && readSlot(1,b);
        if(va || vb) {
            activeSlot=vb && (!va || (int)(get32(b+8)-get32(a+8))>0)?1:0;
            const unsigned char *record=activeSlot?b:a;generation=get32(record+8);memcpy(eeprom,record+16,512);
        }
        InterlockedExchange(&saveInit,2);
    } else while(InterlockedCompareExchange(&saveInit,2,2)!=2)Sleep(0);
}
extern "C" s32 osEepromProbe(OSMesgQueue*) {initializeSave();return 1;}
extern "C" s32 osEepromLongRead(OSMesgQueue*,u8 address,u8 *data,s32 length) {
    unsigned int offset=(unsigned int)address*8;
    if(!data || length<0 || length%8 || offset>512 || (unsigned int)length>512-offset)return -1;
    initializeSave();EnterCriticalSection(&saveLock);memcpy(data,eeprom+offset,length);LeaveCriticalSection(&saveLock);return 0;
}
extern "C" s32 osEepromLongWrite(OSMesgQueue*,u8 address,u8 *data,s32 length) {
    unsigned int offset=(unsigned int)address*8;
    if(!data || length<0 || length%8 || offset>512 || (unsigned int)length>512-offset)return -1;
    initializeSave();EnterCriticalSection(&saveLock);
    if(x360_net_active()){memcpy(eeprom+offset,data,length);LeaveCriticalSection(&saveLock);return 0;}
    unsigned char record[528];memcpy(record,"MK64EEP1",8);put32(record+8,generation+1);
    memcpy(record+16,eeprom,512);memcpy(record+16+offset,data,length);put32(record+12,checksum(record+16,generation+1));
    int slot=activeSlot==0?1:0;
    HANDLE f=CreateFileA(paths[slot],GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,NULL);
    DWORD written=0;bool ok=false;
    if(f!=INVALID_HANDLE_VALUE){ok=WriteFile(f,record,528,&written,NULL) && written==528;CloseHandle(f);}
    unsigned char verify[528];ok=ok && readSlot(slot,verify) && memcmp(record,verify,528)==0;
    if(ok){memcpy(eeprom,record+16,512);++generation;activeSlot=slot;}
    LeaveCriticalSection(&saveLock);return ok?0:-1;
}
extern "C" s32 osContInit(OSMesgQueue*,u8 *bits,OSContStatus *status) {
    if(!bits || !status)return -1;*bits=0;
    for(unsigned int i=0;i<4;++i) {
        XINPUT_STATE state;memset(&state,0,sizeof(state));memset(&status[i],0,sizeof(status[i]));
        if(x360_net_active() ? i<(unsigned int)x360_net_player_count() : XInputGetState(i,&state)==ERROR_SUCCESS){*bits|=(u8)(1<<i);status[i].type=CONT_TYPE_NORMAL;}
        else status[i].errnum=CONT_NO_RESPONSE_ERROR;
    }
    return 0;
}
