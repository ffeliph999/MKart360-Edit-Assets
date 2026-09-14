#include <xtl.h>
#include <d3d9.h>
#include "xbox360/platform.h"

static IDirect3D9 *g_d3d;
static IDirect3DDevice9 *g_dev;
static LONGLONG g_next_tick;
static LARGE_INTEGER g_clock_frequency;
extern "C" int x360_video_is_black(void);

extern "C" IDirect3DDevice9 *x360_d3d_device(void) { return g_dev; }

extern "C" int x360_platform_init(void) {
    if (g_dev) return 1;
    D3DPRESENT_PARAMETERS pp;
    ZeroMemory(&pp, sizeof(pp));
    pp.BackBufferWidth  = 1280;
    pp.BackBufferHeight = 720;
    pp.BackBufferFormat = D3DFMT_A8R8G8B8;
    pp.BackBufferCount  = 1;
    pp.MultiSampleType  = D3DMULTISAMPLE_NONE;
    pp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    pp.EnableAutoDepthStencil = TRUE;
    pp.AutoDepthStencilFormat = D3DFMT_D24S8;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    g_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_d3d) return 0;

    HRESULT hr = g_d3d->CreateDevice(
        0, D3DDEVTYPE_HAL, 0,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &pp, &g_dev);
    if (FAILED(hr) || !g_dev) return 0;

    QueryPerformanceFrequency(&g_clock_frequency);
    g_next_tick = 0;
    return 1;
}

extern "C" void x360_platform_shutdown(void) {
    if (g_dev) { g_dev->Release(); g_dev = 0; }
    if (g_d3d) { g_d3d->Release(); g_d3d = 0; }
}

extern "C" double x360_time_seconds(void) {
    LARGE_INTEGER now;QueryPerformanceCounter(&now);
    if(!g_clock_frequency.QuadPart)QueryPerformanceFrequency(&g_clock_frequency);
    return (double)now.QuadPart/(double)g_clock_frequency.QuadPart;
}
extern "C" void x360_present_and_pace(void) {
    if(g_dev) {
        if(x360_video_is_black())g_dev->Clear(0,0,D3DCLEAR_TARGET,0xFF000000,1.0f,0);
        g_dev->Present(0,0,0,0);
    }
    LARGE_INTEGER now;QueryPerformanceCounter(&now);
    LONGLONG frame=g_clock_frequency.QuadPart/30;
    if(!g_next_tick || now.QuadPart-g_next_tick>frame)g_next_tick=now.QuadPart;
    g_next_tick+=frame;
    while(now.QuadPart<g_next_tick){Sleep(1);QueryPerformanceCounter(&now);}
}
