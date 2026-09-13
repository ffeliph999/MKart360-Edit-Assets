extern "C" void x360_log(const char*);
#include <xtl.h>
#include "xbox360/netplay.h"
extern "C" {
#include "xbox360/platform.h"
#include "xbox360/assets.h"
#include "xbox360/gfx_pc.h"
#include "xbox360/gfx_rendering_api.h"
#include "xbox360/gfx_window_manager_api.h"
extern struct GfxRenderingAPI gfx_xbox360_api;
extern struct GfxWindowManagerAPI gfx_xbox360_window_api;
void main_func(void);
}
extern "C" void x360_log_start(void);
int main(void) {
    x360_log_start();x360_log("MK64: boot begins - B15c from B14c\n");
    if(!x360_load_local_rom())return 1;
    x360_log("MK64: initializing D3D\n");
    if(!x360_platform_init()){x360_log("MK64: D3D initialization failed\n");return 2;}
    x360_log("MK64: D3D ready\n");
    x360_net_boot_menu();
    if(!x360_audio_init())x360_log("MK64: audio output initialization failed\n");
    x360_log("MK64: compiling startup shaders\n");
    gfx_init(&gfx_xbox360_window_api,&gfx_xbox360_api,"Mario Kart 64",false);
    x360_log("MK64: starting native game threads\n");
    main_func();
    for(;;)Sleep(1000);
}
