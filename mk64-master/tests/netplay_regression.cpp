#include "xbox360/netplay_protocol.h"
#include "xbox360/netplay_view.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace mknet;
static unsigned checks;
#define CHECK(x) do { ++checks; if(!(x)){fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x);exit(1);} } while(0)
static bool near(float a,float b){return fabsf(a-b)<0.001f;}
static Pad input(unsigned frame,unsigned slot){Pad p={uint16_t((frame*19+slot*101)&0xffff),int8_t(frame*3+slot),int8_t(frame*7-slot)};return p;}
static uint32_t hash_step(uint32_t h,Pad p){h=(h^p.buttons)*16777619U;h=(h^uint8_t(p.x))*16777619U;return (h^uint8_t(p.y))*16777619U;}

static void views(){
    for(int wide=0;wide<2;++wide){
        mkview::Rect out=mkview::output(wide!=0),full={0,0,1280,720};
        CHECK(out.x==(wide?0:160));CHECK(out.w==(wide?1280:960));
        mkview::Rect mapped=mkview::output_rect(full,out);CHECK(mapped.x==out.x&&mapped.w==out.w&&mapped.h==720);
        for(int players=2;players<=4;++players)for(int mode=1;mode<=3;++mode)for(int slot=0;slot<players;++slot){
            mkview::Rect crop;
            if(!mkview::crop(mode,players,slot,crop))continue;
            float center[4]={0,0,0.5f,1};mkview::vertex(center,crop,crop);CHECK(near(center[0],0)&&near(center[1],0));
            float edge[4]={1,-1,0.5f,1};mkview::vertex(edge,crop,crop);CHECK(near(edge[0],1)&&near(edge[1],-1));
            // A point in a whole-screen overlay maps to the same local center.
            float overlay[4]={float(2*crop.x+crop.w)/1280-1,1-float(2*crop.y+crop.h)/720,0,1};
            mkview::vertex(overlay,full,crop);CHECK(near(overlay[0],0)&&near(overlay[1],0));
            // A native 16x16 HUD square must remain square in physical output pixels.
            float left=-0.2f,top=0.2f,right=left+32.0f/320,bottom=top-32.0f/240;
            float cx=(left+right)/2,cy=(top+bottom)/2;
            mkview::hud_rect(left,top,right,bottom,crop,out);
            CHECK(near((left+right)/2,cx)&&near((top+bottom)/2,cy));
            float width=(right-left)*640*out.w/crop.w,height=(top-bottom)*360*out.h/crop.h;
            CHECK(near(width,height));
            // 1P and online perspective must have the same vertical FOV and square world pixels.
            const float aspect=wide?16.0f/9:4.0f/3;
            const float focal=1.0f/tanf(40.0f*3.14159265f/360.0f);
            float world[4]={focal/(4.0f/3)*((4.0f/3)/aspect),focal,0,10};
            mkview::vertex(world,crop,crop);
            CHECK(near(world[0]/world[3]*out.w/2,world[1]/world[3]*out.h/2));
            for(int other=0;other<players;++other)if(other!=slot){mkview::Rect peer,intersection;CHECK(mkview::crop(mode,players,other,peer));CHECK(!mkview::intersect(crop,peer,intersection));}
        }
    }
    mkview::Rect r;CHECK(!mkview::crop(3,3,3,r));CHECK(!mkview::crop(0,2,0,r));
}

struct Message {unsigned due,from,to;int n;uint8_t data[MAX_PACKET];};
struct Due {bool operator()(const Message&a,const Message&b)const{return a.due<b.due;}};
static unsigned random_state=1;
static unsigned rng(){random_state=random_state*1664525U+1013904223U;return random_state;}
static void queue(std::vector<Message>&q,unsigned now,unsigned from,unsigned to,const uint8_t*p,int n,unsigned rtt,unsigned jitter,unsigned loss,bool burst){
    CHECK(n>0&&n<=MAX_PACKET);CHECK(valid(p,n));
    if(rng()%100<loss||(burst&&now>=2000&&now<2900))return;
    int delay=int(rtt/2)+(jitter?int(rng()%(2*jitter+1))-int(jitter):0);
    Message m;m.due=now+unsigned(delay<0?0:delay);m.from=from;m.to=to;m.n=n;memcpy(m.data,p,n);q.push_back(m);
    if(rng()%20==0){m.due+=7;q.push_back(m);}
}
static void simulate(unsigned players,unsigned rtt,unsigned jitter,unsigned loss,bool burst){
    Stream4 streams[MAX_PLAYERS];uint32_t state[MAX_PLAYERS]={1,1,1,1};
    uint32_t sampled[MAX_PLAYERS]={0xffffffffU,0xffffffffU,0xffffffffU,0xffffffffU};
    unsigned next[MAX_PLAYERS]={0},last_send[MAX_PLAYERS]={0};
    const unsigned delay=input_delay(rtt+2*jitter);
    const unsigned target=900;
    uint8_t session[16]={0},packet[MAX_PACKET];
    std::vector<Message> q;
    for(unsigned s=0;s<players;++s)streams[s].reset(delay,players,s);
    unsigned now=0;
    for(;now<120000;++now){
        std::sort(q.begin(),q.end(),Due());
        while(!q.empty()&&q[0].due<=now){Message m=q.front();q.erase(q.begin());
            if(m.to==0)streams[0].receive_client(m.from,m.data,m.n);else streams[m.to].receive_frameset(m.data,m.n);
        }
        bool done=true;
        for(unsigned s=0;s<players;++s){
            Stream4 &st=streams[s];CHECK(!st.fault);if(st.frame<target)done=false;
            if(now<next[s])continue;
            if(st.frame<target && sampled[s]!=st.frame){st.sample_local(input(st.frame,s),state[s]);sampled[s]=st.frame;last_send[s]=0;}
            if(!last_send[s]||now-last_send[s]>=15){
                if(s==0){for(unsigned peer=1;peer<players;++peer){int n=st.frameset_packet(packet,session,peer);queue(q,now,0,peer,packet,n,rtt,jitter,loss,burst);}}
                else{int n=st.client_packet(packet,session);queue(q,now,s,0,packet,n,rtt,jitter,loss,burst);}
                last_send[s]=now;
            }
            if(st.frame>=target)continue; // Continue relaying/retrying until every peer finishes.
            Pad pads[MAX_PLAYERS];unsigned f=st.frame;
            if(st.consume(pads)){
                for(unsigned p=0;p<players;++p){Pad expected={0,0,0};if(f>=delay)expected=input(f-delay,p);CHECK(equal(pads[p],expected));state[s]=hash_step(state[s],pads[p]);}
                next[s]=now+33;
            }
        }
        if(done)break;
    }
    for(unsigned s=0;s<players;++s){CHECK(streams[s].frame==target);CHECK(state[s]==state[0]);CHECK(!streams[s].fault);}
    printf("PASS %uP %ums RTT +/- %ums jitter %u%% loss%s: %u frames in %ums\n",players,rtt,jitter,loss,burst?" + 900ms outage":"",target,now);
}

static void protocol_edges(){
    uint8_t p[MAX_PACKET]={0},session[16]={0};
    BootBarrier b;b.reset(4);CHECK(!b.all_ready());CHECK(!b.ready(4));CHECK(b.ready(3));CHECK(b.ready(1));CHECK(!b.all_ready());CHECK(b.ready(2));CHECK(b.all_ready());
    Stream4 h,c;h.reset(4,2,0);c.reset(4,2,1);h.sample_local(input(0,0),123);c.sample_local(input(0,1),123);
    int n=c.client_packet(p,session);CHECK(h.receive_client(1,p,n));CHECK(h.receive_client(1,p,n));CHECK(!h.receive_client(2,p,n));
    for(int len=0;len<n;++len)CHECK(!valid(p,len));
    uint8_t saved=p[4];p[4]=2;CHECK(!valid(p,n));p[4]=saved;
    p[HEADER+16+4*4]^=1;CHECK(!h.receive_client(1,p,n));CHECK(h.fault);
    h.reset(4,2,0);c.reset(4,2,1);h.sample_local(input(0,0),1);c.sample_local(input(0,1),2);n=c.client_packet(p,session);CHECK(!h.receive_client(1,p,n));CHECK(h.fault);
    Latency l={0,0,0};for(int i=0;i<16;++i)l.add(40);unsigned stable=input_delay(l.budget());l.add(200);CHECK(input_delay(l.budget())>stable);CHECK(input_delay(0)==2);CHECK(input_delay(10000)==MAX_DELAY);
    // A receiver needs frame 50, outside the ordinary tail ending at frame 100.
    h.reset(4,2,0);h.frame=100;h.latest_complete=100;h.peer_frame[1]=50;
    for(unsigned f=0;f<=100;++f)for(unsigned s=0;s<2;++s){InputSlot &v=h.inputs[s][f%HISTORY];v.present=true;v.frame=f;v.pad=input(f,s);}
    n=h.frameset_packet(p,session,1);CHECK(valid(p,n));CHECK(get32(p+HEADER+4)==50);CHECK(p[HEADER+1]==REDUNDANCY);
    c.reset(4,2,1);c.frame=50;CHECK(c.receive_frameset(p,n));Pad recovered[MAX_PLAYERS];CHECK(c.consume(recovered));CHECK(equal(recovered[0],input(50,0)));
    h.peer_frame[1]=0;h.latest_complete=HISTORY;CHECK(h.frameset_packet(p,session,1)==0);CHECK(h.fault);
    // An eight-second-old datagram must not replace pending inputs in the same ring slot.
    c.reset(12,2,1);c.frame=245;c.sample_local(input(245,1),42);
    InputSlot future=c.inputs[1][257%HISTORY];
    h.reset(12,2,0);n=h.frameset_packet(p,session,1);CHECK(c.receive_frameset(p,n));
    CHECK(c.inputs[1][257%HISTORY].frame==future.frame);CHECK(equal(c.inputs[1][257%HISTORY].pad,future.pad));CHECK(!c.fault);
}
int main(){views();protocol_edges();for(unsigned p=2;p<=4;++p){simulate(p,0,0,0,false);simulate(p,80,20,5,false);simulate(p,180,60,10,false);simulate(p,80,30,5,true);}printf("PASS %u checks\n",checks);}
