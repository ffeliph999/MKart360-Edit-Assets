#ifndef MK64_NETPLAY_DIAGNOSTIC_H
#define MK64_NETPLAY_DIAGNOSTIC_H
#include "netplay_protocol.h"
namespace mknet {
/* Hash synchronized inputs only. Slot/view preferences never enter the state. */
inline uint32_t diagnostic_step(uint32_t hash, const Pad *pads, unsigned players) {
    if (players < 2 || players > MAX_PLAYERS) return hash;
    for (unsigned i=0; i<players; ++i) {
        hash=(hash^pads[i].buttons)*16777619U;
        hash=(hash^uint8_t(pads[i].x))*16777619U;
        hash=(hash^uint8_t(pads[i].y))*16777619U;
    }
    return hash;
}
}
#endif
