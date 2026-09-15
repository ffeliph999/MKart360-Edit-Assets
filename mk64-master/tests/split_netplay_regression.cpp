#define main legacy_stress_main
#include "MK64_NETPLAY_4_TO_8_STRESS_TEST.cpp"
#undef main

static unsigned configurations;
static void split_edges(){
    uint8_t sid[16]={0},p[MAX_PACKET]={0};
    for(unsigned cap=4;cap<=8;cap+=4)for(unsigned used=1;used<=cap;++used){
        CHECK(reservation_fits(used,0,1,cap)==(used<cap));
        CHECK(reservation_fits(used,0,2,cap)==(used+2<=cap));
        CHECK(!reservation_fits(used,0,0,cap));CHECK(!reservation_fits(used,0,3,cap));
        if(used>1){CHECK(reservation_fits(used,1,1,cap));CHECK(reservation_fits(used,1,2,cap)==(used<cap));}
    }
    lobby_capacity()=8;
    int n=header(p,HELLO,sid,1);p[HEADER]=2;CHECK(valid(p,n));
    p[HEADER]=3;CHECK(!valid(p,n));p[HEADER]=0;CHECK(!valid(p,n));
    n=header(p,START,sid,5);
    p[HEADER]=4;p[HEADER+1]=8;p[HEADER+2]=6;p[HEADER+3]=1;p[HEADER+4]=1;
    CHECK(valid(p,n));p[HEADER+2]=7;CHECK(!valid(p,n));
    p[HEADER+2]=6;p[HEADER+4]=0;CHECK(!valid(p,n));
    for(unsigned slot=1;slot<7;++slot){
        Stream4 host,guest,other;
        host.reset(4,8,0);guest.reset(4,8,slot,2);
        other.reset(4,8,slot==1?3:1);
        Pad pair[2]={make_input(0,slot),make_input(0,slot+1)};
        guest.sample_locals(pair,123);n=guest.client_packet(p,sid);
        CHECK(valid(p,n));CHECK(p[HEADER+3]==1);
        CHECK(!host.receive_remote(slot,p,n,1));CHECK(!host.fault);
        CHECK(!host.receive_remote(slot+1,p,n,2));CHECK(!host.fault);
        CHECK(!guest.receive_remote(slot,p,n,2));CHECK(!guest.fault);
        CHECK(host.receive_remote(slot,p,n,2));
        CHECK(host.receive_remote(slot,p,n,2)); // Retransmitted pair is idempotent.
        CHECK(other.receive_remote(slot,p,n,2));
        CHECK(equal(host.inputs[slot][4].pad,pair[0]));
        CHECK(equal(host.inputs[slot+1][4].pad,pair[1]));
        p[n-1]^=1;CHECK(!host.receive_remote(slot,p,n,2));CHECK(host.fault);
        BootBarrier b;b.reset(8);
        for(unsigned i=1;i<8;++i)if(i!=slot&&i!=slot+1)CHECK(b.ready(i));
        CHECK(!b.all_ready());CHECK(b.ready_span(slot,2));CHECK(b.all_ready());
        CHECK(!b.ready_span(7,2));CHECK(!b.ready_span(0,2));
    }
    Stream4 invalid;invalid.reset(4,8,7,2);CHECK(invalid.fault);
}

static void compositions(unsigned players,unsigned next,unsigned ownership[8],bool split){
    if(next==players){
        if(!split)return;
        const Scenario clean={"split-clean",0,0,0,false,900,90000};
        const Scenario rough={"split-loss-outage",180,60,8,true,900,150000};
        ++configurations;
        run_sim(players,clean,0xB3100000+configurations,ownership);
        run_sim(players,rough,0xB3101000+configurations,ownership);
        if(players<=4)run_sim(players,rough,0xB3102000+configurations,ownership,4);
        return;
    }
    ownership[next]=1;compositions(players,next+1,ownership,split);
    if(next+1<players){ownership[next]=2;ownership[next+1]=0;compositions(players,next+2,ownership,true);}
    ownership[next]=0;
}
int main(){
    split_edges();
    for(unsigned players=3;players<=8;++players){unsigned ownership[8]={1};compositions(players,1,ownership,false);}
    printf("PASS SPLIT NETPLAY: %u console layouts, %u checks\n",configurations,checks);
    return 0;
}
