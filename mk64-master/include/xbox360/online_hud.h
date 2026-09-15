#ifndef MK64_ONLINE_HUD_H
#define MK64_ONLINE_HUD_H

/* Display-list markers: consumed when drawing, not when queuing the frame. */
#define X360_ONLINE_HUD_BEGIN 0x48445531U
#define X360_ONLINE_HUD_END   0x48445530U
#ifdef __cplusplus
extern "C" {
#endif
extern int x360_gfx_online_hud;
#ifdef __cplusplus
}
#endif
#endif
