#ifndef MATH_H
#define MATH_H

#define M_PI 3.14159265358979323846


/* MK64_X360_V15_MATH_MACRO_FIX
 * Xbox 360 XDK CRT headers may expose the float math entry points as
 * function-like macros/intrinsics. That breaks declarations such as
 * "float sinf(float);" in this compatibility header under old MSVC.
 */
#if defined(_MSC_VER)
# ifdef sinf
#  undef sinf
# endif
# ifdef cosf
#  undef cosf
# endif
# ifdef sqrtf
#  undef sqrtf
# endif
#endif

float sinf(float);
double sin(double);
float cosf(float);
double cos(double);

float sqrtf(float);

#endif
