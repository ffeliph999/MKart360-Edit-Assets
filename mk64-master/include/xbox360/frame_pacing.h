#ifndef MK64_FRAME_PACING_H
#define MK64_FRAME_PACING_H
#include <stdint.h>
namespace mkpacing {
/* previous is the last frame deadline, not the start of the current work.
 * A late frame has already spent its budget: do not sleep another frame,
 * and do not catch up by running future frames faster than the target. */
inline int64_t next_deadline(int64_t previous, int64_t now, int64_t period) {
    if (!previous) return now + period;
    const int64_t next = previous + period;
    return now >= next ? now : next;
}
}
#endif
