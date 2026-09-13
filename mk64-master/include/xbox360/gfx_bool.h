#ifndef X360_GFX_BOOL_H
#define X360_GFX_BOOL_H
/* PR/ultratypes.h defines bool as signed int even in C++. All native gfx
 * translation units must use the same C++ bool ABI for callback pointers,
 * output arrays, and shared feature structures regardless of include order. */
#ifdef __cplusplus
#ifdef bool
#undef bool
#endif
#endif
#include <stdbool.h>
#endif
