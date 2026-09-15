#ifndef MK64_NETPLAY_AUDIO_H
#define MK64_NETPLAY_AUDIO_H
/* A racer ID and an audio listener ID are different namespaces. */
static int mk_audio_local_player(int online, int racing, int players, int slot) {
    return online && racing && players >= 2 && players <= 8 && slot >= 0 && slot < players ? slot : -1;
}
#endif
