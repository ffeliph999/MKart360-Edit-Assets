#ifndef MK64_AUDIO_QUEUE_H
#define MK64_AUDIO_QUEUE_H
#include <stdint.h>
namespace mkaudio {
enum { RATE=26800,CHANNELS=2,RING_COUNT=24,RING_BYTES=0x2000,DESIRED=2680,START_BUFFERS=3 };
inline unsigned buffered(uint64_t submitted,uint64_t played){return submitted>played?unsigned(submitted-played):0;}
inline bool can_submit(unsigned bytes,unsigned queued){return bytes&&bytes<=RING_BYTES&&bytes%4==0&&queued<RING_COUNT;}
inline bool should_start(bool playing,unsigned queued){return !playing&&queued>=START_BUFFERS;}
/* Keep the game's synthesis chunk limits; only change its queue target. */
inline int generation_size(int nominal,unsigned queued,int minimum,int maximum){int wanted=nominal-int(queued)+DESIRED;wanted=(wanted&~15)+16;return wanted<minimum?minimum:wanted>maximum?maximum:wanted;}
}
#endif
