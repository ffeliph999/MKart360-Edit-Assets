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
inline Rect output(bool widescreen) {
    Rect r={widescreen?0:160,0,widescreen?1280:960,720};return r;
}
/* X360_CRT_480I_NATIVE_BACKBUFFER:
 * Match game aspect to physical display aspect in the 1280x720 logical
 * coordinate space.  4:3-on-4:3 and 16:9-on-16:9 use the whole framebuffer. */
inline Rect output(bool widescreen,bool physical_widescreen) {
    Rect r={0,0,1280,720};
    if(widescreen==physical_widescreen)return r;
    if(!widescreen){r.x=160;r.w=960;}       /* 4:3 pillarbox on 16:9 */
    else {r.y=90;r.h=540;}                  /* 16:9 letterbox on 4:3 */
    return r;
}
inline Rect output_rect(Rect r,Rect out) {
    int right=out.x+(r.x+r.w)*out.w/1280;
    int bottom=out.y+(r.y+r.h)*out.h/720;
    r.x=out.x+r.x*out.w/1280;r.y=out.y+r.y*out.h/720;
    r.w=right-r.x;r.h=bottom-r.y;return r;
}
/* Rectangles are still intact here, before clipping can create triangle fans.
 * Preserve each HUD anchor and fit its native pixels uniformly to the output. */
inline void hud_rect(float &left,float &top,float &right,float &bottom,
                     Rect source,Rect out) {
    const float sx=float(out.w)/(float(source.w)/4.0f);
    const float sy=float(out.h)/(float(source.h)/3.0f);
    const float ux=sx>sy?sy/sx:1.0f,uy=sy>sx?sx/sy:1.0f;
    const float cx=(left+right)*0.5f,cy=(top+bottom)*0.5f;
    left=cx+(left-cx)*ux;right=cx+(right-cx)*ux;
    top=cy+(top-cy)*uy;bottom=cy+(bottom-cy)*uy;
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
