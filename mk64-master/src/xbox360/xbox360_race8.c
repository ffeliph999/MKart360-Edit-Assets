#include <ultra64.h>
#include <string.h>
#include <stdio.h>
#include <common_structs.h>
#include <defines.h>
#include <course.h>
#include <objects.h>
#include <actor_types.h>
#include <bomb_kart.h>
#include "xbox360/race8.h"
#include "xbox360/race8_state.h"
#include "main.h"
#include "menus.h"
#include "camera.h"
#include "spawn_players.h"
#include "code_800029B0.h"
#include "code_80057C60.h"
#include "cpu_vehicles_camera_path.h"
#include "racing/race_logic.h"
#include "racing/skybox_and_splitscreen.h"
#include "audio/external.h"
#include "math_util.h"

static Race8Results results;
static unsigned int resultsFrame;
static Player simulationPlayers[8];
static Object simulationObjects[OBJECT_LIST_SIZE];
static struct Actor simulationActors[ACTOR_LIST_SIZE];
static BombKart simulationBombs[NUM_BOMB_KARTS_MAX];
extern s16 gPlayerBalloonCount[];
extern u16 gRandomSeed16;
extern s32 gLapCountByPlayerId[];
extern s32 D_80164A28;

void x360_race8_prepare(void) {
    gPlayerCount = x360_net_player_count();
    gPlayerCountSelection1 = gPlayerCount;
    gScreenModeSelection = SCREEN_MODE_1P;
    gActiveScreenMode = SCREEN_MODE_1P;
    gCurrentCourseId = x360_net8_course();
    gModeSelection = (gCurrentCourseId==COURSE_BLOCK_FORT || gCurrentCourseId==COURSE_SKYSCRAPER ||
        gCurrentCourseId==COURSE_DOUBLE_DECK || gCurrentCourseId==COURSE_BIG_DONUT) ? BATTLE : VERSUS;
    gCCSelection = x360_net8_cc();
    {int i;for(i=0;i<4;++i)gCharacterSelections[i]=(s8)x360_net8_character(i);}
    gDemoMode=0; gDemoUseController=0; gEnableDebugMode=0;
    gIsMirrorMode=0; gIsGamePaused=0;
    gGamestateNext=RACING;
    race8_results_init(&results,gPlayerCount);
    resultsFrame=0;
}

void x360_race8_service(void) {
    /* Race changes are requested by synchronized controls after the preceding frame. */
}

void x360_race8_controls(void) {
    int i;
    if(results.done) {
        if(x360_net_frame()-resultsFrame < 60) return;
        if(gControllers[0].buttonPressed & (A_BUTTON|B_BUTTON)) {
            if(gControllers[0].buttonPressed&B_BUTTON) x360_net8_configure();
            x360_race8_prepare();
            setup_race();
        }
        return;
    }
    if(gRaceState!=RACE_IN_PROGRESS) return;
    for(i=0;i<x360_net_player_count();++i) if(gControllers[i].buttonPressed&START_BUTTON) {
        /* One toggle per network frame, with an identical winner for simultaneous requests. */
        gIsGamePaused=gIsGamePaused?0:i+1;
        func_800C9F90(gIsGamePaused?1:0);
        break;
    }
}

void x360_race8_spawn(void) {
    int i,n=x360_net_player_count();
    float x=0,y=0,z=0;
    if(gModeSelection==BATTLE) func_8000EEDC();
    else {
        init_course_path_point();
        x=(float)gTrackPaths[0][0].posX; y=(float)gTrackPaths[0][0].posY; z=(float)gTrackPaths[0][0].posZ;
        if(gCurrentCourseId==COURSE_TOADS_TURNPIKE) x=0;
    }
    for(i=0;i<8;++i) {
        float px=x+((i&1)?-20.0f:20.0f), pz=z+30.0f+20.0f*i, py=y, yaw=32768.0f;
        if(gModeSelection==BATTLE) {
            float radius=200.0f,offset=(i>=4)?45.0f:-45.0f;
            py=5;
            if(gCurrentCourseId==COURSE_SKYSCRAPER) {radius=400; py=480;}
            if(gCurrentCourseId==COURSE_DOUBLE_DECK) {radius=160; py=55;}
            if(gCurrentCourseId==COURSE_BIG_DONUT) {radius=575; py=200;}
            switch(i&3) {
                case 0:px=offset;pz=radius;yaw=32768;break;
                case 1:px=offset;pz=-radius;yaw=0;break;
                case 2:px=-radius;pz=offset;yaw=-16384;break;
                default:px=radius;pz=offset;yaw=16384;break;
            }
            /* Face along Big Donut's ring, as the original battle grid does. */
            if(gCurrentCourseId==COURSE_BIG_DONUT) {
                static const float ringYaw[4]={-16384,16384,0,32768};
                yaw=ringYaw[i&3];
            }
        }
        spawn_player(&gPlayers[i],(s8)i,px,pz,py,yaw,(s16)x360_net8_character(i),
            (u16)(i<n?(PLAYER_EXISTS|PLAYER_START_SEQUENCE|PLAYER_HUMAN):0));
    }
    D_80164A28=0;
    func_80039AE4();
    race8_results_init(&results,n);
}

void x360_race8_cameras(void) {
    int i;
    for(i=0;i<x360_net_player_count();++i) {
        func_8001EE98(&gPlayers[i],&cameras[i],(s8)i);
        gCameraZoom[i]=cameras[i].unk_B4;
    }
}

/* Rendering uses viewport zero, but each console chooses its own canonical camera.
 * Restore gameplay state because legacy render routines also modify object flags,
 * player visibility and the shared RNG. No local view may affect lockstep physics. */
void x360_race8_render(void) {
    int local=x360_net_local_slot();
    Camera saved=cameras[0];
    Camera *hudCamera=D_8018CF14;
    int lakitu=gIndexLakituList[0];
    float zoom=gCameraZoom[0];
    Player *viewPlayer=D_800DC5EC->player;
    u16 seed=gRandomSeed16;
    memcpy(simulationPlayers,gPlayers,sizeof(simulationPlayers));
    memcpy(simulationObjects,gObjectList,sizeof(simulationObjects));
    memcpy(simulationActors,gActorList,sizeof(simulationActors));
    memcpy(simulationBombs,gBombKarts,sizeof(simulationBombs));
    cameras[0]=cameras[local];
    D_8018CF14=&cameras[0];
    gIndexLakituList[0]=gIndexLakituList[local];
    gCameraZoom[0]=gCameraZoom[local];
    D_800DC5EC->player=&gPlayers[local];
    render_player_one_1p_screen();
    D_800DC5EC->player=viewPlayer;
    cameras[0]=saved; gCameraZoom[0]=zoom;
    D_8018CF14=hudCamera; gIndexLakituList[0]=lakitu;
    memcpy(gPlayers,simulationPlayers,sizeof(simulationPlayers));
    memcpy(gObjectList,simulationObjects,sizeof(simulationObjects));
    memcpy(gActorList,simulationActors,sizeof(simulationActors));
    memcpy(gBombKarts,simulationBombs,sizeof(simulationBombs));
    gRandomSeed16=seed;
}

int x360_race8_rules(void) {
    int i,j,n=results.players,battle=gModeSelection==BATTLE;
    int finished[8],number=0;
    if(gRaceState<RACE_IN_PROGRESS) return 0; /* Original staging and Lakitu countdown. */
    if(results.done) return 1;
    if(!battle) update_race_position_data();
    for(i=0;i<n;++i) {
        if(results.place[i]>=0) continue;
        if(battle) {
            if(gPlayerBalloonCount[i]<0) finished[number++]=i;
        } else {
            gPlayers[i].lapCount=gLapCountByPlayerId[i];
            if(gPlayers[i].lapCount>=2 && !results.finalLap[i]) {results.finalLap[i]=1;func_800CA49C((u8)i);}
            if(gPlayers[i].lapCount>=3) finished[number++]=i;
        }
    }
    /* Resolve same-tick finishes by interpolated crossing time, then slot for exact ties. */
    if(!battle) for(i=1;i<number;++i) {
        int slot=finished[i]; float t=func_8028EE8C(slot);
        for(j=i;j>0 && func_8028EE8C(finished[j-1])>t;--j) finished[j]=finished[j-1];
        finished[j]=slot;
    }
    for(i=0;i<number;++i) if(race8_finish(&results,finished[i],battle)) {
        int slot=finished[i];
        func_800CA118((u8)slot);
        /* Use the original automatic driving after a VS finish. */
        gPlayers[slot].type |= PLAYER_CPU | PLAYER_CINEMATIC_MODE;
        gPlayers[slot].currentItemCopy=0;
        if(battle) gPlayers[slot].type &= ~PLAYER_EXISTS;
    }
    if(results.count==n-1) {
        for(i=0;i<n;++i) if(results.place[i]<0) {
            func_800CA118((u8)i);
            gPlayers[i].type |= PLAYER_CPU | PLAYER_CINEMATIC_MODE;
            gPlayers[i].currentItemCopy=0;
        }
    }
    race8_complete_last(&results,battle);
    for(i=0;i<n;++i) if(results.place[i]>=0) gPlayers[i].currentRank=(s16)results.place[i];
    if(results.done) {
        gRaceState=RACE_DONE; gIsGamePaused=1; resultsFrame=x360_net_frame();
        gPlayerWinningIndex=results.order[0];
        for(i=0;i<n;++i) gPlayers[i].type |= PLAYER_START_SEQUENCE;
    }
    return 1;
}

int x360_race8_hud_line(int row,char *out,int size) {
    static const char *names[]={"MARIO","LUIGI","YOSHI","TOAD","DK","WARIO","PEACH","BOWSER"};
    static const char *items[]={"NONE","BANANA","BANANA BUNCH","GREEN SHELL","3 GREEN SHELLS","RED SHELL","3 RED SHELLS","BLUE SHELL","LIGHTNING","FAKE ITEM BOX","STAR","BOO","MUSHROOM","2 MUSHROOMS","3 MUSHROOMS","GOLD MUSHROOM"};
    int local=x360_net_local_slot(),lap=gPlayers[local].lapCount+1,item=gPlayers[local].currentItemCopy;
    if(!x360_net8_active() || gGamestate!=RACING) return 0;
    out[0]=0;
    if(lap<1)lap=1;if(lap>3)lap=3;if(item<0||item>15)item=0;
    if(row==0) _snprintf(out,size,"P%d %s   %d PLAYERS",local+1,names[gPlayers[local].characterId&7],results.players);
    else if(row==1) {
        if(gModeSelection==BATTLE) _snprintf(out,size,"BALLOONS %d   ITEM: %s",gPlayerBalloonCount[local]+1,items[item]);
        else _snprintf(out,size,"PLACE %d/%d   LAP %d/3   ITEM: %s",gPlayers[local].currentRank+1,results.players,lap,items[item]);
    } else if(row==2) {
        if(results.done) _snprintf(out,size,"RESULTS - HOST A: REMATCH   B: CHANGE COURSE");
        else if(gIsGamePaused) _snprintf(out,size,"PAUSED BY P%d - START TO RESUME",gIsGamePaused);
        else if(gRaceState<RACE_IN_PROGRESS) _snprintf(out,size,"GET READY");
        else if(results.place[local]>=0) _snprintf(out,size,"FINISHED - WAITING FOR THE OTHER RACERS");
        else _snprintf(out,size,"TIME %d:%02d.%02d",(int)gCourseTimer/60,(int)gCourseTimer%60,(int)(gCourseTimer*100)%100);
    } else if(results.done && row>=3 && row<3+results.players) {
        int slot=results.order[row-3];
        _snprintf(out,size,"%d. P%d  %s",row-2,slot+1,names[gPlayers[slot].characterId&7]);
    }
    out[size-1]=0;
    return out[0]!=0;
}

unsigned int x360_race8_hash(unsigned int h) {
    int i;
    h=(h^(unsigned int)gIsGamePaused)*16777619U;
    h=(h^(unsigned int)gRaceState)*16777619U;
    h=(h^(unsigned int)results.count)*16777619U;
    for(i=0;i<results.players;++i) {
        h=(h^(unsigned int)results.place[i])*16777619U;
        h=(h^(unsigned int)gPlayers[i].currentItemCopy)*16777619U;
        h=(h^(unsigned int)gLapCountByPlayerId[i])*16777619U;
        h=(h^(unsigned int)gPlayerBalloonCount[i])*16777619U;
    }
    return h;
}
