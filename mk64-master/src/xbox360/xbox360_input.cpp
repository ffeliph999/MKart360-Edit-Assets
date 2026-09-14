#include "xbox360/netplay.h"
#include "xbox360/controller_config.h"
#include <xtl.h>
#include <string.h>
#include "xbox360/platform.h"

/*
 * Fill MK64's OSContPad-compatible array directly.
 *
 * The SM64 Xbox binary we mapped used XamInputGetState(0,1,...).
 * For source code we prefer the documented XInput API exposed by the XDK.
 */
#ifndef A_BUTTON
#define A_BUTTON 0x8000
#define B_BUTTON 0x4000
#define Z_TRIG   0x2000
#define START_BUTTON 0x1000
#define U_JPAD   0x0800
#define D_JPAD   0x0400
#define L_JPAD   0x0200
#define R_JPAD   0x0100
#define L_TRIG   0x0020
#define R_TRIG   0x0010
#define U_CBUTTONS 0x0008
#define D_CBUTTONS 0x0004
#define L_CBUTTONS 0x0002
#define R_CBUTTONS 0x0001
#endif

struct PadCompat {
    unsigned short button;
    signed char stick_x;
    signed char stick_y;
    unsigned char err_no;
};


static mkcontrols::Config controls;
static bool controls_loaded;
static uint32_t controls_generation;
static int controls_slot=-1;
static const char *control_paths[2]={"game:\\mk64-controls-0.cfg","game:\\mk64-controls-1.cfg"};
static bool read_controls(int slot,mkcontrols::Config &c,uint32_t &generation) {
    HANDLE f=CreateFileA(control_paths[slot],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    if(f==INVALID_HANDLE_VALUE)return false;
    uint8_t data[mkcontrols::RECORD_BYTES];DWORD got=0;
    bool ok=GetFileSize(f,0)==sizeof(data)&&ReadFile(f,data,sizeof(data),&got,0)&&got==sizeof(data);CloseHandle(f);
    return ok&&mkcontrols::decode(data,sizeof(data),c,generation);
}
extern "C" void x360_controls_load(void){
    if(controls_loaded)return;controls_loaded=true;mkcontrols::defaults(controls);
    mkcontrols::Config a,b;uint32_t ga=0,gb=0;bool va=read_controls(0,a,ga),vb=read_controls(1,b,gb);
    if(va||vb){controls_slot=vb&&(!va||int32_t(gb-ga)>0)?1:0;controls=controls_slot?b:a;controls_generation=controls_slot?gb:ga;}
}
extern "C" int x360_controls_save(void){
    x360_controls_load();uint8_t data[mkcontrols::RECORD_BYTES];mkcontrols::encode(controls,controls_generation+1,data);
    const int slot=controls_slot==0?1:0;
    HANDLE f=CreateFileA(control_paths[slot],GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,0);
    if(f==INVALID_HANDLE_VALUE)return 0;DWORD written=0;
    bool ok=WriteFile(f,data,sizeof(data),&written,0)&&written==sizeof(data);CloseHandle(f);
    mkcontrols::Config check;uint32_t gen=0;
    ok=ok&&read_controls(slot,check,gen)&&gen==controls_generation+1&&memcmp(&check,&controls,sizeof(check))==0;
    if(ok){controls_slot=slot;controls_generation=gen;}return ok?1:0;
}
extern "C" const char *x360_control_action(int action){return action>=0&&action<mkcontrols::ACTIONS?mkcontrols::actions[action]:"";}
extern "C" const char *x360_control_binding(int player,int action){x360_controls_load();if(player<0||player>=4||action<0||action>=mkcontrols::ACTIONS)return "";unsigned b=controls.player[player].bind[action];return b<mkcontrols::SOURCES?mkcontrols::sources[b]:"UNBOUND";}
extern "C" void x360_control_bind(int player,int action,int source){x360_controls_load();if(player>=0&&player<4&&action>=0&&action<mkcontrols::ACTIONS&&(source>=0&&source<mkcontrols::SOURCES||source==mkcontrols::UNBOUND))controls.player[player].bind[action]=(uint8_t)source;}
extern "C" void x360_control_defaults(int player){x360_controls_load();if(player>=0&&player<4)mkcontrols::defaults(controls.player[player]);}
extern "C" int x360_control_stick(int player,int change){x360_controls_load();if(player<0||player>=4)return 0;if(change)controls.player[player].stick^=1;return controls.player[player].stick;}
extern "C" int x360_control_deadzone(int player,int change){x360_controls_load();if(player<0||player>=4)return 24;int d=controls.player[player].deadzone+change;if(d<5)d=5;if(d>40)d=40;controls.player[player].deadzone=(uint8_t)d;return d;}
static uint32_t physical_down(const XINPUT_GAMEPAD &g){
    static const WORD masks[16]={XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_B,XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_Y,XINPUT_GAMEPAD_LEFT_SHOULDER,XINPUT_GAMEPAD_RIGHT_SHOULDER,0,0,XINPUT_GAMEPAD_BACK,XINPUT_GAMEPAD_START,XINPUT_GAMEPAD_LEFT_THUMB,XINPUT_GAMEPAD_RIGHT_THUMB,XINPUT_GAMEPAD_DPAD_UP,XINPUT_GAMEPAD_DPAD_DOWN,XINPUT_GAMEPAD_DPAD_LEFT,XINPUT_GAMEPAD_DPAD_RIGHT};
    uint32_t down=0;for(int i=0;i<16;++i)if(g.wButtons&masks[i])down|=1U<<i;
    if(g.bLeftTrigger>0x40)down|=1U<<mkcontrols::LT;if(g.bRightTrigger>0x40)down|=1U<<mkcontrols::RT;
    if(g.sThumbRY>16000)down|=1U<<mkcontrols::RUP;if(g.sThumbRY<-16000)down|=1U<<mkcontrols::RDOWN;
    if(g.sThumbRX<-16000)down|=1U<<mkcontrols::RLEFT;if(g.sThumbRX>16000)down|=1U<<mkcontrols::RRIGHT;
    if(g.sThumbLY>16000)down|=1U<<mkcontrols::LUP;if(g.sThumbLY<-16000)down|=1U<<mkcontrols::LDOWN;
    if(g.sThumbLX<-16000)down|=1U<<mkcontrols::LLEFT;if(g.sThumbLX>16000)down|=1U<<mkcontrols::LRIGHT;
    return down;
}
extern "C" unsigned int x360_controls_down(void){XINPUT_STATE s;memset(&s,0,sizeof(s));return XInputGetState(0,&s)==ERROR_SUCCESS?physical_down(s.Gamepad):0;}
static void convert_one(const XINPUT_STATE &s,PadCompat &p,int player){
    memset(&p,0,sizeof(p));const mkcontrols::Profile &profile=controls.player[player];const XINPUT_GAMEPAD &g=s.Gamepad;
    p.button=mkcontrols::translate(profile,physical_down(g));
    p.stick_x=mkcontrols::axis(profile.stick?g.sThumbRX:g.sThumbLX,profile.deadzone);
    p.stick_y=mkcontrols::axis(profile.stick?g.sThumbRY:g.sThumbLY,profile.deadzone);
}

extern "C" void x360_read_controllers(void *pads_, int count) {
    x360_controls_load();
    static bool loggingChord;
    XINPUT_STATE chordState;memset(&chordState,0,sizeof(chordState));XInputGetState(0,&chordState);
    const WORD chord=XINPUT_GAMEPAD_LEFT_THUMB|XINPUT_GAMEPAD_RIGHT_THUMB;
    bool held=(chordState.Gamepad.wButtons&chord)==chord;
    if(held&&!loggingChord)x360_set_logging(!x360_logging_enabled());loggingChord=held;
    PadCompat *pads = (PadCompat*)pads_;
    for (int i = 0; i < count; ++i) {
        memset(&pads[i], 0, sizeof(PadCompat));
        XINPUT_STATE s;
        memset(&s, 0, sizeof(s));
        if (XInputGetState((DWORD)i, &s) == ERROR_SUCCESS) {
            convert_one(s, pads[i],i);
        } else {
            pads[i].err_no = 1;
        }
    }
    x360_net_controllers(pads_,count);
}
