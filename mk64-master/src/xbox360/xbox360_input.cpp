#include "xbox360/netplay.h"
#include <xtl.h>
#include <string.h>
#include "xbox360/platform.h"

/*
 * Fill MK64's OSContPad-compatible array directly.
 *
 * The SM64 Xbox binary we mapped used XamInputGetState(0,1,...).
 * For source code we prefer the documented XInput API exposed by the XDK.
 */
#ifndef A_BUTTON
#define A_BUTTON 0x8000
#define B_BUTTON 0x4000
#define Z_TRIG   0x2000
#define START_BUTTON 0x1000
#define U_JPAD   0x0800
#define D_JPAD   0x0400
#define L_JPAD   0x0200
#define R_JPAD   0x0100
#define L_TRIG   0x0020
#define R_TRIG   0x0010
#define U_CBUTTONS 0x0008
#define D_CBUTTONS 0x0004
#define L_CBUTTONS 0x0002
#define R_CBUTTONS 0x0001
#endif

struct PadCompat {
    unsigned short button;
    signed char stick_x;
    signed char stick_y;
    unsigned char err_no;
};

static signed char scale_axis(SHORT v) {
    int x = (int)v;
    /* Reconstructed Xbox-port behavior: ~7849 dead zone, /256, clamp. */
    if (x > -7849 && x < 7849) x = 0;
    x /= 256;
    if (x < -128) x = -128;
    if (x > 127) x = 127;
    return (signed char)x;
}

static void convert_one(const XINPUT_STATE &s, PadCompat &p) {
    memset(&p, 0, sizeof(p));
    const XINPUT_GAMEPAD &g = s.Gamepad;

    if (g.wButtons & XINPUT_GAMEPAD_A) p.button |= A_BUTTON;
    if (g.wButtons & XINPUT_GAMEPAD_B) p.button |= B_BUTTON;
    if (g.wButtons & XINPUT_GAMEPAD_START) p.button |= START_BUTTON;
    if (g.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) p.button |= L_TRIG;
    if (g.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) p.button |= R_TRIG;

    if (g.wButtons & XINPUT_GAMEPAD_DPAD_UP) p.button |= U_JPAD;
    if (g.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) p.button |= D_JPAD;
    if (g.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) p.button |= L_JPAD;
    if (g.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) p.button |= R_JPAD;

    if (g.bLeftTrigger > 0x40) p.button |= Z_TRIG;

    /* B14a: left stick is analog only; right stick supplies N64 C buttons.
       p is cleared each poll, including when a direction is released. */
    if (g.sThumbRX > 16000) p.button |= R_CBUTTONS;
    if (g.sThumbRX < -16000) p.button |= L_CBUTTONS;
    if (g.sThumbRY > 16000) p.button |= U_CBUTTONS;
    if (g.sThumbRY < -16000) p.button |= D_CBUTTONS;

    p.stick_x = scale_axis(g.sThumbLX);
    p.stick_y = scale_axis(g.sThumbLY);
}

extern "C" void x360_read_controllers(void *pads_, int count) {
    static bool loggingChord;
    XINPUT_STATE chordState;memset(&chordState,0,sizeof(chordState));XInputGetState(0,&chordState);
    const WORD chord=XINPUT_GAMEPAD_LEFT_THUMB|XINPUT_GAMEPAD_RIGHT_THUMB;
    bool held=(chordState.Gamepad.wButtons&chord)==chord;
    if(held&&!loggingChord)x360_set_logging(!x360_logging_enabled());loggingChord=held;
    PadCompat *pads = (PadCompat*)pads_;
    for (int i = 0; i < count; ++i) {
        memset(&pads[i], 0, sizeof(PadCompat));
        XINPUT_STATE s;
        memset(&s, 0, sizeof(s));
        if (XInputGetState((DWORD)i, &s) == ERROR_SUCCESS) {
            convert_one(s, pads[i]);
        } else {
            pads[i].err_no = 1;
        }
    }
    x360_net_controllers(pads_,count);
}
