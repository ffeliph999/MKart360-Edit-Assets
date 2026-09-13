#include <xtl.h>
extern "C" {
#include "xbox360/gfx_window_manager_api.h"
#include "xbox360/platform.h"
}

static bool (*g_key_down)(int);
static bool (*g_key_up)(int);
static void (*g_all_keys_up)(void);

static void w_init(const char *name,bool fs){(void)name;(void)fs;x360_platform_init();}
static void w_keys(bool(*d)(int),bool(*u)(int),void(*a)(void)){g_key_down=d;g_key_up=u;g_all_keys_up=a;}
static void w_fs_cb(void(*cb)(bool)){(void)cb;}
static void w_fs(bool e){(void)e;}
static void w_loop(void(*f)(void)){for(;;)f();}
static void w_dims(uint32_t*w,uint32_t*h){if(w)*w=1280;if(h)*h=720;}
static void w_events(void){}
static bool w_start(void){return true;}
static void w_swap_begin(void){}
static void w_swap_end(void){x360_present_and_pace();}
static double w_time(void){return x360_time_seconds();}
static void w_shutdown(void){x360_platform_shutdown();}

extern "C" struct GfxWindowManagerAPI gfx_xbox360_window_api={
    w_init,w_keys,w_fs_cb,w_fs,w_loop,w_dims,w_events,w_start,w_swap_begin,w_swap_end,w_time,w_shutdown
};
