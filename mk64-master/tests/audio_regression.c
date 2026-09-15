#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio/external.c"

static int online, players, slot,locals=1;
s32 gGamestate = RACING;
Player gPlayers[8];
Camera cameras[8];
Camera *camera1=&cameras[0], *camera2=&cameras[1], *camera3=&cameras[2], *camera4=&cameras[3];
struct SequencePlayer gSequencePlayers[SEQUENCE_PLAYERS];
struct SequenceChannel gSequenceChannelNone;
u32 gAudioRandom;
OSMesgQueue *D_800EA3B0, *D_800EA3B4;
s32 gPlayerCountSelection1, gScreenModeSelection, gModeSelection;
s32 gPlayerWinningIndex;
s16 gCurrentCourseId;
s8 D_801657E5;
u8 D_8018ED90;
s32 osSendMesg(OSMesgQueue *q, OSMesg msg, s32 flag) {return 0;}
s32 osRecvMesg(OSMesgQueue *q, OSMesg *msg, s32 flag) {*msg=0;return 0;}
void func_800CBB88(u32 cmd, f32 value) {}
void func_800CBBB8(u32 cmd, u32 value) {}
void func_800CBBE8(u32 cmd, s8 value) {}
void func_800CBC24(void) {}
void func_8001AAAC(s16 a,s16 b,s16 c) {}
f32 func_800416D8(f32 x, f32 z, u16 angle) {return x;}
f32 func_80041724(f32 x, f32 z, u16 angle) {return z;}
int x360_net_active(void) { return online; }
int x360_net_player_count(void) { return players; }
int x360_net_local_slot(void) { return slot; }
int x360_net_local_count(void) {return locals;}
int x360_net8_active(void) { return players > 4; }
static int checks;
#define CHECK(x) do { ++checks; if (!(x)) { printf("FAIL %d: %s\n",__LINE__,#x);exit(1); } } while(0)
int main(void) {
    int count, local, source;
    Player before[8];
    for (count=2; count<=8; ++count) for (local=0; local<count; ++local) {
        online=1;players=count;slot=local;gGamestate=RACING;
        D_800EA1C0=0;
        func_800C2474();sound_init();
        memcpy(before,gPlayers,sizeof(before));
        CHECK(x360_audio_player()==local);
        for(source=0;source<count;++source) {
            sSoundRequestCount=0;
            play_sound(0x0100FA28, &D_800E9F7C[source].pos, source, &D_800EA1D4,&D_800EA1D4,&D_800EA1DC);
            CHECK(sSoundRequestCount==(source==local));
            if (source==local) CHECK(sSoundRequests[0].cameraId==0);
        }
        sSoundRequestCount=0;
        play_sound(0x49008001, &D_800EA1C8, 4, &D_800EA1D4,&D_800EA1D4,&D_800EA1DC);
        CHECK(sSoundRequestCount==1);
        CHECK(sSoundRequests[0].cameraId==0);
        sSoundRequestCount=0;
        play_sound(0x51008001, &D_8018EFD8[1].unk18, 0, &D_800EA1D4,&D_800EA1D4,&D_800EA1DC);
        CHECK(sSoundRequestCount==1);
        CHECK(func_800C16E8(0,0,0)==64);
        CHECK(func_800C16E8(-100,0,0)<64);
        CHECK(func_800C16E8(100,0,0)>64);
        sSoundBanks[5][1].soundBits=0x51008001;
        sSoundBanks[5][1].distance=0;
        CHECK(func_800C1480(5,1)==1.0f);
        sSoundBanks[5][1].distance=2100;
        CHECK(func_800C1480(5,1)==0.0f);
        /* Only this racer's lap/finish/Star events may change music. */
        D_800EA1E4=0;D_8018FC08=0;
        for(source=0;source<count;++source) if(source!=local) func_800CA49C(source);
        CHECK(D_800EA1E4==0);CHECK(D_8018FC08==0);
        func_800CA49C(local);CHECK(D_800EA1E4>0);CHECK(D_8018FC08==1);
        D_800EA1E4=0;
        for(source=0;source<count;++source) if(source!=local) func_800CA59C(source);
        CHECK(D_800EA1E4==0);CHECK(D_800EA164==0);
        func_800CA59C(local);CHECK(D_800EA1E4>0);CHECK(D_800EA164==1);
        CHECK(D_800EA10C[local]==1);
        func_800CA730(local);CHECK(D_800EA164==0);CHECK(D_800EA10C[local]==0);
        for(source=0;source<count;++source) if(source!=local) func_800CA118(source);
        CHECK(D_800EA0F0==0);CHECK(D_800E9EA4[local]==0);
        gModeSelection=VERSUS;gPlayers[local].currentRank=1;
        before[local].currentRank=1;
        func_800CA118(local);CHECK(D_800E9EA4[local]==1);
        func_800C76C0(local);CHECK(D_800EA104==1);
        CHECK(D_80192CD0[(u8)(D_800EA1E4-2)]==(0xC1510000U|SEQ_MENU_RESULTS_SCREEN_LOSE));
        CHECK(memcmp(before,gPlayers,sizeof(before))==0);
        /* One listener gets every remote engine once, with stoppable emitters. */
        func_800C2474();sound_init();D_800EA1C0=0;sSoundRequestCount=0;
        for(source=0;source<count;++source) func_800C94A4(source);
        CHECK(sSoundRequestCount==count);
        CHECK(D_8018FB91==count-1);
        for(source=0;source<count;++source) if(source!=local) {
            u32 bits=gPlayers[source].characterId+0x31028000;
            CHECK(func_800C21E8(gPlayers[source].pos,bits)!=NULL);
            func_800C97C4(source);
            CHECK(func_800C21E8(gPlayers[source].pos,bits)==NULL);
        }
        /* A guest listener follows its own physical camera, including Doppler. */
        func_800C2474();D_800EA1C0=0;
        for(source=0;source<8;++source) {cameras[source].pos[0]=source*100.0f;cameras[source].pos[2]=0;}
        func_800C1F8C();
        CHECK(gCopyCamera[0]==&cameras[local]);
    }
    /* Every contiguous two-racer guest assignment, including P7/P8. */
    for(count=3;count<=8;++count)for(local=1;local+1<count;++local){
        online=1;players=count;slot=local;locals=2;gGamestate=RACING;D_800EA1C0=1;
        func_800C2474();sound_init();
        for(source=0;source<count;++source){
            int own=source==local||source==local+1;sSoundRequestCount=0;
            play_sound(0x0100FA28,&D_800E9F7C[source].pos,source,&D_800EA1D4,&D_800EA1D4,&D_800EA1DC);
            CHECK(sSoundRequestCount==own);if(own)CHECK(sSoundRequests[0].cameraId==source-local);
        }
        func_800C1F8C();CHECK(gCopyCamera[0]==&cameras[local]);CHECK(gCopyCamera[1]==&cameras[local+1]);
        CHECK(func_800C16E8(0,0,0)==0);CHECK(func_800C16E8(0,0,1)==127);
        sSoundRequestCount=0;
        for(source=0;source<count;++source)func_800C94A4(source);
        CHECK(sSoundRequestCount==2+2*(count-2));
        for(source=0;source<count;++source)if(source!=local&&source!=local+1){
            u32 bits=gPlayers[source].characterId+0x31028000;
            func_800C97C4(source);CHECK(func_800C21E8(gPlayers[source].pos,bits)==NULL);
        }
        /* Controller two gets local Star/lap cues without restarting TV music. */
        D_800EA1E4=0;D_8018FC08=0;func_800CA49C(local+1);CHECK(D_800EA1E4==0);
        func_800CA59C(local+1);CHECK(D_800EA10C[local+1]==1);CHECK(D_800EA164==0);CHECK(D_800EA1E4==0);
        func_800CA730(local+1);CHECK(D_800EA10C[local+1]==0);CHECK(D_800EA1E4==0);
        /* Finishing one local racer cannot silence the other local engine. */
        gModeSelection=VERSUS;func_800CA118(local);CHECK(!D_800EA0F0);
        func_800C76C0(local);CHECK(!D_800EA104);
        func_800CA118(local+1);CHECK(D_800EA0F0);func_800C76C0(local+1);CHECK(D_800EA104);
    }
    locals=1;
    online=0;D_800EA1C0=3;
    for(source=0;source<4;++source) {
        sSoundRequestCount=0;
        play_sound(0x0100FA28,&D_800E9F7C[source].pos,source,&D_800EA1D4,&D_800EA1D4,&D_800EA1DC);
        CHECK(sSoundRequestCount==1);
        CHECK(sSoundRequests[0].cameraId==source);
        CHECK(func_800C16E8(0,0,source)==(source&1)*127);
    }
    online=1;players=8;slot=7;gGamestate=0;
    CHECK(x360_audio_player()==-1);
    gGamestate=RACING;slot=8;CHECK(x360_audio_player()==-1);
    printf("PASS: %d production audio routing/panning/distance checks\n",checks);
    return 0;
}
