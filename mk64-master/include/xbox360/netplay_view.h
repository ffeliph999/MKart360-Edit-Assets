#ifndef MK64_NETPLAY_VIEW_H
#define MK64_NETPLAY_VIEW_H
namespace mkview {
struct Rect { int x,y,w,h; };
inline bool crop(int mode,int players,int slot,Rect &r){
    r.x=r.y=0;r.w=1280;r.h=720;
    if(players<2||players>4||slot<0||slot>=players)return false;
    if(mode==1&&players==2){r.h=360;r.y=slot*360;}
    else if(mode==2&&players==2){r.w=640;r.x=slot*640;}
    else if(mode==3){r.w=640;r.h=360;r.x=(slot%2)*640;r.y=(slot/2)*360;}
    else return false;
    return true;
}
inline bool intersect(Rect a,Rect b,Rect &r){
    r.x=a.x>b.x?a.x:b.x;r.y=a.y>b.y?a.y:b.y;
    int right=a.x+a.w<b.x+b.w?a.x+a.w:b.x+b.w;
    int bottom=a.y+a.h<b.y+b.h?a.y+a.h:b.y+b.h;
    r.w=right-r.x;r.h=bottom-r.y;return r.w>0&&r.h>0;
}
inline void vertex(float *v,Rect viewport,Rect crop){
    v[0]=v[0]*(float(viewport.w)/crop.w)+v[3]*(float(2*(viewport.x-crop.x)+viewport.w)/crop.w-1.0f);
    v[1]=v[1]*(float(viewport.h)/crop.h)+v[3]*(1.0f-float(2*(viewport.y-crop.y)+viewport.h)/crop.h);
}
}
#endif
