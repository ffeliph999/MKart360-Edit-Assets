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


static volatile LONG music_enabled=1;
extern "C" int x360_music_enabled(void){return InterlockedCompareExchange(&music_enabled,0,0)!=0;}
extern "C" int x360_music_set_enabled(int enabled){
    InterlockedExchange(&music_enabled,enabled?1:0);
    const BYTE data[8]={'M','K','M','U','S','1',(BYTE)(enabled?1:0),(BYTE)(enabled?0xFE:0xFF)};
    HANDLE f=CreateFileA("game:\\mk64-music.cfg",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,0);
    if(f==INVALID_HANDLE_VALUE)return 0;
    DWORD written=0;bool ok=WriteFile(f,data,sizeof(data),&written,0)&&written==sizeof(data);
    CloseHandle(f);return ok?1:0;
}
static void load_music_setting(){
    HANDLE f=CreateFileA("game:\\mk64-music.cfg",GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    if(f==INVALID_HANDLE_VALUE)return;
    BYTE data[8];DWORD got=0;
    bool ok=GetFileSize(f,0)==sizeof(data)&&ReadFile(f,data,sizeof(data),&got,0)&&got==sizeof(data);
    CloseHandle(f);
    if(ok&&!memcmp(data,"MKMUS1",6)&&data[6]<=1&&data[7]==(BYTE)(data[6]^0xFF))
        InterlockedExchange(&music_enabled,data[6]);
}
static mkcontrols::Config controls;
static uint8_t steering_sensitivity[4]={100,100,100,100};
static bool steering_settings_loaded;
static const char *steering_path="game:\\mk64-steering.cfg";
static void load_steering_settings(){
    if(steering_settings_loaded)return;steering_settings_loaded=true;
    HANDLE f=CreateFileA(steering_path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    if(f==INVALID_HANDLE_VALUE)return;
    BYTE data[10];DWORD got=0;
    bool ok=GetFileSize(f,0)==sizeof(data)&&ReadFile(f,data,sizeof(data),&got,0)&&got==sizeof(data);CloseHandle(f);
    if(!ok||memcmp(data,"MKSTR1",6))return;
    BYTE sum=0;for(int i=0;i<9;++i)sum^=data[i];if(sum!=data[9])return;
    for(int i=0;i<4;++i)if(data[6+i]<50||data[6+i]>150)return;
    for(int i=0;i<4;++i)steering_sensitivity[i]=data[6+i];
}
static bool save_steering_settings(){
    BYTE data[10]={'M','K','S','T','R','1',0,0,0,0};
    for(int i=0;i<4;++i)data[6+i]=steering_sensitivity[i];
    BYTE sum=0;for(int i=0;i<9;++i)sum^=data[i];data[9]=sum;
    HANDLE f=CreateFileA(steering_path,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,0);
    if(f==INVALID_HANDLE_VALUE)return false;DWORD written=0;
    bool ok=WriteFile(f,data,sizeof(data),&written,0)&&written==sizeof(data);CloseHandle(f);return ok;
}
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
    if(controls_loaded)return;controls_loaded=true;mkcontrols::defaults(controls);load_music_setting();load_steering_settings();
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
    if(ok){controls_slot=slot;controls_generation=gen;}
    bool steering_ok=save_steering_settings();return ok&&steering_ok?1:0;
}
extern "C" const char *x360_control_action(int action){return action>=0&&action<mkcontrols::ACTIONS?mkcontrols::actions[action]:"";}
extern "C" const char *x360_control_binding(int player,int action){x360_controls_load();if(player<0||player>=4||action<0||action>=mkcontrols::ACTIONS)return "";unsigned b=controls.player[player].bind[action];return b<mkcontrols::SOURCES?mkcontrols::sources[b]:"UNBOUND";}
extern "C" void x360_control_bind(int player,int action,int source){x360_controls_load();if(player>=0&&player<4&&action>=0&&action<mkcontrols::ACTIONS&&(source>=0&&source<mkcontrols::SOURCES||source==mkcontrols::UNBOUND))controls.player[player].bind[action]=(uint8_t)source;}
extern "C" void x360_control_defaults(int player){x360_controls_load();if(player>=0&&player<4){mkcontrols::defaults(controls.player[player]);steering_sensitivity[player]=100;}}
extern "C" int x360_control_stick(int player,int change){x360_controls_load();if(player<0||player>=4)return 0;if(change)controls.player[player].stick^=1;return controls.player[player].stick;}
extern "C" int x360_control_deadzone(int player,int change){x360_controls_load();if(player<0||player>=4)return 24;int d=controls.player[player].deadzone+change;if(d<5)d=5;if(d>40)d=40;controls.player[player].deadzone=(uint8_t)d;return d;}
extern "C" int x360_control_sensitivity(int player,int change){x360_controls_load();if(player<0||player>=4)return 100;int s=(int)steering_sensitivity[player]+change;if(s<50)s=50;if(s>150)s=150;steering_sensitivity[player]=(uint8_t)s;return s;}
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
static int8_t steering_curve(int8_t analog,unsigned sensitivity){
    if(!analog||sensitivity==100)return analog;
    const int sign=analog<0?-1:1;
    int mag=analog<0?-(int)analog:(int)analog;
    const int maxmag=analog<0?128:127;
    int factor;
    if(sensitivity<100)factor=(int)sensitivity+(100-(int)sensitivity)*mag/maxmag;
    else factor=(int)sensitivity-((int)sensitivity-100)*mag/maxmag;
    mag=(mag*factor+50)/100;if(mag>maxmag)mag=maxmag;
    int value=sign*mag;if(value<-128)value=-128;if(value>127)value=127;return (int8_t)value;
}
static void convert_one(const XINPUT_STATE &s,PadCompat &p,int player){
    memset(&p,0,sizeof(p));const mkcontrols::Profile &profile=controls.player[player];const XINPUT_GAMEPAD &g=s.Gamepad;
    p.button=mkcontrols::translate(profile,physical_down(g));
    p.stick_x=mkcontrols::steer(steering_curve(mkcontrols::axis(profile.stick?g.sThumbRX:g.sThumbLX,profile.deadzone),steering_sensitivity[player]),physical_down(g));
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
        if (i < 4 && XInputGetState((DWORD)i, &s) == ERROR_SUCCESS) {
            convert_one(s, pads[i],i);
        } else {
            pads[i].err_no = 1;
        }
    }
    x360_net_controllers(pads_,count);
}
