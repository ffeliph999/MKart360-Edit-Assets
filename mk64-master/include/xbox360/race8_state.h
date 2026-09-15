#ifndef MK64_RACE8_STATE_H
#define MK64_RACE8_STATE_H
/* Shared, pointer-free rules used by the console and host-side tests. */
typedef struct {
    int players, course, cc, character[8], ready[8];
    unsigned short previous[8];
} Race8Lobby;
typedef struct {
    int players, count, place[8], order[8], finalLap[8], done;
} Race8Results;
static int race8_valid_count(int count) { return count >= 4 && count <= 8; }
static void race8_lobby_init(Race8Lobby *s, int count) {
    int i;
    s->players = race8_valid_count(count) ? count : 4;
    s->course = 0; s->cc = 1;
    for(i=0;i<8;++i) { s->character[i]=i; s->ready[i]=0; s->previous[i]=0; }
}
/* N64 button masks. Only host controls course, CC and start; every racer owns readiness. */
static int race8_lobby_step(Race8Lobby *s, const unsigned short *buttons) {
    int i,j,choice,changed=0,all=1; unsigned short press[8];
    for(i=0;i<s->players;++i) { press[i]=buttons[i]&~s->previous[i]; s->previous[i]=buttons[i]; }
    if(press[0]&0x0800) { s->course=(s->course+19)%20; changed=1; }
    if(press[0]&0x0400) { s->course=(s->course+1)%20; changed=1; }
    if(press[0]&0x0010) { s->cc=(s->cc+1)%3; changed=1; }
    if(changed) for(i=0;i<s->players;++i) s->ready[i]=0;
    for(i=0;i<s->players;++i) {
        if(press[i]&0x4000) s->ready[i]=0;
        if(!s->ready[i] && (press[i]&0x0300)) {
            choice=(s->character[i]+((press[i]&0x0100)?1:7))%8;
            for(j=0;j<s->players;++j) if(s->character[j]==choice) break;
            if(j==s->players || !s->ready[j]) {
                if(j<s->players) s->character[j]=s->character[i];
                s->character[i]=choice;
            }
        }
        if(!changed && (press[i]&0x8000)) s->ready[i]=1;
        if(!s->ready[i]) all=0;
    }
    return all && (press[0]&0x1000) != 0;
}
static void race8_results_init(Race8Results *s, int count) {
    int i; s->players=count; s->count=0; s->done=0;
    for(i=0;i<8;++i) { s->place[i]=-1; s->order[i]=-1; s->finalLap[i]=0; }
}
static int race8_finish(Race8Results *s, int slot, int battle) {
    int place;
    if(slot<0 || slot>=s->players || s->place[slot]>=0) return 0;
    place=battle?s->players-1-s->count:s->count;
    s->place[slot]=place; s->order[place]=slot; ++s->count;
    if(s->count==s->players) s->done=1;
    return 1;
}
static void race8_complete_last(Race8Results *s, int battle) {
    int i;
    if(s->count==s->players-1)
        for(i=0;i<s->players;++i) if(s->place[i]<0) { race8_finish(s,i,battle); break; }
}
#endif
