/* Portable replacements for the byte-memory helpers and MIPS MIO0 decoder.
 * Input remains the original big-endian compressed data on every host.
 */
#include <stddef.h>
#include <string.h>

void bcopy(const void *src, void *dst, size_t size) { memmove(dst, src, size); }
void bzero(void *dst, size_t size) { memset(dst, 0, size); }

static unsigned int read_be32(const unsigned char *p) {
    return ((unsigned int)p[0] << 24) | ((unsigned int)p[1] << 16)
         | ((unsigned int)p[2] << 8) | p[3];
}

void mio0decode(unsigned char *src, unsigned char *dst) {
    unsigned int size, written = 0, mask = 0, bits = 0;
    unsigned char *flags, *pairs, *raw;
    if (!src || !dst || memcmp(src, "MIO0", 4) != 0) return;
    size = read_be32(src + 4);
    pairs = src + read_be32(src + 8);
    raw = src + read_be32(src + 12);
    flags = src + 16;
    while (written < size) {
        if (!bits) { mask = *flags++; bits = 8; }
        if (mask & 0x80) { dst[written++] = *raw++; }
        else {
            unsigned int pair = ((unsigned int)pairs[0] << 8) | pairs[1];
            unsigned int count = (pair >> 12) + 3;
            unsigned int distance = (pair & 0xFFF) + 1;
            pairs += 2;
            if (distance > written || count > size - written) return;
            while (count--) { dst[written] = dst[written - distance]; ++written; }
        }
        mask <<= 1; --bits;
    }
}
