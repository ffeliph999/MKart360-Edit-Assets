#include <stdio.h>
#include <stdlib.h>
#include "xbox360/netplay_view.h"
#include "xbox360/online_hud.h"
int x360_gfx_online_hud=0;
static int active=1,extended=0,count=4,slot=0;
static int gGamestate=4,gActiveScreenMode=3,gPlayerCountSelection1=4;
static int checks,flushes,flushedPhase=-1;
#define G_NOOP 0xC0
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
int x360_net_active(){return active;}
int x360_net8_active(){return extended;}
int x360_net_player_count(){return count;}
int x360_net_local_slot(){return slot;}
void gfx_flush(){++flushes;flushedPhase=x360_gfx_online_hud;}
#include "build/hud_bridge_production.inc"
int main(){
    for(count=2;count<=8;++count)for(slot=0;slot<count;++slot)
    for(gActiveScreenMode=0;gActiveScreenMode<4;++gActiveScreenMode)
    for(extended=0;extended<2;++extended){
        mkview::Rect crop;
        gPlayerCountSelection1=count;
        bool expected=!extended&&mkview::crop(gActiveScreenMode,count,slot,crop);
        CHECK(rectangle_crop()==expected);CHECK(triangle_crop()==expected);
        flushes=0;marker(X360_ONLINE_HUD_BEGIN);
        CHECK(flushes==1&&flushedPhase==0&&x360_gfx_online_hud==1);
        CHECK(!rectangle_crop());CHECK(!triangle_crop());
        marker(0);CHECK(flushes==1&&x360_gfx_online_hud==1);
        marker(X360_ONLINE_HUD_END);
        CHECK(flushes==2&&flushedPhase==1&&x360_gfx_online_hud==0);
        CHECK(rectangle_crop()==expected);CHECK(triangle_crop()==expected);
    }
    count=4;slot=0;extended=0;gActiveScreenMode=3;gPlayerCountSelection1=4;
    active=0;CHECK(!rectangle_crop()&&!triangle_crop());active=1;
    gGamestate=0;CHECK(!rectangle_crop()&&!triangle_crop());gGamestate=4;
    gPlayerCountSelection1=1;CHECK(!rectangle_crop()&&!triangle_crop());
    printf("PASS online HUD graphics phases: %d checks\n",checks);
    return 0;
}
