#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include <objects.h>
#include <course.h>
#include "racing/skybox_and_splitscreen.h"
#include <assets/common_data.h>
#include "main.h"
#include "camera.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "update_objects.h"
#include "render_objects.h"
#include "data/some_data.h"
#include "xbox360/netplay.h"

static int checks,online,extended,locals=1,slot;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL %d: %s\n",__LINE__,#x);exit(1);}}while(0)
int x360_net_active(void){return online;}
int x360_net8_active(void){return extended;}
int x360_net_local_count(void){return locals;}
int x360_net_local_slot(void){return slot;}
s32 gGamestate=RACING;
s16 gCurrentCourseId;
s8 gHUDDisable,D_801657C8,D_8018D230;
s32 D_8018D1F0;
s16 D_8018D218;
s16 D_8018D208,D_8018D210;
f32 D_8018D1E8,gCameraZoom[8];
u8 D_8018D228;
Object gObjectList[OBJECT_LIST_SIZE];
s32 D_8018CC80[D_8018CC80_SIZE];
static u8 textures[4][1024];
u8 (*D_8018D220)[1024]=textures;
static Camera cams[4];
static struct UnkStruct_800DC5EC views[4];
struct UnkStruct_800DC5EC *D_800DC5EC=&views[0],*D_800DC5F0=&views[1],*D_800DC5F4=&views[2],*D_800DC5F8=&views[3];
Gfx D_0D0076F8[1],D_0D007A60[1],common_rectangle_display[1];
Vtx D_0D005FB0[4];
static Gfx commands[4096];
Gfx *gDisplayListHead=commands;
static f32 projectionHeight=240;
typedef struct {s32 x,y;f32 scale;u8 *texture;} Draw;
static Draw draws[64];
static int drawCount,legacyCalls;
static u8 *texture;
void guOrtho(Mtx *m,f32 l,f32 r,f32 b,f32 t,f32 n,f32 f,f32 s){
 CHECK(l==0&&r==320&&t==0&&n==-1&&f==1&&s==1);projectionHeight=b;
}
void set_matrix_hud_screen(void){projectionHeight=240;}
void func_80044DA0(u8 *p,s32 w,s32 h){CHECK(w==64&&h==32);texture=p;}
void func_80042330(s32 x,s32 y,u16 a,f32 s){
 CHECK(drawCount<64&&a==0);draws[drawCount].x=x;draws[drawCount].y=y;
 draws[drawCount].scale=s;draws[drawCount++].texture=texture;
}
void func_8004B6C4(s32 r,s32 g,s32 b){CHECK(r==255&&g==255&&b==255);}
void func_8004B414(s32 r,s32 g,s32 b,s32 a){CHECK(0);}
void func_800519D4(s32 index,s16 x,s16 y){CHECK(0);}
void func_80051EF8(void){++legacyCalls;}
void func_80051F9C(void){++legacyCalls;}
void func_80052044(void){++legacyCalls;}
void func_80052080(void){++legacyCalls;}
void init_object(s32 i,s32 state){memset(&gObjectList[i],0,sizeof(Object));gObjectList[i].state=(s16)state;}
void func_80073404(s32 i,u8 w,u8 h,Vtx *v){gObjectList[i].textureWidth=w;gObjectList[i].textureHeight=h;gObjectList[i].vertex=v;}
void set_object_flag_status_true(s32 i,s32 mask){gObjectList[i].status|=mask;}
void set_object_flag_status_false(s32 i,s32 mask){gObjectList[i].status&=~mask;}
#include "build/cloud_production.inc"

int main(void){
 int course,i,yaw,zoom,view,kind,split,k,count;Draw expected[64];Object saved[OBJECT_LIST_SIZE];
 const int courses[]={COURSE_MARIO_RACEWAY,COURSE_LUIGI_RACEWAY,COURSE_YOSHI_VALLEY,COURSE_MOO_MOO_FARM,COURSE_KOOPA_BEACH,COURSE_ROYAL_RACEWAY,COURSE_KALAMARI_DESERT,COURSE_SHERBET_LAND};
 CloudData *lists[]={gKalimariDesertClouds,gLuigiRacewayClouds,gYoshiValleyMooMooFarmClouds,gYoshiValleyMooMooFarmClouds,gKoopaTroopaBeachClouds,gRoyalRacewayClouds,gKalimariDesertClouds,gSherbetLandClouds};
 for(i=0;i<4;++i)views[i].camera=&cams[i];
 for(course=0;course<8;++course){
  CloudData *angles=course==0?gLuigiRacewayClouds:lists[course];
  gCurrentCourseId=(s16)courses[course];
  for(D_8018D1F0=0;lists[course][D_8018D1F0].rotY!=0xFFFF;++D_8018D1F0){
   D_8018CC80[D_8018D1F0]=D_8018D1F0;init_cloud_object(D_8018D1F0,1,&lists[course][D_8018D1F0]);
  }
  for(yaw=0;yaw<65536;yaw+=2048)for(zoom=30;zoom<=70;zoom+=7){
   s16 field=(s16)(zoom+40.75f);
   D_8018D208=(s16)((field/2)*DEGREES(1)+DEGREES(10));D_8018D210=-D_8018D208;
   D_8018D1E8=1.7578125/field;D_8018D218=160;
   cams[0].rot[1]=(s16)yaw;views[0].cameraHeight=(s16)(100+yaw%53);
   update_clouds(0,&cams[0],angles);
   online=0;drawCount=0;gDisplayListHead=commands;func_80057FC4(0);
   count=drawCount;memcpy(expected,draws,sizeof(expected));CHECK(projectionHeight==240);
   memcpy(saved,gObjectList,sizeof(saved));
   for(kind=0;kind<2;++kind)for(split=0;split<2;++split){
    if(split&&!kind)continue;
    for(view=0;view<(kind?(split?2:1):4);++view){
     online=1;extended=kind;locals=split+1;slot=kind?7-split:view;
     /* Make every other camera wrong to catch host-camera reuse on guests. */
     for(i=0;i<4;++i){cams[i].rot[1]=(s16)(yaw+32768);views[i].cameraHeight=66;gCameraZoom[i]=99;}
     cams[view].rot[1]=(s16)yaw;views[view].cameraHeight=(s16)(100+yaw%53);gCameraZoom[view]=(f32)zoom+0.75f;
     drawCount=0;gDisplayListHead=commands;
     func_80057FC4(kind?(split?(view?4:3):0):8+view);
     CHECK(drawCount==count);CHECK(projectionHeight==(split?120:240));
     CHECK(!memcmp(saved,gObjectList,sizeof(saved)));
     for(k=0;k<count;++k){
      CHECK(draws[k].x==expected[k].x);CHECK(draws[k].y==(split?expected[k].y/2:expected[k].y));
      CHECK(fabs(draws[k].scale-expected[k].scale)<0.00001);CHECK(draws[k].texture==expected[k].texture);
     }
    }
   }
  }
 }
 gCurrentCourseId=COURSE_LUIGI_RACEWAY;extended=0;locals=1;online=1;
 for(i=1;i<=4;++i){slot=(i==2||i==4);drawCount=0;gDisplayListHead=commands;func_80057FC4(i);CHECK(projectionHeight==240);}
 slot=1;drawCount=0;gDisplayListHead=commands;func_80057FC4(1);CHECK(drawCount==0);
 gHUDDisable=1;func_80057FC4(2);CHECK(drawCount==0);gHUDDisable=0;
 D_801657C8=1;func_80057FC4(2);CHECK(drawCount==0);D_801657C8=0;
 gCurrentCourseId=COURSE_TOADS_TURNPIKE;legacyCalls=0;func_80057FC4(2);CHECK(legacyCalls==1);
 online=0;for(i=1;i<=4;++i)func_80057FC4(i);CHECK(legacyCalls==5);
 printf("PASS online/offline cloud equivalence: %d checks\n",checks);return 0;
}
