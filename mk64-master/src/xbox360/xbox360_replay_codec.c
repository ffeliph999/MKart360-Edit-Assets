#include <stddef.h>
#include <string.h>

/* The original two-stage replay API first emits flag/token groups, then
 * separates those groups into the three MIO0 streams. The first stage's
 * token count is consumed by the second stage on the game thread. */
static unsigned int replay_tokens;

static void put_be32(unsigned char *p, unsigned int v) {
    p[0]=(unsigned char)(v>>24); p[1]=(unsigned char)(v>>16);
    p[2]=(unsigned char)(v>>8); p[3]=(unsigned char)v;
}

int func_80040174(void *input, int length, void *output) {
    const unsigned char *in=(const unsigned char*)input;
    unsigned char *out=(unsigned char*)output;
    unsigned int pos=0, used=0, size, group=0, mask=0;
    replay_tokens=0;
    if (!input || !output || length<=0) return 0;
    size=(unsigned int)length;
    while(pos<size) {
        unsigned int start=pos>4096 ? pos-4096 : 0;
        unsigned int best=0, distance=0, scan;
        if (!mask) {group=used++;out[group]=0;mask=0x80;}
        for(scan=pos;scan>start;) {
            unsigned int n=0; --scan;
            while(n<18 && pos+n<size && in[scan+n]==in[pos+n]) ++n;
            if(n>best) {best=n;distance=pos-scan; if(n==18)break;}
        }
        if(best>=3) {
            unsigned int pair=((best-3)<<12)|(distance-1);
            /* Intermediate tokens are low-byte first, as in the MIPS routine. */
            out[used++]=(unsigned char)pair;out[used++]=(unsigned char)(pair>>8);
            pos+=best;
        } else {out[group]|=(unsigned char)mask;out[used++]=in[pos++];}
        ++replay_tokens;mask>>=1;
    }
    return (int)used;
}

int mio0encode(void *input, int length, void *output) {
    const unsigned char *in=(const unsigned char*)input;
    unsigned char *out=(unsigned char*)output;
    unsigned int i, pos=0, bits=0, flags=0, pairs=0, masks=(replay_tokens+7)/8;
    unsigned int pairpos, rawpos;
    if(!in || !out || length<0) return 0;
    for(i=0;i<replay_tokens;++i) {
        if(!bits) {flags=in[pos++];bits=8;}
        if(flags&0x80) ++pos;else {pos+=2;pairs+=2;}
        flags<<=1;--bits;
    }
    pairpos=(16+masks+3)&~3U;rawpos=pairpos+pairs;
    memset(out,0,rawpos);
    memcpy(out,"MIO0",4);put_be32(out+4,(unsigned int)length);
    put_be32(out+8,pairpos);put_be32(out+12,rawpos);
    pos=0;bits=0;masks=16;
    for(i=0;i<replay_tokens;++i) {
        if(!bits) {flags=in[pos++];bits=8;out[masks++]=(unsigned char)flags;}
        if(flags&0x80)out[rawpos++]=in[pos++];
        else {out[pairpos++]=in[pos+1];out[pairpos++]=in[pos];pos+=2;}
        flags<<=1;--bits;
    }
    return (int)rawpos;
}

/* Legacy alternate decoder: 16-bit flag groups interleaved with pairs.
 * Its output size is header word 2; literals start relative to header end. */
static unsigned int get_be32(const unsigned char *p) {
    return ((unsigned int)p[0]<<24)|((unsigned int)p[1]<<16)|((unsigned int)p[2]<<8)|p[3];
}
void func_80040030(unsigned char *src,unsigned char *dst) {
    unsigned int size=get_be32(src+8), written=0, bits=0, flags=0;
    unsigned char *stream=src+16,*raw=stream+get_be32(src+12);
    while(written<size) {
        if(!bits) {flags=((unsigned int)stream[0]<<8)|stream[1];stream+=2;bits=16;}
        if(flags&0x8000)dst[written++]=*raw++;
        else {
            unsigned int pair=((unsigned int)stream[0]<<8)|stream[1];
            unsigned int n=(pair>>12)+3,d=(pair&4095)+1;stream+=2;
            if(d>written || n>size-written)return;
            while(n--) {dst[written]=dst[written-d];++written;}
        }
        flags<<=1;--bits;
    }
}
