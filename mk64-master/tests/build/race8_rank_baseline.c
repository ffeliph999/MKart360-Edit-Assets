#include <stdlib.h>
#include "race8_rank_baseline.inc"

static int checks,netCount=8,netSlot,course,cc=1,configured,setupCount,cameraCalls,finishCalls[8],lapCalls[8];
static unsigned int frame=100;
static int netLocals=1,videoBlack=1,draws[2],clears,online=1,extended=1;
static int itemCalls[8],timerCalls[8],lapDraws[8],rankCalls[8],mapCalls;
static int drawnRank[8],initPlayerCalls;
Player *gPlayerOne=gPlayers;
s16 D_80165270[8];
u16 gNearestPathPointByPlayerId[8],gSelectedPathCount;
static int legacyHudCalls;
static float orthoRight,orthoBottom;
static Gfx testCommands[2048];
Gfx *gDisplayListHead=testCommands;
hud_player playerHUD[8];
s8 gHUDDisable,D_80165800[8];
s32 gIsHUDVisible=1;
u16 D_800DC5B8=1;
s32 D_8018D21C,D_8018D22C;
void osViBlack(u8 value){videoBlack=value;}
#define CHECK(x) do {++checks;if(!(x)){printf("FAIL line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
Player gPlayers[8];
struct Controller gControllers[9];
Camera cameras[8];
Camera *D_8018CF14=&cameras[0];
Object gObjectList[OBJECT_LIST_SIZE];
struct Actor gActorList[ACTOR_LIST_SIZE];
BombKart gBombKarts[NUM_BOMB_KARTS_MAX];
struct UnkStruct_800DC5EC viewport,viewport2;
struct UnkStruct_800DC5EC *D_800DC5F0=&viewport2;
struct UnkStruct_800DC5EC *D_800DC5EC=&viewport;
f32 gCameraZoom[8],gCourseTimer;
u16 gRandomSeed16,gIsGamePaused,gDemoMode,gEnableDebugMode;
s8 gPlayerCount,gDemoUseController,gCharacterSelections[4];
s32 gRaceState,gPlayerCountSelection1,gScreenModeSelection,gActiveScreenMode,gCCSelection,gModeSelection,gIsMirrorMode,gGamestateNext,gGamestate=RACING;
s32 gLapCountByPlayerId[10],D_80164A28,gPlayerWinningIndex,gIndexLakituList[8];
s16 gCurrentCourseId,gPlayerBalloonCount[8];
s16 D_8016348C,gGPCurrentRacePlayerIdByRank[12],gPrevPlayerIdByRank[12],gPlayerPositionLUT[8];
s32 gGPCurrentRaceRankByPlayerId[10],gGPCurrentRaceRankByPlayerIdDup[10],gPreviousGPCurrentRaceRankByPlayerId[10];
f32 gCourseCompletionPercentByRank[8],gCourseCompletionPercentByPlayerId[10],gTimePlayerLastTouchedFinishLine[8];
s32 D_8018D1F0,D_8018CC80[D_8018CC80_SIZE];
TrackPathPoint track[2];
TrackPathPoint *gTrackPaths[4]={track,track,track,track};
int x360_net8_active(void){return online && extended;}
int x360_net_active(void){return online;}
int x360_net_player_count(void){return netCount;}
int x360_net_local_slot(void){return netSlot;}
int x360_net_local_count(void){return netLocals;}
void course_update_clouds(s32 view){
    CHECK(view==0||view==3||view==4);
}
void func_80058BF4(void){}
void guOrtho(Mtx *m,f32 l,f32 r,f32 b,f32 t,f32 near,f32 far,f32 scale){
    CHECK(l==0&&t==0&&near==-1&&far==1&&scale==1);
    orthoRight=r;orthoBottom=b;
}
void draw_item_window(s32 slot){++itemCalls[slot];}
void render_hud_timer(s32 slot){CHECK(gModeSelection==GRAND_PRIX);++timerCalls[slot];}
void draw_simplified_lap_count(s32 slot){++lapDraws[slot];}
void func_8004EE54(s32 view){CHECK(view==0);++mapCalls;}
void render_mini_map_finish_line(s32 view){CHECK(view==0);}
void func_8004F3E4(s32 view){CHECK(view==0);}
/* These are called only by the stock dispatcher, which online must bypass. */
void func_80058F78(void){++legacyHudCalls;}
void render_hud_2p_horizontal_player_two_horizontal_player_one(void){++legacyHudCalls;}
void render_hud_2p_horizontal_player_two(void){++legacyHudCalls;}
void render_hud_2p_vertical_player_one(void){++legacyHudCalls;}
void render_hud_2p_vertical_player_two(void){++legacyHudCalls;}
void render_hud_1p_multi(void){++legacyHudCalls;}
void render_hud_2p_multi(void){++legacyHudCalls;}
void render_hud_3p_multi(void){++legacyHudCalls;}
void render_hud_4p_multi(void){++legacyHudCalls;}
Gfx D_0D0076F8[1];
s32 D_800E55F8[8];
u8 common_texture_hud_place[8][4096];
Vtx D_0D0068F0[8];
void func_8004A384(s32 x,s32 y,u16 angle,f32 scale,s32 red,s32 green,s32 blue,s32 alpha,
                  u8 *texture,Vtx *vertices,s32 w,s32 h,s32 tw,s32 th){
    int slot,rank=-1;
    for(slot=0;slot<8;++slot)if(texture==common_texture_hud_place[slot])rank=slot;
    CHECK(rank>=0);CHECK(vertices==D_0D0068F0);CHECK(w==128&&h==64&&tw==128&&th==64);
    CHECK(orthoRight==320&&orthoBottom==240);
    CHECK(x-64*scale>=0&&x+64*scale<=orthoRight);
    CHECK(y-32*scale>=0&&y+32*scale<=orthoBottom);
    for(slot=netSlot;slot<netSlot+netLocals;++slot)if(playerHUD[slot].rankY==y)break;
    CHECK(slot<netSlot+netLocals);
    if(rank!=gPlayers[slot].currentRank)
        printf("Rank mismatch: humans=%d slot=%d mode=%d displayed=%d race=%d\n",
               netCount,slot,gModeSelection,rank+1,gPlayers[slot].currentRank+1);
    CHECK(rank==gPlayers[slot].currentRank);
    drawnRank[slot]=rank;
    ++rankCalls[slot];
}
/* Extracted verbatim from production by prepare_hud_tests.py. */
#include "hud_production.inc"
void move_segment_table_to_dmem(void){}
void init_rdp(void){}
void init_z_buffer(void){++clears;}
void select_framebuffer(void){}
unsigned int x360_net_frame(void){return frame;}
int x360_net8_course(void){return course;}
int x360_net8_cc(void){return cc;}
int x360_net8_character(int slot){return slot;}
void x360_net8_configure(void){++configured;}
void func_800C9F90(u8 paused){CHECK(paused==0||paused==1);}
void func_800CA118(u8 slot){CHECK(slot<netCount);++finishCalls[slot];}
void func_800CA49C(u8 slot){CHECK(slot<netCount);++lapCalls[slot];}
f32 func_8028EE8C(s32 slot){return 10.0f-slot*0.01f;}
void func_8000EEDC(void){}
void init_course_path_point(void){}
void init_players(void){++initPlayerCalls;initialize_rank_fixture();}
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
    CHECK(cameras[0].playerId==netSlot);CHECK(D_800DC5EC->player==&gPlayers[netSlot]);
    CHECK(D_800DC5EC->screenWidth==320 && D_800DC5EC->screenHeight==240);
    CHECK(gIndexLakituList[0]==netSlot+10);CHECK(D_8018CF14==&cameras[0]);
    /* Deliberately contaminate each legacy structure to exercise isolation. */
    memset(gPlayers,0x42,sizeof(gPlayers));memset(gObjectList,0x37,sizeof(gObjectList));
    memset(gActorList,0x24,sizeof(gActorList));memset(gBombKarts,0x57,sizeof(gBombKarts));gRandomSeed16=13;
}
static void draw_split(int view){
    struct UnkStruct_800DC5EC *v=view?D_800DC5F0:D_800DC5EC;
    CHECK(netLocals==2);CHECK(gActiveScreenMode==SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL);
    CHECK(cameras[view].playerId==netSlot+view);CHECK(D_8018CF14==&cameras[view]);
    CHECK(v->player==&gPlayers[netSlot+view]);CHECK(v->controllers==&gControllers[netSlot+view]);
    CHECK(v->camera==&cameras[view]);CHECK(v->screenWidth==320&&v->screenHeight==120);
    CHECK(v->screenStartY==60+120*view);CHECK(gIndexLakituList[view]==netSlot+view+10);
    ++draws[view];memset(gPlayers,0x42,sizeof(gPlayers));gRandomSeed16=13;
}
void render_player_one_2p_screen_horizontal(void){draw_split(0);}
void render_player_two_2p_screen_horizontal(void){draw_split(1);}
static void split_tests(void){
    int n,slot,i;char text[128];
    for(n=3;n<=8;++n)for(slot=1;slot+1<n;++slot){
        unsigned int before;Camera saved[8];
        netCount=n;netSlot=slot;netLocals=2;videoBlack=1;
        course=0;x360_race8_prepare();CHECK(videoBlack==0);x360_race8_spawn();
        for(i=0;i<8;++i){cameras[i].playerId=(s16)i;gIndexLakituList[i]=10+i;}
        memcpy(saved,cameras,sizeof(saved));before=x360_race8_hash(123);clears=draws[0]=draws[1]=0;
        x360_race8_render();CHECK(clears==1);CHECK(draws[0]==1&&draws[1]==1);
        CHECK(gActiveScreenMode==SCREEN_MODE_1P&&gScreenModeSelection==SCREEN_MODE_1P);
        CHECK(D_800DC5EC==&viewport&&D_800DC5F0==&viewport2);
        CHECK(!memcmp(cameras,saved,sizeof(saved)));CHECK(x360_race8_hash(123)==before);
        for(i=0;i<2;++i){CHECK(!x360_race8_hud_line_for_view(i,0,text,sizeof(text)));}
        gIsGamePaused=1;
        CHECK(x360_race8_hud_line_for_view(1,0,text,sizeof(text)));
        CHECK(strstr(text,"PAUSED")!=0);gIsGamePaused=0;
    }
    netLocals=1;
    /* Each sky uses its camera and separate object list, without changing time. */
    D_8018D1F0=1;D_8018CC80[0]=3;D_8018CC80[1]=4;
    memset(gObjectList,0,sizeof(gObjectList));gCurrentCourseId=COURSE_LUIGI_RACEWAY;
    cameras[0].rot[1]=0;cameras[1].rot[1]=DEGREES(180);gCameraZoom[0]=gCameraZoom[1]=40;
    race8_project_sky(0);race8_project_sky(1);
    CHECK(gObjectList[3].unk_09C==160);CHECK(gObjectList[3].status&0x10);CHECK(!(gObjectList[4].status&0x10));
    D_8018D1F0=0;
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
        for(c=0;c<20;++c){course=c;videoBlack=1;x360_race8_prepare();CHECK(videoBlack==0);x360_race8_spawn();
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
            x360_race8_render();CHECK(viewport.player==&gPlayers[0]);CHECK(x360_race8_hash(123)==before);CHECK(gRandomSeed16==4321);CHECK(cameras[0].playerId==0);
            CHECK(gObjectList[0].status==0);CHECK(gActorList[0].type==0);CHECK(gBombKarts[0].state==0);
            gRaceState=RACE_IN_PROGRESS;memset(gControllers,0,sizeof(gControllers));gControllers[slot].buttonPressed=START_BUTTON;
            x360_race8_controls();CHECK(gIsGamePaused==slot+1);x360_race8_controls();CHECK(gIsGamePaused==0);
            CHECK(!x360_race8_hud_line(0,text,sizeof(text)));
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
static void hud_tests(void){
    int n,slot,mode,i,split,kind;hud_player saved[8];char text[128];

    for(kind=0;kind<2;++kind)for(n=2;n<=8;++n){
        if(!kind&&n>4)continue;
        extended=kind;netCount=n;course=0;x360_race8_prepare();
        for(slot=0;slot<n;++slot)for(split=0;split<2;++split){
            if(split&&(!kind||slot+1>=n))continue;
            netSlot=slot;netLocals=split+1;
            for(mode=0;mode<4;++mode){
                gActiveScreenMode=mode;gScreenModeSelection=mode;
                viewport.screenWidth=160;viewport.screenHeight=120;
                viewport.screenStartX=80;viewport.screenStartY=60;
                for(i=0;i<8;++i){
                    memset(&playerHUD[i],0x35,sizeof(playerHUD[i]));
                    playerHUD[i].unk_81=0;gPlayers[i].currentRank=(s16)((i+3)%n);
                    D_80165800[i]=1;
                }
                memcpy(saved,playerHUD,sizeof(saved));
                memset(itemCalls,0,sizeof(itemCalls));memset(timerCalls,0,sizeof(timerCalls));
                memset(lapDraws,0,sizeof(lapDraws));memset(rankCalls,0,sizeof(rankCalls));
                mapCalls=0;gDisplayListHead=testCommands;
                x360_race8_render_hud();
                CHECK(testCommands[0].words.w1==X360_ONLINE_HUD_BEGIN);
                CHECK(gDisplayListHead[-1].words.w1==X360_ONLINE_HUD_END);
                CHECK((testCommands[0].words.w0>>24)==G_NOOP);
                CHECK((testCommands[2].words.w0>>24)==G_SETSCISSOR);
                CHECK((testCommands[2].words.w1&0xFFF)==240*4);
                CHECK(((testCommands[2].words.w1>>12)&0xFFF)==320*4);
                CHECK(gModeSelection==VERSUS&&gActiveScreenMode==mode&&gScreenModeSelection==mode);
                CHECK(viewport.screenWidth==160&&viewport.screenHeight==120);
                CHECK(!memcmp(saved,playerHUD,sizeof(saved)));CHECK(mapCalls==!split);
                for(i=0;i<8;++i){int visible=i>=slot&&i<slot+netLocals;
                    CHECK(itemCalls[i]==visible);CHECK(timerCalls[i]==visible);
                    CHECK(lapDraws[i]==visible);CHECK(rankCalls[i]==visible);
                }
                /* Simulate every legacy HUD entry before the new pass. */
                legacyHudCalls=0;for(i=0;i<=RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR;++i)render_hud(i);
                CHECK(legacyHudCalls==0);
            }
        }
    }
    extended=1;netCount=8;netSlot=7;netLocals=1;
    gHUDDisable=1;gDisplayListHead=testCommands;x360_race8_render_hud();CHECK(gDisplayListHead==testCommands);gHUDDisable=0;
    gIsHUDVisible=0;x360_race8_render_hud();CHECK(gDisplayListHead==testCommands);gIsHUDVisible=1;
    gModeSelection=BATTLE;memset(rankCalls,0,sizeof(rankCalls));memset(lapDraws,0,sizeof(lapDraws));memset(timerCalls,0,sizeof(timerCalls));
    x360_race8_render_hud();CHECK(rankCalls[7]==0&&lapDraws[7]==0&&timerCalls[7]==0);
    gIsGamePaused=0;gPlayerBalloonCount[7]=1;
    CHECK(x360_race8_hud_line_for_view(0,0,text,sizeof(text)));CHECK(!strcmp(text,"BALLOONS 2"));
    gModeSelection=VERSUS;gCurrentCourseId=COURSE_YOSHI_VALLEY;x360_race8_render_hud();CHECK(rankCalls[7]==0);
    CHECK(!x360_race8_hud_line(0,0,0));CHECK(!x360_race8_hud_line_for_view(0,0,text,0));
    CHECK(!x360_race8_hud_line_for_view(2,0,text,sizeof(text)));
    online=0;gDemoMode=0;gPlayerCountSelection1=4;legacyHudCalls=0;
    for(i=0;i<=RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR;++i)render_hud(i);CHECK(legacyHudCalls==9);
    gDisplayListHead=testCommands;x360_race8_render_hud();CHECK(gDisplayListHead==testCommands);
    online=1;gActiveScreenMode=SCREEN_MODE_1P;
}
/* Feed race progress through the real sorter, position update, and online HUD.
 * Human count is deliberately independent of GP's eight-racer field. */
static void online_spawn_rank_tests(void){
    int n,i,pass,slot,lead,rank,initBefore;
    struct ShellActor shell;
    online=extended=1;gCurrentCourseId=COURSE_LUIGI_RACEWAY;course=gCurrentCourseId;
    gSelectedPathCount=1000;
    for(n=3;n<=8;++n)for(pass=0;pass<2;++pass){
        netCount=n;netSlot=0;netLocals=2;x360_race8_prepare();
        /* Cold boot has zero-filled tables; restart can inherit arbitrary GP
         * order and finish counters. Neither may leak into the new VS race. */
        for(i=0;i<8;++i){
            gGPCurrentRaceRankByPlayerId[i]=pass?7-i:0;
            gGPCurrentRacePlayerIdByRank[i]=(s16)(pass?7-i:0);
            gPrevPlayerIdByRank[i]=(s16)(pass?7-i:0);
            gGPCurrentRaceRankByPlayerIdDup[i]=pass?7-i:0;
            gLapCountByPlayerId[i]=3;gPlayerPositionLUT[i]=0;
        }
        initBefore=initPlayerCalls;x360_race8_spawn();
        CHECK(initPlayerCalls==initBefore+1);
        for(i=0;i<n;++i){
            CHECK(gGPCurrentRaceRankByPlayerId[i]==i);
            CHECK(gGPCurrentRacePlayerIdByRank[i]==i);
            CHECK(gPrevPlayerIdByRank[i]==i);
            CHECK(gPlayers[i].currentRank==i&&gPlayerPositionLUT[i]==i);
            CHECK(gLapCountByPlayerId[i]==-1);
        }
        /* Everyone takes the lead; each console view uses the resulting slot
         * rank, while the actual red-shell acquisition branch targets ahead. */
        for(lead=0;lead<n;++lead){
            for(rank=0;rank<n;++rank){
                i=(lead+rank)%n;
                gCourseCompletionPercentByPlayerId[i]=100.0f-rank;
            }
            set_places();update_race_position_data();
            for(rank=0;rank<n;++rank){
                i=(lead+rank)%n;
                CHECK(gPlayers[i].currentRank==rank);CHECK(gPlayerPositionLUT[rank]==i);
                memset(&shell,0,sizeof(shell));shell.playerId=(s16)i;
                acquire_red_shell_target(&shell);
                if(rank==0){CHECK(red_shell_target_ahead((s16)i)==-1);CHECK(shell.state==TRIPLE_GREEN_SHELL&&shell.someTimer==600);}
                else{CHECK(shell.targetPlayer==(lead+rank-1)%n);CHECK(shell.targetPlayer!=i);
                     CHECK(shell.state==(rank>=5?GREEN_SHELL_HIT_A_RACER:RED_SHELL_LOCK_ON));}
            }
            /* Host top/bottom, then every possible guest slot/full or split. */
            for(slot=0;slot<n;++slot){
                netSlot=slot;netLocals=slot+1<n?2:1;gDisplayListHead=testCommands;
                x360_race8_render_hud();CHECK(drawnRank[slot]==gPlayers[slot].currentRank);
                if(netLocals==2)CHECK(drawnRank[slot+1]==gPlayers[slot+1].currentRank);
            }
        }
    }
    /* Corrupt lookup must never steer a shell back to its owner or off-array. */
    gPlayers[0].currentRank=1;gPlayerPositionLUT[0]=0;
    memset(&shell,0,sizeof(shell));shell.playerId=0;acquire_red_shell_target(&shell);
    CHECK(red_shell_target_ahead(0)==-1&&shell.state==TRIPLE_GREEN_SHELL&&shell.someTimer==600);
    gPlayerPositionLUT[0]=8;CHECK(red_shell_target_ahead(0)==-1);
    gPlayerPositionLUT[0]=-1;CHECK(red_shell_target_ahead(0)==-1);
    gPlayerPositionLUT[0]=1;gPlayers[1].type=0;CHECK(red_shell_target_ahead(0)==-1);
    gPlayers[1].type=PLAYER_EXISTS;gPlayers[1].currentRank=1;CHECK(red_shell_target_ahead(0)==-1);
    gPlayers[0].currentRank=8;CHECK(red_shell_target_ahead(0)==-1);
    gPlayers[0].currentRank=-1;CHECK(red_shell_target_ahead(0)==-1);
    CHECK(red_shell_target_ahead(-1)==-1&&red_shell_target_ahead(8)==-1);
    course=COURSE_BLOCK_FORT;x360_race8_prepare();initBefore=initPlayerCalls;x360_race8_spawn();
    CHECK(initPlayerCalls==initBefore); /* Battle has no course/rank initialization. */
}
static void cpu_rank_tests(void){
    struct ShellActor shell;
    int humans,slot,locals,kind,place,i,other,rank,direction;
    for(humans=2;humans<=8;++humans)for(kind=0;kind<2;++kind){
        if(!kind&&humans>4)continue;
        extended=kind;netCount=humans;gPlayerCount=(s8)humans;
        gPlayerCountSelection1=humans;gModeSelection=GRAND_PRIX;
        gCurrentCourseId=COURSE_LUIGI_RACEWAY;D_8016348C=0;
        for(slot=0;slot<humans;++slot)for(locals=1;locals<=2;++locals){
            if(locals==2&&(!kind||slot+1>=humans))continue;
            netSlot=slot;netLocals=locals;
            /* Walk both directions through 1st..8th, overtaking humans/CPUs. */
            for(direction=0;direction<2;++direction)for(place=0;place<8;++place){
                int wanted=direction?7-place:place;
                for(i=0;i<8;++i){
                    gPlayers[i].type=PLAYER_EXISTS|(i<humans?PLAYER_HUMAN:PLAYER_CPU);
                    gGPCurrentRacePlayerIdByRank[i]=(s16)i;
                    gPrevPlayerIdByRank[i]=(s16)i;
                    gGPCurrentRaceRankByPlayerId[i]=i;
                    gCourseCompletionPercentByPlayerId[i]=100.0f-i;
                }
                for(other=0,rank=0;other<8;++other){
                    if(other==slot)continue;
                    if(rank==wanted)++rank;
                    gCourseCompletionPercentByPlayerId[other]=100.0f-rank++;
                }
                gCourseCompletionPercentByPlayerId[slot]=100.0f-wanted;
                set_places();update_race_position_data();
                CHECK(gPlayers[slot].currentRank==wanted);
                for(i=0;i<8;++i)CHECK(gPlayerPositionLUT[gPlayers[i].currentRank]==i);
                /* CPU and human targets share the same complete GP ranking. */
                memset(&shell,0,sizeof(shell));shell.playerId=(s16)slot;
                acquire_red_shell_target(&shell);
                if(wanted==0) CHECK(shell.state==TRIPLE_GREEN_SHELL);
                else {
                    CHECK(shell.targetPlayer==gPlayerPositionLUT[wanted-1]);
                    CHECK(shell.targetPlayer!=slot);
                }
                memset(rankCalls,0,sizeof(rankCalls));
                gDisplayListHead=testCommands;x360_race8_render_hud();
                CHECK(drawnRank[slot]==wanted);CHECK(rankCalls[slot]==1);
                if(locals==2){CHECK(rankCalls[slot+1]==1);CHECK(drawnRank[slot+1]==gPlayers[slot+1].currentRank);}
                CHECK(gModeSelection==GRAND_PRIX&&gPlayerCount==humans);
                /* Rendering must not rewrite gameplay ranks or the finish LUT. */
                for(i=0;i<8;++i)CHECK(gGPCurrentRaceRankByPlayerId[i]==gPlayers[i].currentRank);
            }
        }
    }
}
int main(void){
 int i;
 netCount=3;netLocals=2;netSlot=0;course=COURSE_LUIGI_RACEWAY;
 x360_race8_prepare();x360_race8_spawn();
 for(i=0;i<3;++i)gCourseCompletionPercentByPlayerId[i]=100.0f-i*10;
 set_places();update_race_position_data();
 printf("Old spawn, P1 physically ahead: displayed P1=%d P2=%d P3=%d; original P1 red-shell target slot=%d (owner=0)\n",
 gPlayers[0].currentRank+1,gPlayers[1].currentRank+1,gPlayers[2].currentRank+1,
 gPlayerPositionLUT[gPlayers[0].currentRank-1]);
 CHECK(gPlayers[0].currentRank==0&&gPlayers[1].currentRank==1&&gPlayers[2].currentRank==2);
 return 0;
}
