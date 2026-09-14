#ifndef MK64_TEXTURE_DIMENSIONS_H
#define MK64_TEXTURE_DIMENSIONS_H
/* LOADTILE coordinates are inclusive. A wrapped, masked tile may load an
 * extra row/column, but its sampling period remains 2^mask texels. */
static inline unsigned x360_texture_period(unsigned loaded, unsigned mask, unsigned mode) {
    if (!(mode & 2) && mask > 0 && mask <= 15) {
        unsigned period = 1U << mask;
        if (period < loaded) return period;
    }
    return loaded;
}
#endif
