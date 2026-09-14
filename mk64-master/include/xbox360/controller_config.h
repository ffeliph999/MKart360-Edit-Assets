#ifndef MK64_CONTROLLER_CONFIG_H
#define MK64_CONTROLLER_CONFIG_H
#include <stdint.h>
#include <string.h>
namespace mkcontrols {
enum { PLAYERS=4,ACTIONS=14,SOURCES=24,UNBOUND=255,RECORD_BYTES=80 };
enum Source { A,B,X,Y,LB,RB,LT,RT,BACK,START,LS,RS,UP,DOWN,LEFT,RIGHT,
              RUP,RDOWN,RLEFT,RRIGHT,LUP,LDOWN,LLEFT,LRIGHT };
struct Profile { uint8_t bind[ACTIONS],stick,deadzone; };
struct Config { Profile player[PLAYERS]; };
static const uint16_t buttons[ACTIONS]={0x8000,0x4000,0x2000,0x1000,0x20,0x10,0x800,0x400,0x200,0x100,8,4,2,1};
static const char *const actions[ACTIONS]={"ACCELERATE / A","BRAKE / B","ITEM / Z","START","L","HOP / R","DPAD UP","DPAD DOWN","DPAD LEFT","DPAD RIGHT","C UP","C DOWN","C LEFT","C RIGHT"};
static const char *const sources[SOURCES]={"A","B","X","Y","LB","RB","LT","RT","BACK","START","LS CLICK","RS CLICK","DPAD UP","DPAD DOWN","DPAD LEFT","DPAD RIGHT","RS UP","RS DOWN","RS LEFT","RS RIGHT","LS UP","LS DOWN","LS LEFT","LS RIGHT"};
inline void defaults(Profile &p){const uint8_t d[ACTIONS]={A,B,LT,START,LB,RB,UP,DOWN,LEFT,RIGHT,RUP,RDOWN,RLEFT,RRIGHT};memcpy(p.bind,d,ACTIONS);p.stick=0;p.deadzone=24;}
inline void defaults(Config &c){for(int i=0;i<PLAYERS;++i)defaults(c.player[i]);}
inline uint16_t translate(const Profile&p,uint32_t down){uint16_t out=0;for(int a=0;a<ACTIONS;++a)if(p.bind[a]<SOURCES&&(down&(1U<<p.bind[a])))out|=buttons[a];return out;}
inline int8_t axis(int value,unsigned deadzone){
    const int threshold=int(deadzone)*32768/100;
    if(value>-threshold&&value<threshold)value=0;
    int scaled=value/256;
    if(scaled<-128)scaled=-128;if(scaled>127)scaled=127;return int8_t(scaled);
}
/* D-pad steering augments the chosen stick; opposite directions cancel. */
inline int8_t steer(int8_t analog,uint32_t down){
    const bool left=(down&(1U<<LEFT))!=0,right=(down&(1U<<RIGHT))!=0;
    return left||right ? (left==right?0:left?-127:127) : analog;
}
inline uint32_t get32(const uint8_t*p){return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
inline void put32(uint8_t*p,uint32_t n){p[0]=uint8_t(n>>24);p[1]=uint8_t(n>>16);p[2]=uint8_t(n>>8);p[3]=uint8_t(n);}
inline uint32_t checksum(const uint8_t*p){uint32_t h=2166136261U;for(int i=0;i<RECORD_BYTES-4;++i)h=(h^p[i])*16777619U;return h;}
inline void encode(const Config&c,uint32_t generation,uint8_t*p){memcpy(p,"MK64CTL1",8);put32(p+8,generation);for(int i=0;i<PLAYERS;++i){uint8_t*q=p+12+i*16;memcpy(q,c.player[i].bind,ACTIONS);q[14]=c.player[i].stick;q[15]=c.player[i].deadzone;}put32(p+76,checksum(p));}
inline bool decode(const uint8_t*p,unsigned bytes,Config &out,uint32_t &generation){
    if(bytes!=RECORD_BYTES||memcmp(p,"MK64CTL1",8)||get32(p+76)!=checksum(p))return false;
    Config candidate;
    for(int i=0;i<PLAYERS;++i){const uint8_t*q=p+12+i*16;for(int a=0;a<ACTIONS;++a)if(q[a]>=SOURCES&&q[a]!=UNBOUND)return false;if(q[14]>1||q[15]<5||q[15]>40)return false;memcpy(candidate.player[i].bind,q,ACTIONS);candidate.player[i].stick=q[14];candidate.player[i].deadzone=q[15];}
    out=candidate;generation=get32(p+8);return true;
}
}
#endif
