#include <xtl.h>
#include <winsockx.h>
#include <d3d9.h>
#include <stdio.h>
#include <stdarg.h>
#include "xbox360/netplay.h"
#include "xbox360/netplay_protocol.h"
extern "C" IDirect3DDevice9 *x360_d3d_device(void);
extern "C" void x360_log(const char *);
/* UI uses target clears, so it needs no external font file or shader state. */
#include "xbox360_netfont.h"
static void screen(const char *title,const char *a,const char *b,const char *c,const char *d) {
    IDirect3DDevice9 *dev=x360_d3d_device();if(!dev)return;
    dev->Clear(0,0,D3DCLEAR_TARGET,0xFF102030,1,0);
    net_text(dev,72,64,title,5,0xFFFFD050);net_text(dev,72,170,a,3,0xFFFFFFFF);
    net_text(dev,72,240,b,3,0xFFFFFFFF);net_text(dev,72,330,c,3,0xFF90D0FF);
    net_text(dev,72,410,d,3,0xFF90D0FF);
    net_text(dev,72,580,"B BACK    Y TOGGLE LOGGING",3,0xFFAAAAAA);
    net_text(dev,72,630,x360_logging_enabled()?"LOGGING ON":"LOGGING OFF",3,0xFFAAAAAA);
    dev->Present(0,0,0,0);
}
static WORD prev_buttons;
static WORD pressed() {
    XINPUT_STATE s;memset(&s,0,sizeof(s));XInputGetState(0,&s);
    WORD p=s.Gamepad.wButtons&~prev_buttons;prev_buttons=s.Gamepad.wButtons;
    if(p&XINPUT_GAMEPAD_Y)x360_set_logging(!x360_logging_enabled());return p;
}

/* B19.4R explicit xboxkrnl XEX resolver declarations */
/* xtl.h in this XDK configuration does not expose these prototypes. */
extern "C" DWORD XexGetModuleHandle(PSZ moduleName, PHANDLE hand);
extern "C" DWORD XexGetProcedureAddress(HANDLE hand, DWORD dwOrdinal, PVOID Address);

/* MK64NET B19.4R - XBOX 360 NETDLL IMPLEMENTATION
 *
 * Xbox 360 NetDll networking ABI used by MK64:
 * caller/xnc = 1, modern XNet/WSA startup exports when available, and the
 * normal NetDll socket family for all sockets used by netplay and UPnP.
 */
#define MK64_XNC_TITLE  1
#define MK64_XNC_SYSAPP 2
#define MK64_NETDLL_VERSION 0x20530800

/*
 * Normally we use the title caller (1), using the MK64 title-caller path for
 * Internet hosting on retail-like/freeboot consoles.  Some XDK-like
 * runtimes using the SYSAPP networking context may reject the
 * undocumented 0x5801 unencrypted-socket option for TITLE with WSAEACCES.
 * In that case open_network() switches this to SYSAPP (2) and retries.
 */
static DWORD mk64_xnc=MK64_XNC_TITLE;

typedef int    (__cdecl *MK_ND_XNETSTARTUP_OLD)(DWORD, XNetStartupParams *);
typedef int    (__cdecl *MK_ND_XNETSTARTUP_NEW)(DWORD, XNetStartupParams *, DWORD);
typedef int    (__cdecl *MK_ND_XNETRANDOM)(DWORD, BYTE *, UINT);
typedef DWORD  (__cdecl *MK_ND_XNETGETTITLEXNADDR)(DWORD, XNADDR *);

typedef int    (__cdecl *MK_ND_WSASTARTUP_OLD)(DWORD, WORD, WSADATA *);
typedef int    (__cdecl *MK_ND_WSASTARTUP_NEW)(DWORD, WORD, WSADATA *, DWORD);
typedef SOCKET (__cdecl *MK_ND_SOCKET)(DWORD, int, int, int);
typedef int    (__cdecl *MK_ND_CLOSESOCKET)(DWORD, SOCKET);
typedef int    (__cdecl *MK_ND_IOCTLSOCKET)(DWORD, SOCKET, long, u_long *);
typedef int    (__cdecl *MK_ND_SETSOCKOPT)(DWORD, SOCKET, int, int, const char *, int);
typedef int    (__cdecl *MK_ND_BIND)(DWORD, SOCKET, const struct sockaddr *, int);
typedef int    (__cdecl *MK_ND_CONNECT)(DWORD, SOCKET, const struct sockaddr *, int);
typedef int    (__cdecl *MK_ND_SELECT)(DWORD, int, fd_set *, fd_set *, fd_set *, const timeval *);
typedef int    (__cdecl *MK_ND_RECV)(DWORD, SOCKET, char *, int, int);
typedef int    (__cdecl *MK_ND_RECVFROM)(DWORD, SOCKET, char *, int, int, struct sockaddr *, int *);
typedef int    (__cdecl *MK_ND_SEND)(DWORD, SOCKET, const char *, int, int);
typedef int    (__cdecl *MK_ND_SENDTO)(DWORD, SOCKET, const char *, int, int, const struct sockaddr *, int);
typedef int    (__cdecl *MK_ND_WSAERROR)(void);

static MK_ND_XNETSTARTUP_OLD      mk_nd_xnetstartup_old=0;
static MK_ND_XNETSTARTUP_NEW      mk_nd_xnetstartup_new=0;
static MK_ND_XNETRANDOM           mk_nd_xnetrandom=0;
static MK_ND_XNETGETTITLEXNADDR   mk_nd_xnetgettitlexnaddr=0;
static MK_ND_WSASTARTUP_OLD       mk_nd_wsastartup_old=0;
static MK_ND_WSASTARTUP_NEW       mk_nd_wsastartup_new=0;
static MK_ND_SOCKET               mk_nd_socket=0;
static MK_ND_CLOSESOCKET          mk_nd_closesocket=0;
static MK_ND_IOCTLSOCKET          mk_nd_ioctlsocket=0;
static MK_ND_SETSOCKOPT           mk_nd_setsockopt=0;
static MK_ND_BIND                 mk_nd_bind=0;
static MK_ND_CONNECT              mk_nd_connect=0;
static MK_ND_SELECT               mk_nd_select=0;
static MK_ND_RECV                 mk_nd_recv=0;
static MK_ND_RECVFROM             mk_nd_recvfrom=0;
static MK_ND_SEND                 mk_nd_send=0;
static MK_ND_SENDTO               mk_nd_sendto=0;
static MK_ND_WSAERROR             mk_nd_wsaerror=0;
static bool                       mk_nd_resolved=false;

static DWORD mk_nd_proc(HANDLE h,DWORD ord) {
    DWORD p=0;
    return XexGetProcedureAddress(h,ord,&p)==0?p:0;
}

static bool mk_nd_resolve(void) {
    if(mk_nd_resolved)return true;
    HANDLE h=0;
    if(XexGetModuleHandle((PSZ)"xam.xex",&h)!=0||!h)return false;

    mk_nd_wsastartup_old     =(MK_ND_WSASTARTUP_OLD)    mk_nd_proc(h,0x01);
    mk_nd_socket             =(MK_ND_SOCKET)            mk_nd_proc(h,0x03);
    mk_nd_closesocket        =(MK_ND_CLOSESOCKET)       mk_nd_proc(h,0x04);
    mk_nd_ioctlsocket        =(MK_ND_IOCTLSOCKET)       mk_nd_proc(h,0x06);
    mk_nd_setsockopt         =(MK_ND_SETSOCKOPT)        mk_nd_proc(h,0x07);
    mk_nd_bind               =(MK_ND_BIND)              mk_nd_proc(h,0x0B);
    mk_nd_connect            =(MK_ND_CONNECT)           mk_nd_proc(h,0x0C);
    mk_nd_select             =(MK_ND_SELECT)            mk_nd_proc(h,0x0F);
    mk_nd_recv               =(MK_ND_RECV)              mk_nd_proc(h,0x12);
    mk_nd_recvfrom           =(MK_ND_RECVFROM)          mk_nd_proc(h,0x14);
    mk_nd_send               =(MK_ND_SEND)              mk_nd_proc(h,0x16);
    mk_nd_sendto             =(MK_ND_SENDTO)            mk_nd_proc(h,0x18);
    mk_nd_wsaerror           =(MK_ND_WSAERROR)          mk_nd_proc(h,0x1B);

    mk_nd_wsastartup_new     =(MK_ND_WSASTARTUP_NEW)    mk_nd_proc(h,0x24);
    mk_nd_xnetstartup_old    =(MK_ND_XNETSTARTUP_OLD)   mk_nd_proc(h,0x33);
    mk_nd_xnetrandom         =(MK_ND_XNETRANDOM)        mk_nd_proc(h,0x35);
    mk_nd_xnetgettitlexnaddr =(MK_ND_XNETGETTITLEXNADDR)mk_nd_proc(h,0x49);
    mk_nd_xnetstartup_new    =(MK_ND_XNETSTARTUP_NEW)   mk_nd_proc(h,0x50);

    mk_nd_resolved =
        (mk_nd_xnetstartup_new||mk_nd_xnetstartup_old) &&
        (mk_nd_wsastartup_new||mk_nd_wsastartup_old) &&
        mk_nd_xnetrandom && mk_nd_xnetgettitlexnaddr &&
        mk_nd_socket && mk_nd_closesocket && mk_nd_ioctlsocket &&
        mk_nd_setsockopt && mk_nd_bind && mk_nd_connect && mk_nd_select &&
        mk_nd_recv && mk_nd_recvfrom && mk_nd_send && mk_nd_sendto &&
        mk_nd_wsaerror;
    return mk_nd_resolved;
}

static int mk64_net_xnetstartup(XNetStartupParams *p) {
    if(!mk_nd_resolve())return SOCKET_ERROR;
    if(mk_nd_xnetstartup_new)
        return mk_nd_xnetstartup_new(mk64_xnc,p,MK64_NETDLL_VERSION);
    return mk_nd_xnetstartup_old(mk64_xnc,p);
}
static int mk64_net_xnetrandom(BYTE *p,UINT cb) {
    return mk_nd_resolve()?mk_nd_xnetrandom(mk64_xnc,p,cb):SOCKET_ERROR;
}
static DWORD mk64_net_xnetgettitlexnaddr(XNADDR *a) {
    return mk_nd_resolve()?mk_nd_xnetgettitlexnaddr(mk64_xnc,a):0;
}
static int mk64_net_wsastartup(WORD v,WSADATA *w) {
    if(!mk_nd_resolve())return SOCKET_ERROR;
    if(mk_nd_wsastartup_new)
        return mk_nd_wsastartup_new(mk64_xnc,v,w,MK64_NETDLL_VERSION);
    return mk_nd_wsastartup_old(mk64_xnc,v,w);
}
static SOCKET mk64_net_socket(int af,int type,int proto) {
    return mk_nd_resolve()?mk_nd_socket(mk64_xnc,af,type,proto):INVALID_SOCKET;
}
static int mk64_net_closesocket(SOCKET s) {
    return mk_nd_resolve()?mk_nd_closesocket(mk64_xnc,s):SOCKET_ERROR;
}
static int mk64_net_ioctlsocket(SOCKET s,long cmd,u_long *argp) {
    return mk_nd_resolve()?mk_nd_ioctlsocket(mk64_xnc,s,cmd,argp):SOCKET_ERROR;
}
static int mk64_net_setsockopt(SOCKET s,int level,int opt,const char *val,int len) {
    return mk_nd_resolve()?mk_nd_setsockopt(mk64_xnc,s,level,opt,val,len):SOCKET_ERROR;
}
static int mk64_net_bind(SOCKET s,const struct sockaddr *a,int n) {
    return mk_nd_resolve()?mk_nd_bind(mk64_xnc,s,a,n):SOCKET_ERROR;
}
static int mk64_net_connect(SOCKET s,const struct sockaddr *a,int n) {
    return mk_nd_resolve()?mk_nd_connect(mk64_xnc,s,a,n):SOCKET_ERROR;
}
static int mk64_net_select(int nfds,fd_set *r,fd_set *w,fd_set *e,const timeval *tv) {
    return mk_nd_resolve()?mk_nd_select(mk64_xnc,nfds,r,w,e,tv):SOCKET_ERROR;
}
static int mk64_net_recv(SOCKET s,char *b,int n,int f) {
    return mk_nd_resolve()?mk_nd_recv(mk64_xnc,s,b,n,f):SOCKET_ERROR;
}
static int mk64_net_recvfrom(SOCKET s,char *b,int n,int f,struct sockaddr *a,int *alen) {
    return mk_nd_resolve()?mk_nd_recvfrom(mk64_xnc,s,b,n,f,a,alen):SOCKET_ERROR;
}
static int mk64_net_send(SOCKET s,const char *b,int n,int f) {
    return mk_nd_resolve()?mk_nd_send(mk64_xnc,s,b,n,f):SOCKET_ERROR;
}
/* B19.4R3 built-in network stress simulator
 *
 * Only MK64 UDP/6464 traffic is affected. STUN, DNS and UPnP are untouched.
 *
 * The selected "simulated RTT" is split in half and applied as outbound
 * delay. Set the same profile on both peers to approximate the displayed RTT.
 *
 * Delayed packets are queued instead of sleeping the game thread, so this
 * behaves like network latency rather than a frame-rate stall.
 */
static const unsigned mk_stress_rtt_opts[]    = {0,50,100,200,300,500};
static const unsigned mk_stress_jitter_opts[] = {0,10,20,50,100,200};
static const unsigned mk_stress_loss_opts[]   = {0,1,2,5,10};
static int mk_stress_rtt_index=0;
static int mk_stress_jitter_index=0;
static int mk_stress_loss_index=0;

/* B19.4R4 variable-latency jitter simulation */

#define MK_STRESS_QUEUE 96
struct MkStressPacket {
    bool used;
    SOCKET s;
    int n;
    int flags;
    int alen;
    DWORD due;
    sockaddr_in to;
    char data[mknet::MAX_PACKET];
};
static MkStressPacket mk_stress_q[MK_STRESS_QUEUE];
static DWORD mk_stress_rng=0x4D4B3634U;
static unsigned mk_stress_queued=0;
static unsigned mk_stress_sent=0;
static unsigned mk_stress_dropped=0;
static unsigned mk_stress_overflow=0;
static unsigned mk_stress_delay_samples=0;
static unsigned mk_stress_delay_sum=0;
static unsigned mk_stress_delay_min=0xFFFFFFFFU;
static unsigned mk_stress_delay_max=0;

static unsigned mk_stress_rtt_ms(void) {
    return mk_stress_rtt_opts[mk_stress_rtt_index];
}
static unsigned mk_stress_jitter_ms(void) {
    return mk_stress_jitter_opts[mk_stress_jitter_index];
}
static unsigned mk_stress_loss_pct(void) {
    return mk_stress_loss_opts[mk_stress_loss_index];
}
static unsigned mk_stress_oneway_ms(void) {
    return mk_stress_rtt_ms()/2;
}
static bool mk_stress_enabled(void) {
    return mk_stress_rtt_ms()!=0 || mk_stress_jitter_ms()!=0 || mk_stress_loss_pct()!=0;
}
static unsigned mk_stress_rand32(void) {
    mk_stress_rng=mk_stress_rng*1664525U+1013904223U;
    return mk_stress_rng;
}
static unsigned mk_stress_rand100(void) {
    return (mk_stress_rand32()>>16)%100U;
}
static unsigned mk_stress_packet_delay_ms(void) {
    int delay=(int)mk_stress_oneway_ms();
    unsigned jitter=mk_stress_jitter_ms()/2;

    if(jitter) {
        unsigned span=jitter*2U+1U;
        int delta=(int)(mk_stress_rand32()%span)-(int)jitter;
        delay+=delta;
    }

    if(delay<0)delay=0;

    unsigned d=(unsigned)delay;
    ++mk_stress_delay_samples;
    mk_stress_delay_sum+=d;
    if(d<mk_stress_delay_min)mk_stress_delay_min=d;
    if(d>mk_stress_delay_max)mk_stress_delay_max=d;
    return d;
}
static void mk_stress_clear_queue(void) {
    memset(mk_stress_q,0,sizeof(mk_stress_q));
}
static void mk_stress_reset_stats(void) {
    mk_stress_queued=mk_stress_sent=mk_stress_dropped=mk_stress_overflow=0;
    mk_stress_delay_samples=mk_stress_delay_sum=mk_stress_delay_max=0;
    mk_stress_delay_min=0xFFFFFFFFU;
    mk_stress_rng^=GetTickCount()+0x9E3779B9U;
}
static void mk_stress_flush_due(void) {
    if(!mk_nd_resolve()||!mk_nd_sendto)return;
    DWORD now=GetTickCount();
    for(int i=0;i<MK_STRESS_QUEUE;++i) {
        MkStressPacket &q=mk_stress_q[i];
        if(!q.used)continue;
        if((LONG)(now-q.due)<0)continue;
        mk_nd_sendto(mk64_xnc,q.s,q.data,q.n,q.flags,(const sockaddr*)&q.to,q.alen);
        q.used=false;
        ++mk_stress_sent;
    }
}
static int mk_stress_sendto(SOCKET s,const char *b,int n,int f,const struct sockaddr *a,int alen) {
    if(!mk_nd_resolve())return SOCKET_ERROR;

    const bool game_packet = n>=mknet::HEADER && mknet::valid((const uint8_t*)b,n);
    if(!mk_stress_enabled() || !game_packet ||
       n<=0 || n>(int)sizeof(mk_stress_q[0].data)) {
        return mk_nd_sendto(mk64_xnc,s,b,n,f,a,alen);
    }

    /* Always let a real session GOODBYE leave immediately. */
    if(n>=mknet::HEADER && ((const unsigned char*)b)[5]==mknet::GOODBYE) {
        return mk_nd_sendto(mk64_xnc,s,b,n,f,a,alen);
    }

    if(mk_stress_loss_pct() && mk_stress_rand100()<mk_stress_loss_pct()) {
        ++mk_stress_dropped;
        /* Real UDP sendto() still reports success when the network later drops it. */
        return n;
    }

    unsigned delay=mk_stress_packet_delay_ms();
    if(!delay) {
        return mk_nd_sendto(mk64_xnc,s,b,n,f,a,alen);
    }

    mk_stress_flush_due();

    for(int i=0;i<MK_STRESS_QUEUE;++i) {
        MkStressPacket &q=mk_stress_q[i];
        if(q.used)continue;
        q.used=true;
        q.s=s;
        q.n=n;
        q.flags=f;
        q.alen=(int)sizeof(sockaddr_in);
        q.due=GetTickCount()+delay;
        q.to=*(const sockaddr_in*)a;
        memcpy(q.data,b,n);
        ++mk_stress_queued;
        return n;
    }

    /* Queue saturation behaves like a network drop, not an application error. */
    ++mk_stress_overflow;
    return n;
}

static int mk64_net_sendto(SOCKET s,const char *b,int n,int f,const struct sockaddr *a,int alen) {
    return mk_stress_sendto(s,b,n,f,a,alen);
}
static int mk64_net_wsaerror(void) {
    return mk_nd_resolve()?mk_nd_wsaerror():-1;
}

#define XNetStartup(p)                  mk64_net_xnetstartup((p))
#define XNetRandom(p,n)                 mk64_net_xnetrandom((BYTE*)(p),(UINT)(n))
#define XNetGetTitleXnAddr(a)           mk64_net_xnetgettitlexnaddr((a))
#define WSAStartup(v,w)                 mk64_net_wsastartup((v),(w))
#define socket(a,t,p)                   mk64_net_socket((a),(t),(p))
#define closesocket(s)                  mk64_net_closesocket((s))
#define ioctlsocket(s,c,a)              mk64_net_ioctlsocket((s),(c),(a))
#define setsockopt(s,l,o,v,n)           mk64_net_setsockopt((s),(l),(o),(v),(n))
#define bind(s,a,n)                     mk64_net_bind((s),(a),(n))
#define connect(s,a,n)                  mk64_net_connect((s),(a),(n))
#define select(n,r,w,e,t)               mk64_net_select((n),(r),(w),(e),(t))
#define recv(s,b,n,f)                   mk64_net_recv((s),(b),(n),(f))
#define recvfrom(s,b,n,f,a,l)           mk64_net_recvfrom((s),(b),(n),(f),(a),(l))
#define send(s,b,n,f)                   mk64_net_send((s),(b),(n),(f))
#define sendto(s,b,n,f,a,l)             mk64_net_sendto((s),(b),(n),(f),(a),(l))
#define WSAGetLastError()               mk64_net_wsaerror()

/* B22N1 2-4 PLAYER HOST-RELAY NETPLAY */
static SOCKET sock=INVALID_SOCKET;
static bool initialized,hosting,active,start_sent,failed;
static mknet::BootBarrier boot;
static uint8_t session[16],nonce[16];
static sockaddr_in host_peer;
static sockaddr_in join_target;
static bool host_session_known;
static unsigned assigned_slot;
static unsigned player_count=1;
static unsigned local_slot=0;
static unsigned chosen_delay=4;
static DWORD last_received;
static mknet::Stream4 stream;
static char public_address[80],local_address[80];

struct PeerState {
    bool used,ready,acked,gameplay_seen;
    sockaddr_in addr;
    uint8_t nonce[16];
    unsigned slot;
    DWORD last_received,last_offer;
    unsigned best_rtt;
};
static PeerState peers[mknet::MAX_PLAYERS-1];

static unsigned net_rx_total,net_rx_valid,net_rx_hello,net_rx_offer,net_rx_ready,net_rx_start,net_rx_ack;
static unsigned net_tx_punch,net_tx_punch_fail,net_rx_input,net_rx_input_reject,net_tx_input,net_tx_input_fail;
static bool first_gameplay_input;

static void net_log(const char *fmt,...) {
    if(!x360_logging_enabled())return;
    char text[384];va_list ap;va_start(ap,fmt);_vsnprintf(text,sizeof(text)-1,fmt,ap);va_end(ap);text[sizeof(text)-1]=0;x360_log(text);
}
static void reset_net_counters(){
    net_rx_total=net_rx_valid=net_rx_hello=net_rx_offer=net_rx_ready=net_rx_start=net_rx_ack=0;
    net_tx_punch=net_tx_punch_fail=net_rx_input=net_rx_input_reject=net_tx_input=net_tx_input_fail=0;
    first_gameplay_input=false;
}
static void peer_text(char *dst,const sockaddr_in &a){uint32_t ip=ntohl(a.sin_addr.s_addr);_snprintf(dst,79,"%u.%u.%u.%u:%u",ip>>24,(ip>>16)&255,(ip>>8)&255,ip&255,ntohs(a.sin_port));dst[79]=0;}
static bool same_address(const sockaddr_in &a,const sockaddr_in &b){return a.sin_addr.s_addr==b.sin_addr.s_addr&&a.sin_port==b.sin_port;}
static bool same_ip(const sockaddr_in &a,const sockaddr_in &b){return a.sin_addr.s_addr==b.sin_addr.s_addr;}
static void address_text(char *dst,uint32_t ip,unsigned port){_snprintf(dst,79,"%u.%u.%u.%u:%u",ip>>24,(ip>>16)&255,(ip>>8)&255,ip&255,port);dst[79]=0;}
#include "xbox360_upnp.h"

static int peer_count(void){int n=0;while(n<(int)(mknet::MAX_PLAYERS-1)&&peers[n].used)++n;return n;}
static int ready_count(void){int n=peer_count(),r=0;for(int i=0;i<n;++i)if(peers[i].ready)++r;return r;}
static int ack_count(void){int n=peer_count(),r=0;for(int i=0;i<n;++i)if(peers[i].acked)++r;return r;}
static bool all_ready(void){int n=peer_count();if(n<1)return false;for(int i=0;i<n;++i)if(!peers[i].ready)return false;return true;}
static bool all_acked(void){int n=peer_count();if(n<1)return false;for(int i=0;i<n;++i)if(!peers[i].acked)return false;return true;}
static int find_peer_address(const sockaddr_in &a){int n=peer_count();for(int i=0;i<n;++i)if(same_address(peers[i].addr,a))return i;return -1;}
static int find_peer_nonce(const uint8_t n[16]){int c=peer_count();for(int i=0;i<c;++i)if(!memcmp(peers[i].nonce,n,16))return i;return -1;}
static void reset_peer(PeerState &p){memset(&p,0,sizeof(p));p.best_rtt=0xFFFFFFFFU;}
static void remove_peer(int idx){
    int n=peer_count();if(idx<0||idx>=n)return;
    char who[80];peer_text(who,peers[idx].addr);net_log("MK64NET4: removing P%u %s before start\n",peers[idx].slot+1,who);
    for(int i=idx;i<n-1;++i){peers[i]=peers[i+1];peers[i].slot=(unsigned)i+1;peers[i].ready=false;peers[i].acked=false;peers[i].last_offer=0;}
    reset_peer(peers[n-1]);
}

static int send_packet_to(const sockaddr_in &to,mknet::Type t,const uint8_t sid[16],const uint8_t *payload,int size){
    if(sock==INVALID_SOCKET||size<0||size>mknet::MAX_PACKET-mknet::HEADER||(size&&!payload))return SOCKET_ERROR;
    uint8_t p[mknet::MAX_PACKET];int n=mknet::header(p,t,sid,size);
    if(size)memcpy(p+mknet::HEADER,payload,size);
    return sendto(sock,(const char*)p,n,0,(const sockaddr*)&to,sizeof(to));
}
static int send_peer_message(int idx,mknet::Type t,const uint8_t *payload,int size){
    int n=peer_count();if(idx<0||idx>=n)return SOCKET_ERROR;
    return send_packet_to(peers[idx].addr,t,session,payload,size);
}
static int send_host_message(mknet::Type t,const uint8_t *payload,int size){
    if(!host_session_known&&t!=mknet::HELLO)return SOCKET_ERROR;
    return send_packet_to(host_peer,t,t==mknet::HELLO?nonce:session,payload,size);
}

static void close_network(){
    if(sock!=INVALID_SOCKET){
        if(hosting){
            int n=peer_count();
            if(active||start_sent)for(int i=0;i<n;++i)send_peer_message(i,mknet::GOODBYE,0,0);
        }else if(host_session_known&&(active||start_sent)){
            send_host_message(mknet::GOODBYE,0,0);
        }
        if(mk_stress_enabled()){
            unsigned davg=mk_stress_delay_samples?mk_stress_delay_sum/mk_stress_delay_samples:0;
            unsigned dmin=mk_stress_delay_samples?mk_stress_delay_min:0;
            net_log("MK64NET4: STRESS summary rtt=%u jitter=+-%u loss=%u%% delay=%u/%u/%u ms queued=%u sent=%u dropped=%u overflow=%u\n",
                    mk_stress_rtt_ms(),mk_stress_jitter_ms(),mk_stress_loss_pct(),dmin,davg,mk_stress_delay_max,
                    mk_stress_queued,mk_stress_sent,mk_stress_dropped,mk_stress_overflow);
        }
        closesocket(sock);sock=INVALID_SOCKET;
    }
    mk_stress_clear_queue();
    active=start_sent=failed=false;boot.reset(1);host_session_known=false;assigned_slot=0;player_count=1;local_slot=0;
    memset(&host_peer,0,sizeof(host_peer));memset(&join_target,0,sizeof(join_target));
    for(unsigned i=0;i<mknet::MAX_PLAYERS-1;++i)reset_peer(peers[i]);
    unmap_router();
}

static bool open_network(bool host){
    close_network();hosting=host;reset_net_counters();mk_stress_reset_stats();
    net_log(host?"MK64NET4: host requested (up to 4 players)\n":"MK64NET4: join requested\n");
    if(!initialized){
        if(!mk_nd_resolve()){net_log("MK64NET4: MK64 NetDll export resolve FAILED\n");return false;}
        mk64_xnc=MK64_XNC_TITLE;
        net_log("MK64NET4: MK64 NetDll exports OK xnc=%u startup=%s wsa=%s ver=0x20530800\n",
                (unsigned)mk64_xnc,mk_nd_xnetstartup_new?"ord50":"ord33",mk_nd_wsastartup_new?"ord24":"ord01");
        XNetStartupParams p;memset(&p,0,sizeof(p));p.cfgSizeOfStruct=sizeof(p);p.cfgFlags=XNET_STARTUP_BYPASS_SECURITY;
        int xe=XNetStartup(&p);if(xe){net_log("MK64NET4: XNetStartup failed err=%d\n",xe);return false;}
        WSADATA w;int we=WSAStartup(MAKEWORD(2,2),&w);if(we){net_log("MK64NET4: WSAStartup failed err=%d\n",we);XNetCleanup();return false;}
        initialized=true;net_log("MK64NET4: MK64 NetDll XNet/WSA ready\n");
    }
    sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);if(sock==INVALID_SOCKET){net_log("MK64NET4: socket failed wsa=%d\n",WSAGetLastError());return false;}
    /*
     * MK64 INTERNET SOCKET SETUP:
     * Keep the game UDP socket on the exact MK64 NetDll socket setup that was
     * proven to discover the public STUN endpoint and accept Internet peers
     * in the confirmed-working 2-player build.  Earlier experimental builds added 0x5802 here;
     * that was not part of the proven setup.
     */
    BOOL rawSocket=TRUE;
    int opt5801=setsockopt(sock,SOL_SOCKET,0x5801,(PCSTR)&rawSocket,sizeof(rawSocket));
    int err5801=(opt5801==SOCKET_ERROR)?WSAGetLastError():0;
    net_log("MK64NET4: MK64 socket opt 5801=%s(%d) xnc=%u [MK64 Internet]\n",
            opt5801==0?"OK":"FAIL",err5801,(unsigned)mk64_xnc);

    /*
     * MK64 SYSAPP compatibility fallback.  A title-caller socket can be
     * denied the private 0x5801 option with WSAEACCES (10013) even though
     * XNet/WSA startup itself succeeds.  Recreate the complete NetDll context
     * as SYSAPP rather than mixing caller IDs on one socket.
     */
    if(opt5801==SOCKET_ERROR && err5801==10013 && mk64_xnc==MK64_XNC_TITLE){
        net_log("MK64NET4: 5801 denied for TITLE; retrying as SYSAPP xnc=2 [MK64 SYSAPP compat]\n");
        closesocket(sock);sock=INVALID_SOCKET;
        mk64_xnc=MK64_XNC_SYSAPP;

        XNetStartupParams p2;memset(&p2,0,sizeof(p2));p2.cfgSizeOfStruct=sizeof(p2);p2.cfgFlags=XNET_STARTUP_BYPASS_SECURITY;
        /* SYSAPP homebrew convention: prefer legacy XNetStartup (ord33). */
        int xe2=mk_nd_xnetstartup_old
            ?mk_nd_xnetstartup_old(MK64_XNC_SYSAPP,&p2)
            :mk_nd_xnetstartup_new(MK64_XNC_SYSAPP,&p2,MK64_NETDLL_VERSION);
        if(xe2){
            net_log("MK64NET4: SYSAPP XNetStartup failed err=%d api=%s\n",xe2,mk_nd_xnetstartup_old?"ord33":"ord50");
            mk64_xnc=MK64_XNC_TITLE;
            return false;
        }
        WSADATA w2;
        /* NetDll_WSAStartupEx SYSAPP examples use final version/context arg 2. */
        int we2=mk_nd_wsastartup_new
            ?mk_nd_wsastartup_new(MK64_XNC_SYSAPP,MAKEWORD(2,2),&w2,2)
            :mk_nd_wsastartup_old(MK64_XNC_SYSAPP,MAKEWORD(2,2),&w2);
        if(we2){
            net_log("MK64NET4: SYSAPP WSAStartup failed err=%d api=%s\n",we2,mk_nd_wsastartup_new?"ord24":"ord01");
            mk64_xnc=MK64_XNC_TITLE;
            return false;
        }

        sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        if(sock==INVALID_SOCKET){
            net_log("MK64NET4: SYSAPP socket failed wsa=%d\n",WSAGetLastError());
            mk64_xnc=MK64_XNC_TITLE;
            return false;
        }
        opt5801=setsockopt(sock,SOL_SOCKET,0x5801,(PCSTR)&rawSocket,sizeof(rawSocket));
        err5801=(opt5801==SOCKET_ERROR)?WSAGetLastError():0;
        net_log("MK64NET4: SYSAPP socket opt 5801=%s(%d) xnc=%u [MK64 SYSAPP compat]\n",
                opt5801==0?"OK":"FAIL",err5801,(unsigned)mk64_xnc);
    }

    if(opt5801==SOCKET_ERROR){
        net_log("MK64NET4: required MK64 0x5801 socket option failed xnc=%u\n",(unsigned)mk64_xnc);
        closesocket(sock);sock=INVALID_SOCKET;
        mk64_xnc=MK64_XNC_TITLE;
        return false;
    }
    int netbuf=128*1024;
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(PCSTR)&netbuf,sizeof(netbuf));
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(PCSTR)&netbuf,sizeof(netbuf));
    sockaddr_in bindaddr;memset(&bindaddr,0,sizeof(bindaddr));bindaddr.sin_family=AF_INET;bindaddr.sin_addr.s_addr=htonl(INADDR_ANY);bindaddr.sin_port=htons(6464);
    if(bind(sock,(sockaddr*)&bindaddr,sizeof(bindaddr))==SOCKET_ERROR){net_log("MK64NET4: bind failed port=6464 wsa=%d\n",WSAGetLastError());close_network();return false;}
    u_long nonblocking=1;if(ioctlsocket(sock,FIONBIO,&nonblocking)){net_log("MK64NET4: FIONBIO failed wsa=%d\n",WSAGetLastError());close_network();return false;}
    if(XNetRandom(session,sizeof(session))||XNetRandom(nonce,sizeof(nonce))){net_log("MK64NET4: XNetRandom failed\n");close_network();return false;}
    last_received=GetTickCount();
    if(host)net_log("MK64NET4: UDP 6464 bind OK; waiting for up to 3 guests\n");else net_log("MK64NET4: join UDP 6464 bind OK\n");
    return true;
}

static void public_ip(){
    strcpy(public_address,"PUBLIC ADDRESS UNAVAILABLE");strcpy(local_address,"LOCAL ADDRESS UNAVAILABLE");
    XNADDR addr;memset(&addr,0,sizeof(addr));XNetGetTitleXnAddr(&addr);if(addr.ina.s_addr)address_text(local_address,ntohl(addr.ina.s_addr),6464);

    XNDNS *dns=0;
    int dns_start=XNetDnsLookup("stun.cloudflare.com",0,&dns);
    if(dns_start||!dns){
        net_log("MK64NET4: STUN DNS start failed rc=%d dns=%s\n",dns_start,dns?"yes":"no");
        return;
    }
    DWORD begin=GetTickCount();
    while(dns->iStatus==WSAEINPROGRESS&&GetTickCount()-begin<4000){
        screen("HOST 2-4 PLAYER GAME","FINDING INTERNET ADDRESS",local_address,"","");
        if(pressed()&XINPUT_GAMEPAD_B)break;
        Sleep(10);
    }

    int dns_status=dns->iStatus;
    int dns_count=dns->cina;
    sockaddr_in stun;memset(&stun,0,sizeof(stun));stun.sin_family=AF_INET;stun.sin_port=htons(3478);
    bool found=dns_status==0&&dns_count>0;
    if(found)stun.sin_addr=dns->aina[0];
    XNetDnsRelease(dns);
    if(!found){
        net_log("MK64NET4: STUN DNS failed status=%d count=%d\n",dns_status,dns_count);
        return;
    }

    char stun_text[80];peer_text(stun_text,stun);
    net_log("MK64NET4: STUN target %s\n",stun_text);

    uint8_t request[20]={0,1,0,0,0x21,0x12,0xA4,0x42};
    if(XNetRandom(request+8,12)){
        net_log("MK64NET4: STUN transaction random generation failed\n");
        return;
    }

    begin=GetTickCount();DWORD sent=0;unsigned sends=0;
    while(GetTickCount()-begin<3500){
        if(!sent||GetTickCount()-sent>500){
            int sr=sendto(sock,(char*)request,20,0,(sockaddr*)&stun,sizeof(stun));
            ++sends;
            if(sr==SOCKET_ERROR&&sends<=4)net_log("MK64NET4: STUN send failed try=%u wsa=%d\n",sends,WSAGetLastError());
            sent=GetTickCount();
        }
        uint8_t p[1024];sockaddr_in from;int len=sizeof(from);
        int n=recvfrom(sock,(char*)p,sizeof(p),0,(sockaddr*)&from,&len);
        uint32_t ip;uint16_t port;
        if(n>0&&same_address(from,stun)&&mknet::stun_address(p,n,request+8,ip,port)){
            address_text(public_address,ip,port);
            net_log("MK64NET4: STUN public endpoint=%s\n",public_address);
            return;
        }
        screen("HOST 2-4 PLAYER GAME","FINDING INTERNET ADDRESS",local_address,"","");
        if(pressed()&XINPUT_GAMEPAD_B)break;
        Sleep(10);
    }
    net_log("MK64NET4: STUN timed out after %u sends\n",sends);
}

static void send_offer(int idx){
    if(idx<0||idx>=peer_count())return;
    PeerState &ps=peers[idx];uint8_t payload[24];memset(payload,0,sizeof(payload));
    memcpy(payload,ps.nonce,16);DWORD stamp=GetTickCount();mknet::put32(payload+16,stamp);payload[20]=uint8_t(ps.slot);payload[21]=uint8_t(1+peer_count());
    send_peer_message(idx,mknet::OFFER,payload,sizeof(payload));ps.last_offer=stamp;
}
static void send_start(int idx){
    if(idx<0||idx>=peer_count())return;
    uint8_t payload[4];payload[0]=uint8_t(chosen_delay);payload[1]=uint8_t(player_count);payload[2]=uint8_t(peers[idx].slot);payload[3]=0;
    send_peer_message(idx,mknet::START,payload,sizeof(payload));
}
static void send_ready_from_offer(const uint8_t *offer_payload){uint8_t payload[24];memcpy(payload,offer_payload,24);send_host_message(mknet::READY,payload,24);}

static void pump(){
    mk_stress_flush_due();
    for(int loop=0;loop<96;++loop){
        uint8_t p[mknet::MAX_PACKET+1];sockaddr_in from;int fl=sizeof(from);int n=recvfrom(sock,(char*)p,sizeof(p),0,(sockaddr*)&from,&fl);
        if(n==SOCKET_ERROR){int e=WSAGetLastError();if(e!=WSAEWOULDBLOCK&&e!=WSAEMSGSIZE){net_log("MK64NET4: recvfrom failed wsa=%d\n",e);failed=true;}break;}
        ++net_rx_total;
        if(n<mknet::HEADER){if(net_rx_total<=8)net_log("MK64NET4: short datagram bytes=%d dropped\n",n);continue;}
        if(!mknet::valid(p,n)){if(net_rx_total<=8)net_log("MK64NET4: invalid datagram bytes=%d type=%u\n",n,n>5?(unsigned)p[5]:255U);continue;}
        ++net_rx_valid;
        if(p[5]==mknet::HELLO)++net_rx_hello;else if(p[5]==mknet::OFFER)++net_rx_offer;else if(p[5]==mknet::READY)++net_rx_ready;else if(p[5]==mknet::START)++net_rx_start;else if(p[5]==mknet::START_ACK)++net_rx_ack;
        uint8_t *q=p+mknet::HEADER;DWORD now=GetTickCount();

        if(hosting){
            if(p[5]==mknet::HELLO&&!active&&!start_sent){
                int idx=find_peer_nonce(p+12);
                if(idx>=0){
                    if(!same_ip(peers[idx].addr,from)){net_log("MK64NET4: HELLO nonce seen from different IP - rejected\n");continue;}
                    if(!same_address(peers[idx].addr,from)){peers[idx].addr=from;peers[idx].ready=false;peers[idx].acked=false;net_log("MK64NET4: P%u NAT endpoint updated\n",peers[idx].slot+1);}
                }else{
                    idx=find_peer_address(from);
                    if(idx>=0){
                        memcpy(peers[idx].nonce,p+12,16);peers[idx].ready=false;peers[idx].acked=false;peers[idx].best_rtt=0xFFFFFFFFU;
                        net_log("MK64NET4: P%u restarted handshake\n",peers[idx].slot+1);
                    }else{
                        int count=peer_count();
                        if(count>=mknet::MAX_PLAYERS-1){net_log("MK64NET4: HELLO rejected - lobby full\n");continue;}
                        idx=count;reset_peer(peers[idx]);peers[idx].used=true;peers[idx].addr=from;memcpy(peers[idx].nonce,p+12,16);peers[idx].slot=(unsigned)idx+1;
                        char who[80];peer_text(who,from);net_log("MK64NET4: assigned %s -> P%u\n",who,peers[idx].slot+1);
                    }
                }
                peers[idx].last_received=now;send_offer(idx);continue;
            }

            if(memcmp(p+12,session,16))continue;
            int idx=find_peer_address(from);
            if(idx<0 && (p[5]==mknet::CLIENT_INPUT || p[5]==mknet::START_ACK)) {
                unsigned claimed=(p[5]==mknet::CLIENT_INPUT)?q[0]:q[0];
                int candidate=(int)claimed-1;
                if(candidate>=0&&candidate<peer_count()&&same_ip(peers[candidate].addr,from)) {
                    peers[candidate].addr=from;idx=candidate;
                    net_log("MK64NET4: P%u NAT endpoint rebound\n",peers[candidate].slot+1);
                }
            }
            if(idx<0)continue;PeerState &ps=peers[idx];

            if(p[5]==mknet::READY&&!active&&!start_sent){
                if(memcmp(q,ps.nonce,16)||q[20]!=ps.slot){net_log("MK64NET4: READY rejected P%u\n",ps.slot+1);continue;}
                DWORD stamp=mknet::get32(q+16);DWORD rtt=now-stamp;if(rtt>10000)continue;
                if(rtt<ps.best_rtt)ps.best_rtt=rtt;ps.ready=true;ps.last_received=now;
                net_log("MK64NET4: P%u READY rtt=%u ms\n",ps.slot+1,(unsigned)rtt);continue;
            }
            if(active&&p[5]==mknet::BOOT_READY){
                if(!boot.ready(ps.slot))continue;ps.last_received=now;
                if(boot.complete)send_peer_message(idx,mknet::BOOT_GO,0,0);
                continue;
            }
            if(p[5]==mknet::START_ACK&&start_sent){
                if(q[0]!=ps.slot)continue;ps.acked=true;ps.last_received=now;net_log("MK64NET4: P%u START_ACK\n",ps.slot+1);continue;
            }
            if((active||start_sent)&&p[5]==mknet::CLIENT_INPUT){
                ++net_rx_input;bool accepted=stream.receive_client(ps.slot,p,n);
                if(accepted){ps.last_received=now;ps.gameplay_seen=true;first_gameplay_input=true;if(start_sent)ps.acked=true;if(net_rx_input<=8)net_log("MK64NET4: P%u INPUT accepted frame=%u bytes=%d\n",ps.slot+1,(unsigned)stream.frame,n);}
                else{++net_rx_input_reject;if(net_rx_input_reject<=12)net_log("MK64NET4: P%u INPUT rejected frame=%u fault=%u\n",ps.slot+1,(unsigned)stream.frame,stream.fault?1U:0U);}
                continue;
            }
            if((active||start_sent)&&p[5]==mknet::GOODBYE){net_log("MK64NET4: P%u GOODBYE\n",ps.slot+1);failed=true;continue;}
        }else{
            if(p[5]==mknet::OFFER&&!active){
                if(!same_ip(join_target,from))continue;
                if(memcmp(q,nonce,16)){net_log("MK64NET4: OFFER nonce mismatch\n");continue;}
                unsigned slot=q[20];if(slot<1||slot>=mknet::MAX_PLAYERS)continue;
                host_peer=from;memcpy(session,p+12,16);host_session_known=true;assigned_slot=slot;last_received=now;
                send_ready_from_offer(q);net_log("MK64NET4: OFFER accepted; provisional slot P%u\n",assigned_slot+1);continue;
            }
            if(!host_session_known||memcmp(p+12,session,16))continue;
            if(!same_address(host_peer,from)) {
                if(same_ip(host_peer,from)&&(p[5]==mknet::START||p[5]==mknet::FRAMESET)) {
                    host_peer=from;net_log("MK64NET4: host NAT endpoint rebound\n");
                } else continue;
            }
            if(p[5]==mknet::START){
                unsigned delay=q[0],players=q[1],slot=q[2];if(slot!=assigned_slot||slot>=players)continue;
                if(!active){chosen_delay=delay;player_count=players;local_slot=slot;stream.reset(chosen_delay,player_count,local_slot);boot.reset(player_count);active=true;net_log("MK64NET4: START P%u players=%u delay=%u\n",local_slot+1,player_count,chosen_delay);}
                uint8_t ack=uint8_t(local_slot);send_host_message(mknet::START_ACK,&ack,1);last_received=now;continue;
            }
            if(active&&p[5]==mknet::BOOT_GO){boot.complete=true;last_received=now;continue;}
            if(active&&p[5]==mknet::FRAMESET){
                ++net_rx_input;bool accepted=stream.receive_frameset(p,n);
                if(accepted){last_received=now;first_gameplay_input=true;if(net_rx_input<=8)net_log("MK64NET4: FRAMESET accepted frame=%u bytes=%d\n",(unsigned)stream.frame,n);}
                else{++net_rx_input_reject;if(net_rx_input_reject<=12)net_log("MK64NET4: FRAMESET rejected frame=%u fault=%u\n",(unsigned)stream.frame,stream.fault?1U:0U);}
                continue;
            }
            if(active&&p[5]==mknet::GOODBYE){net_log("MK64NET4: host GOODBYE\n");failed=true;continue;}
        }
    }
}

static bool wait_for_xnet_route(){
    DWORD begin=GetTickCount();DWORD lastStatus=0xFFFFFFFF;
    while(GetTickCount()-begin<15000){
        XNADDR a;memset(&a,0,sizeof(a));DWORD st=XNetGetTitleXnAddr(&a);
        if(st!=lastStatus){char ipbuf[64]="0.0.0.0";if(a.ina.s_addr)address_text(ipbuf,ntohl(a.ina.s_addr),6464);net_log("MK64NET4: XNet addr status=0x%08X title=%s\n",st,ipbuf);lastStatus=st;}
        if((st&0x20)!=0&&a.ina.s_addr!=0){char ipbuf[64];address_text(ipbuf,ntohl(a.ina.s_addr),6464);net_log("MK64NET4: Internet route ready status=0x%08X title=%s\n",st,ipbuf);return true;}
        screen("NETWORK INIT","WAITING FOR XNET GATEWAY","PLEASE WAIT","B = CANCEL","");if(pressed()&XINPUT_GAMEPAD_B)return false;Sleep(100);
    }
    net_log("MK64NET4: Internet route NOT ready after 15 seconds\n");return false;
}

static bool host_ip_editor(sockaddr_in &out){
    char digits[16]="000.000.000.000";int cursor=0;
    while(true){
        char marker[80];memset(marker,' ',strlen(digits));marker[strlen(digits)]=0;marker[cursor]='^';
        screen("JOIN 2-4 PLAYER GAME",digits,marker,"ENTER HOST PUBLIC IPV4","A CONNECT - UDP 6464");WORD p=pressed();
        if(p&XINPUT_GAMEPAD_B)return false;
        if(p&XINPUT_GAMEPAD_DPAD_LEFT){do{cursor=(cursor+14)%15;}while(digits[cursor]=='.');}
        if(p&XINPUT_GAMEPAD_DPAD_RIGHT){do{cursor=(cursor+1)%15;}while(digits[cursor]=='.');}
        if(p&XINPUT_GAMEPAD_DPAD_UP)digits[cursor]=digits[cursor]=='9'?'0':digits[cursor]+1;
        if(p&XINPUT_GAMEPAD_DPAD_DOWN)digits[cursor]=digits[cursor]=='0'?'9':digits[cursor]-1;
        if(p&XINPUT_GAMEPAD_A){char ep[24];_snprintf(ep,sizeof(ep)-1,"%s:6464",digits);ep[sizeof(ep)-1]=0;uint32_t ip;uint16_t port;if(mknet::parse_endpoint(ep,ip,port)){memset(&out,0,sizeof(out));out.sin_family=AF_INET;out.sin_addr.s_addr=htonl(ip);out.sin_port=htons(6464);return true;}}
        Sleep(16);
    }
}

static void prune_prestart_timeouts(DWORD now){
    if(start_sent)return;
    for(int i=peer_count()-1;i>=0;--i)if(now-peers[i].last_received>30000)remove_peer(i);
}

static bool lobby(bool host){
    if(!open_network(host))return false;failed=false;
    if(!wait_for_xnet_route()){screen("NETWORK ERROR","NO XNET INTERNET ROUTE","CHECK NETWORK / LOG","B BACK","");Sleep(1500);close_network();return false;}
    strcpy(local_address,"LOCAL ADDRESS UNAVAILABLE");XNADDR addr;memset(&addr,0,sizeof(addr));XNetGetTitleXnAddr(&addr);if(addr.ina.s_addr)address_text(local_address,ntohl(addr.ina.s_addr),6464);

    if(host){
        strcpy(public_address,"DISCOVERING PUBLIC ADDRESS");public_ip();screen("HOST 2-4 PLAYER GAME",public_address,local_address,"SETTING UP UDP 6464 AUTOMATICALLY","");
        bool mapped=map_router(public_address);net_log("MK64NET4: host ready public=%s upnp=%s\n",public_address,mapped?"OK":"NO");
    }else{
        if(!host_ip_editor(join_target)){close_network();return false;}host_peer=join_target;char target[80];peer_text(target,join_target);net_log("MK64NET4: join target %s\n",target);
    }

    DWORD begin=GetTickCount();last_received=begin;DWORD last_hello=0;DWORD last_start_send=0;
    while(!active&&!failed){
        pump();WORD p=pressed();if(p&XINPUT_GAMEPAD_B){close_network();return false;}DWORD now=GetTickCount();
        if(!hosting&&!host_session_known&&now-last_hello>=250){
            int sent=send_packet_to(join_target,mknet::HELLO,nonce,0,0);if(sent==SOCKET_ERROR){++net_tx_punch_fail;if(net_tx_punch_fail<=4||(net_tx_punch_fail%40)==0)net_log("MK64NET4: HELLO send fail count=%u wsa=%d\n",net_tx_punch_fail,WSAGetLastError());}else{++net_tx_punch;if(net_tx_punch<=2)net_log("MK64NET4: HELLO send OK count=%u\n",net_tx_punch);}last_hello=now;
        }

        if(hosting){
            prune_prestart_timeouts(now);int n=peer_count();
            if(!start_sent){for(int i=0;i<n;++i)if(now-peers[i].last_offer>=1000)send_offer(i);}
            if(all_ready()&&!start_sent&&(p&XINPUT_GAMEPAD_A)){
                unsigned worst=0;for(int i=0;i<n;++i)if(peers[i].best_rtt!=0xFFFFFFFFU&&peers[i].best_rtt>worst)worst=peers[i].best_rtt;
                /* Host relay takes up to a full worst-peer RTT between clients, plus jitter. */
                unsigned d=(worst*30+999)/1000+2;chosen_delay=d<2?2:d>8?8:d;player_count=(unsigned)n+1;local_slot=0;stream.reset(chosen_delay,player_count,0);boot.reset(player_count);start_sent=true;last_start_send=0;
                for(int i=0;i<n;++i)peers[i].acked=false;net_log("MK64NET4: starting %u players delay=%u worstRTT=%u\n",player_count,chosen_delay,worst);
            }
            if(start_sent&&now-last_start_send>=100){for(int i=0;i<n;++i)if(!peers[i].acked)send_start(i);last_start_send=now;}
            if(start_sent&&all_acked()){active=true;net_log("MK64NET4: all START ACKs received; host active\n");break;}

            char status[80],diag[80];
            if(start_sent)_snprintf(status,sizeof(status)-1,"STARTING %uP - ACK %d/%d",player_count,ack_count(),n);
            else if(n==0)_snprintf(status,sizeof(status)-1,"WAITING FOR PLAYERS (1/4)");
            else if(all_ready())_snprintf(status,sizeof(status)-1,"PLAYERS %d/4 READY - A START",n+1);
            else _snprintf(status,sizeof(status)-1,"PLAYERS %d/4 - READY %d/%d",n+1,ready_count(),n);
            status[sizeof(status)-1]=0;
            _snprintf(diag,sizeof(diag)-1,"RX=%u V=%u H=%u R=%u",net_rx_total,net_rx_valid,net_rx_hello,net_rx_ready);diag[sizeof(diag)-1]=0;
            screen("HOST 2-4 PLAYER GAME",public_address,status,diag,upnp_mapped?"UDP 6464 AUTO-MAPPED":"UDP 6464 MUST BE REACHABLE");
            if(start_sent){for(int i=0;i<n;++i)if(now-peers[i].last_received>30000){net_log("MK64NET4: P%u start timeout\n",peers[i].slot+1);failed=true;}}
        }else{
            char status[80],diag[80];if(host_session_known)_snprintf(status,sizeof(status)-1,"ASSIGNED P%u - WAITING FOR HOST",assigned_slot+1);else _snprintf(status,sizeof(status)-1,"CONNECTING TO HOST");status[sizeof(status)-1]=0;
            _snprintf(diag,sizeof(diag)-1,"RX=%u V=%u OFFER=%u TX=%u F=%u",net_rx_total,net_rx_valid,net_rx_offer,net_tx_punch,net_tx_punch_fail);diag[sizeof(diag)-1]=0;
            screen("JOIN 2-4 PLAYER GAME",status,local_address,diag,"ALL GUESTS ENTER SAME HOST IP");
            if(now-last_received>30000){net_log("MK64NET4: join timeout after 30s\n");failed=true;}
        }
        Sleep(10);
    }
    if(active){prev_buttons=0;return true;}close_network();return false;
}

static void mk_stress_desc(char *dst,int size){if(!mk_stress_enabled())_snprintf(dst,size-1,"NET STRESS: OFF");else _snprintf(dst,size-1,"NET STRESS: %uMS J+-%u L%u%%",mk_stress_rtt_ms(),mk_stress_jitter_ms(),mk_stress_loss_pct());dst[size-1]=0;}
static void mk_stress_menu(void){
    int row=0;prev_buttons=0;
    for(;;){
        char a[80],b[80],c[80],d[80];
        if(mk_stress_rtt_ms())_snprintf(a,sizeof(a)-1,"%c SIMULATED RTT: %u MS",row==0?'>':' ',mk_stress_rtt_ms());else _snprintf(a,sizeof(a)-1,"%c SIMULATED RTT: OFF",row==0?'>':' ');a[sizeof(a)-1]=0;
        _snprintf(b,sizeof(b)-1,"%c RTT JITTER: +-%u MS",row==1?'>':' ',mk_stress_jitter_ms());b[sizeof(b)-1]=0;
        _snprintf(c,sizeof(c)-1,"%c PACKET LOSS: %u%%",row==2?'>':' ',mk_stress_loss_pct());c[sizeof(c)-1]=0;
        _snprintf(d,sizeof(d)-1,"SET SAME PROFILE ON ALL CONSOLES");d[sizeof(d)-1]=0;screen("NET STRESS TEST",a,b,c,d);
        WORD p=pressed();if(p&XINPUT_GAMEPAD_B){prev_buttons=0;return;}if(p&XINPUT_GAMEPAD_A){prev_buttons=0;return;}if(p&XINPUT_GAMEPAD_DPAD_UP)row=(row+2)%3;if(p&XINPUT_GAMEPAD_DPAD_DOWN)row=(row+1)%3;
        if(p&XINPUT_GAMEPAD_DPAD_RIGHT){if(row==0)mk_stress_rtt_index=(mk_stress_rtt_index+1)%(int)(sizeof(mk_stress_rtt_opts)/sizeof(mk_stress_rtt_opts[0]));else if(row==1)mk_stress_jitter_index=(mk_stress_jitter_index+1)%(int)(sizeof(mk_stress_jitter_opts)/sizeof(mk_stress_jitter_opts[0]));else mk_stress_loss_index=(mk_stress_loss_index+1)%(int)(sizeof(mk_stress_loss_opts)/sizeof(mk_stress_loss_opts[0]));mk_stress_clear_queue();}
        if(p&XINPUT_GAMEPAD_DPAD_LEFT){if(row==0){int n=(int)(sizeof(mk_stress_rtt_opts)/sizeof(mk_stress_rtt_opts[0]));mk_stress_rtt_index=(mk_stress_rtt_index+n-1)%n;}else if(row==1){int n=(int)(sizeof(mk_stress_jitter_opts)/sizeof(mk_stress_jitter_opts[0]));mk_stress_jitter_index=(mk_stress_jitter_index+n-1)%n;}else{int n=(int)(sizeof(mk_stress_loss_opts)/sizeof(mk_stress_loss_opts[0]));mk_stress_loss_index=(mk_stress_loss_index+n-1)%n;}mk_stress_clear_queue();}
        Sleep(16);
    }
}

extern "C" int x360_net_boot_menu(void){
    int selection=0;
    for(;;){
        char stress_line[80],stress_item[80];mk_stress_desc(stress_line,sizeof(stress_line));_snprintf(stress_item,sizeof(stress_item)-1,"%c %s",selection==3?'>':' ',stress_line);stress_item[sizeof(stress_item)-1]=0;
        screen("MARIO KART 64 - 2-4P NETPLAY",selection==0?"> OFFLINE":"  OFFLINE",selection==1?"> HOST 2-4 PLAYER GAME":"  HOST 2-4 PLAYER GAME",selection==2?"> JOIN 2-4 PLAYER GAME":"  JOIN 2-4 PLAYER GAME",stress_item);
        WORD p=pressed();if(p&XINPUT_GAMEPAD_DPAD_DOWN)selection=(selection+1)%4;if(p&XINPUT_GAMEPAD_DPAD_UP)selection=(selection+3)%4;
        if(p&XINPUT_GAMEPAD_A){
            if(selection==0){close_network();return 0;}
            if(selection==3){mk_stress_menu();continue;}
            if(lobby(selection==1))return 1;
            while(true){screen("CONNECTION NOT ESTABLISHED","NO GAME WAS STARTED","ALL GUESTS USE THE SAME HOST IP","CHECK UDP 6464 / NETWORK SETTINGS","A RETURN TO MENU");if(pressed()&XINPUT_GAMEPAD_A)break;Sleep(16);}
        }
        Sleep(16);
    }
}

extern "C" int x360_net_active(void){return active?1:0;}
extern "C" int x360_net_player_count(void){return active?(int)player_count:1;}
extern "C" int x360_net_local_slot(void){return active?(int)local_slot:0;}
extern "C" unsigned int x360_net_frame(void){return stream.frame;}
struct NetPadCompat {unsigned short button;signed char stick_x,stick_y;unsigned char err_no;};

static bool gameplay_timeout(DWORD now){
    if(hosting){int n=peer_count();for(int i=0;i<n;++i){DWORD timeout=peers[i].gameplay_seen?15000:60000;if(now-peers[i].last_received>timeout){net_log("MK64NET4: P%u input timeout age=%u\n",peers[i].slot+1,(unsigned)(now-peers[i].last_received));return true;}}return false;}
    DWORD timeout=first_gameplay_input?15000:60000;return now-last_received>timeout;
}

extern "C" void x360_net_controllers(void *pads_,int count){
    if(!active||count<2)return;NetPadCompat *pads=(NetPadCompat*)pads_;
    /* All peers reach the first controller read after shaders and game threads
     * are initialized. Keep the input clock stopped until every peer is here. */
    if(!boot.complete){
        DWORD begin=GetTickCount(),sent=0;
        while(!boot.complete&&!failed&&GetTickCount()-begin<60000){
            DWORD now=GetTickCount();
            if(!hosting&&(!sent||now-sent>=100)){send_host_message(mknet::BOOT_READY,0,0);sent=now;}
            pump();
            if(hosting){
                int pc=peer_count();
                if(boot.all_ready()){boot.complete=true;for(int i=0;i<pc;++i){peers[i].last_received=now;send_peer_message(i,mknet::BOOT_GO,0,0);}}
            }
            XINPUT_STATE cancel;memset(&cancel,0,sizeof(cancel));XInputGetState(0,&cancel);
            if((cancel.Gamepad.wButtons&(XINPUT_GAMEPAD_BACK|XINPUT_GAMEPAD_START))==(XINPUT_GAMEPAD_BACK|XINPUT_GAMEPAD_START))failed=true;
            if(!boot.complete)Sleep(1);
        }
        if(!boot.complete)failed=true;
    }
    mknet::Pad input={pads[0].err_no?0:pads[0].button,pads[0].err_no?0:pads[0].stick_x,pads[0].err_no?0:pads[0].stick_y};
    stream.sample_local(input,x360_net_state_hash());DWORD begin=GetTickCount(),sent=0;uint32_t sent_complete=0xFFFFFFFFU;mknet::Pad frame_pads[mknet::MAX_PLAYERS];
    while(true){
        pump();DWORD now=GetTickCount();
        if(!sent||now-sent>=15||(hosting&&sent_complete!=stream.latest_complete)){
            uint8_t packet[mknet::MAX_PACKET];int n;
            if(hosting){
                n=stream.frameset_packet(packet,session);int pc=peer_count();for(int i=0;i<pc;++i){int sr=sendto(sock,(char*)packet,n,0,(sockaddr*)&peers[i].addr,sizeof(peers[i].addr));++net_tx_input;if(sr==SOCKET_ERROR){++net_tx_input_fail;if(net_tx_input_fail<=12)net_log("MK64NET4: FRAMESET send fail P%u tx=%u frame=%u wsa=%d\n",peers[i].slot+1,net_tx_input,(unsigned)stream.frame,WSAGetLastError());}}
            }else{
                n=stream.client_packet(packet,session);int sr=sendto(sock,(char*)packet,n,0,(sockaddr*)&host_peer,sizeof(host_peer));++net_tx_input;if(sr==SOCKET_ERROR){++net_tx_input_fail;if(net_tx_input_fail<=12)net_log("MK64NET4: CLIENT_INPUT send fail P%u tx=%u frame=%u wsa=%d\n",local_slot+1,net_tx_input,(unsigned)stream.frame,WSAGetLastError());}
            }
            sent=now;sent_complete=stream.latest_complete;
        }
        XINPUT_STATE cancelState;memset(&cancelState,0,sizeof(cancelState));XInputGetState(0,&cancelState);
        if((cancelState.Gamepad.wButtons&(XINPUT_GAMEPAD_BACK|XINPUT_GAMEPAD_START))==(XINPUT_GAMEPAD_BACK|XINPUT_GAMEPAD_START)){net_log("MK64NET4: gameplay cancelled by BACK+START\n");failed=true;}
        if(failed||stream.fault||gameplay_timeout(now))break;
        if(stream.consume(frame_pads)){
            if(stream.frame<=4)net_log("MK64NET4: frame consumed=%u players=%u RX=%u REJ=%u TX=%u TF=%u\n",(unsigned)stream.frame,player_count,net_rx_input,net_rx_input_reject,net_tx_input,net_tx_input_fail);
            memset(pads,0,sizeof(*pads)*count);
            for(unsigned i=0;i<player_count&&i<(unsigned)count;++i){pads[i].button=frame_pads[i].buttons;pads[i].stick_x=frame_pads[i].x;pads[i].stick_y=frame_pads[i].y;pads[i].err_no=0;}
            for(int i=(int)player_count;i<count;++i)pads[i].err_no=1;
            return;
        }
        if(now-begin>1000){char diag[96],who[80];_snprintf(diag,sizeof(diag)-1,"%uP FRAME=%u RX=%u REJ=%u TXF=%u",player_count,(unsigned)stream.frame,net_rx_input,net_rx_input_reject,net_tx_input_fail);diag[sizeof(diag)-1]=0;if(hosting)_snprintf(who,sizeof(who)-1,"WAITING FOR REMOTE PLAYERS");else _snprintf(who,sizeof(who)-1,"P%u WAITING FOR HOST",local_slot+1);who[sizeof(who)-1]=0;screen("WAITING FOR PLAYERS","GAME PAUSED - RETRYING CONNECTION",who,diag,"BACK AND START EXIT GAME");}
        Sleep(1);
    }

    const bool desync=stream.fault;const unsigned stopped_players=player_count;const char *why=desync?"STATE/HASH MISMATCH":(failed?"PEER/CANCEL/NETWORK FAILURE":"GAMEPLAY INPUT TIMEOUT");
    net_log("MK64NET4: session stop reason=%s frame=%u players=%u RX=%u REJ=%u TX=%u TF=%u\n",why,(unsigned)stream.frame,stopped_players,net_rx_input,net_rx_input_reject,net_tx_input,net_tx_input_fail);
    close_network();char stats[96];_snprintf(stats,sizeof(stats)-1,"%uP RX=%u REJ=%u TX=%u F=%u",stopped_players,net_rx_input,net_rx_input_reject,net_tx_input,net_tx_input_fail);stats[sizeof(stats)-1]=0;
    for(;;){screen(desync?"GAME STATE MISMATCH":"CONNECTION LOST","SESSION STOPPED TO PREVENT DIVERGENCE",why,stats,"B EXIT GAME");if(pressed()&XINPUT_GAMEPAD_B)XLaunchNewImage(0,0);Sleep(16);}
}
