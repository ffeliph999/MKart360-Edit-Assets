#include <stdlib.h>
#include "xbox360/xbox360_race8.c"

static int checks,netCount=8,netSlot,course,cc=1,configured,setupCount,cameraCalls,finishCalls[8],lapCalls[8];
static unsigned int frame=100;
#define CHECK(x) do {++checks;if(!(x)){printf("FAIL line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
Player gPlayers[8];
struct Controller gControllers[9];
Camera cameras[8];
Camera *D_8018CF14=&cameras[0];
Object gObjectList[OBJECT_LIST_SIZE];
struct Actor gActorList[ACTOR_LIST_SIZE];
BombKart gBombKarts[NUM_BOMB_KARTS_MAX];
struct UnkStruct_800DC5EC viewport;
struct UnkStruct_800DC5EC *D_800DC5EC=&viewport;
f32 gCameraZoom[8],gCourseTimer;
u16 gRandomSeed16,gIsGamePaused,gDemoMode,gEnableDebugMode;
s8 gPlayerCount,gDemoUseController,gCharacterSelections[4];
s32 gRaceState,gPlayerCountSelection1,gScreenModeSelection,gActiveScreenMode,gCCSelection,gModeSelection,gIsMirrorMode,gGamestateNext,gGamestate=RACING;
s32 gLapCountByPlayerId[10],D_80164A28,gPlayerWinningIndex,gIndexLakituList[8];
s16 gCurrentCourseId,gPlayerBalloonCount[8];
TrackPathPoint track[2];
TrackPathPoint *gTrackPaths[4]={track,track,track,track};
int x360_net8_active(void){return 1;}
int x360_net_player_count(void){return netCount;}
int x360_net_local_slot(void){return netSlot;}
unsigned int x360_net_frame(void){return frame;}
int x360_net8_course(void){return course;}
int x360_net8_cc(void){return cc;}
int x360_net8_character(int slot){return slot;}
void x360_net8_configure(void){++configured;}
void func_800C9F90(u8 paused){CHECK(paused==0||paused==1);}
void func_800CA118(u8 slot){CHECK(slot<netCount);++finishCalls[slot];}
void func_800CA49C(u8 slot){CHECK(slot<netCount);++lapCalls[slot];}
void update_race_position_data(void){}
f32 func_8028EE8C(s32 slot){return 10.0f-slot*0.01f;}
void func_8000EEDC(void){}
void init_course_path_point(void){}
void func_80039AE4(void){}
void spawn_player(Player *p,s8 id,f32 x,f32 z,f32 y,f32 yaw,u16 character,s16 type){
    memset(p,0,sizeof(*p));p->type=type;p->characterId=character;
    p->pos[0]=x;p->pos[1]=y;p->pos[2]=z;p->rotation[1]=(s16)yaw;
    CHECK(id>=0&&id<8);
}
void setup_race(void){++setupCount;x360_race8_spawn();gRaceState=RACE_NONE;}
void func_8001EE98(Player *p,Camera *camera,s8 index){
    CHECK(p==&gPlayers[index]);CHECK(camera==&cameras[index]);++cameraCalls;
    camera->pos[0]=(f32)index;camera->unk_B4=40.0f+index;
}
void render_player_one_1p_screen(void){
    CHECK(cameras[0].playerId==netSlot);CHECK(viewport.player==&gPlayers[netSlot]);
    CHECK(gIndexLakituList[0]==netSlot+10);CHECK(D_8018CF14==&cameras[0]);
    /* Deliberately contaminate each legacy structure to exercise isolation. */
    memset(gPlayers,0x42,sizeof(gPlayers));memset(gObjectList,0x37,sizeof(gObjectList));
    memset(gActorList,0x24,sizeof(gActorList));memset(gBombKarts,0x57,sizeof(gBombKarts));gRandomSeed16=13;
}
static void lobby_tests(void){
    int n,i,j,k;Race8Lobby s;unsigned short buttons[8]={0};
    for(n=4;n<=8;++n){
        race8_lobby_init(&s,n);memset(buttons,0,sizeof(buttons));
        buttons[1]=0x1000;CHECK(!race8_lobby_step(&s,buttons));
        for(i=0;i<n;++i)buttons[i]=0x8000;
        CHECK(!race8_lobby_step(&s,buttons));
        buttons[0]=0x1000;CHECK(race8_lobby_step(&s,buttons));
        CHECK(!race8_lobby_step(&s,buttons)); /* Held Start cannot retrigger. */
        buttons[0]=0x0400;CHECK(!race8_lobby_step(&s,buttons));
        for(i=0;i<n;++i)CHECK(!s.ready[i]);
        /* Hundreds of character swaps preserve a unique roster and never alter a ready slot. */
        memset(buttons,0,sizeof(buttons));
        for(k=0;k<300;++k){
            race8_lobby_step(&s,buttons);i=k%n;buttons[i]=0x0100;race8_lobby_step(&s,buttons);buttons[i]=0;
            for(i=0;i<n;++i){CHECK(s.character[i]>=0&&s.character[i]<8);for(j=i+1;j<n;++j)CHECK(s.character[i]!=s.character[j]);}
        }
        for(k=0;k<60;++k){buttons[0]=0;race8_lobby_step(&s,buttons);buttons[0]=0x0410;race8_lobby_step(&s,buttons);CHECK(s.course>=0&&s.course<20);CHECK(s.cc>=0&&s.cc<3);}
    }
}
static void finish_permutations(int *order,int depth,int n,int mask){
    int i,b;Race8Results s;
    if(depth<n){for(i=0;i<n;++i)if(!(mask&(1<<i))){order[depth]=i;finish_permutations(order,depth+1,n,mask|(1<<i));}return;}
    for(b=0;b<2;++b){
        race8_results_init(&s,n);CHECK(!race8_finish(&s,-1,b));CHECK(!race8_finish(&s,n,b));
        for(i=0;i<n-1;++i){CHECK(race8_finish(&s,order[i],b));CHECK(!race8_finish(&s,order[i],b));CHECK(s.place[order[i]]==(b?n-1-i:i));}
        race8_complete_last(&s,b);CHECK(s.done);CHECK(s.count==n);CHECK(s.place[order[n-1]]==(b?0:n-1));
        for(i=0;i<n;++i)CHECK(s.order[s.place[i]]==i);
    }
}
static void gameplay_tests(void){
    int n,c,i,j,slot;char text[128];unsigned int before;
    for(n=4;n<=8;++n){netCount=n;
        for(c=0;c<20;++c){course=c;x360_race8_prepare();x360_race8_spawn();
            CHECK(gPlayerCount==n&&gPlayerCountSelection1==n&&gScreenModeSelection==SCREEN_MODE_1P);
            CHECK(gCurrentCourseId==c&&gGamestateNext==RACING);
            for(i=0;i<8;++i){CHECK(gPlayers[i].characterId==i);CHECK(!!(gPlayers[i].type&PLAYER_EXISTS)==(i<n));
                if(i<n)CHECK((gPlayers[i].type&PLAYER_HUMAN)!=0);
                for(j=0;j<i;++j)CHECK(gPlayers[i].pos[0]!=gPlayers[j].pos[0]||gPlayers[i].pos[2]!=gPlayers[j].pos[2]);
            }
        }
        cameraCalls=0;x360_race8_cameras();CHECK(cameraCalls==n);
        for(slot=0;slot<n;++slot){
            netSlot=slot;course=0;x360_race8_prepare();x360_race8_spawn();
            for(i=0;i<8;++i){cameras[i].playerId=(s16)i;gIndexLakituList[i]=i+10;}
            gRandomSeed16=4321;viewport.player=&gPlayers[0];before=x360_race8_hash(123);
            memset(gObjectList,0,sizeof(gObjectList));memset(gActorList,0,sizeof(gActorList));memset(gBombKarts,0,sizeof(gBombKarts));
            x360_race8_render();CHECK(x360_race8_hash(123)==before);CHECK(gRandomSeed16==4321);CHECK(cameras[0].playerId==0);
            CHECK(gObjectList[0].status==0);CHECK(gActorList[0].type==0);CHECK(gBombKarts[0].state==0);
            gRaceState=RACE_IN_PROGRESS;memset(gControllers,0,sizeof(gControllers));gControllers[slot].buttonPressed=START_BUTTON;
            x360_race8_controls();CHECK(gIsGamePaused==slot+1);x360_race8_controls();CHECK(gIsGamePaused==0);
            CHECK(x360_race8_hud_line(0,text,sizeof(text)));CHECK(strstr(text,"PLAYERS")!=0);
        }
        memset(gControllers,0,sizeof(gControllers));memset(gLapCountByPlayerId,0,sizeof(gLapCountByPlayerId));
        memset(finishCalls,0,sizeof(finishCalls));memset(lapCalls,0,sizeof(lapCalls));
        course=0;x360_race8_prepare();x360_race8_spawn();gRaceState=RACE_STAGING;CHECK(!x360_race8_rules());gRaceState=RACE_IN_PROGRESS;
        for(i=0;i<n;++i)gLapCountByPlayerId[i]=2;
        CHECK(x360_race8_rules());CHECK(x360_race8_rules());for(i=0;i<n;++i)CHECK(lapCalls[i]==1);
        for(i=0;i<n-1;++i)gLapCountByPlayerId[i]=3;
        CHECK(x360_race8_rules());CHECK(results.done);CHECK(gRaceState==RACE_DONE);CHECK(gIsGamePaused==1);CHECK(results.order[0]==n-2);
        for(i=0;i<n;++i)CHECK(finishCalls[i]==1);
        gControllers[0].buttonPressed=A_BUTTON;x360_race8_controls();CHECK(results.done);
        frame+=61;x360_race8_controls();CHECK(!results.done);CHECK(!gIsGamePaused);
        course=15;x360_race8_prepare();x360_race8_spawn();gRaceState=RACE_IN_PROGRESS;
        for(i=0;i<n;++i)gPlayerBalloonCount[i]=2;
        for(i=0;i<n-1;++i){gPlayerBalloonCount[i]=-1;x360_race8_rules();}
        CHECK(results.done);CHECK(results.order[0]==n-1);CHECK(!(gPlayers[0].type&PLAYER_EXISTS));
        frame+=61;gControllers[0].buttonPressed=B_BUTTON;x360_race8_controls();CHECK(!results.done);
    }
    CHECK(configured==5);CHECK(setupCount==10);
}
int main(void){int n,order[8];lobby_tests();for(n=4;n<=8;++n)finish_permutations(order,0,n,0);gameplay_tests();printf("PASS race8 gameplay/lobby/results: %d checks\n",checks);return 0;}
