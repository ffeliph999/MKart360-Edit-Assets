extern "C" void x360_log(const char*);
#include <xtl.h>
#include "xbox360/netplay.h"

/* MK360_XLIVE_INVITE_PROBE_BEGIN */
#include <xonline.h>
#include <xparty.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern "C" DWORD XexGetModuleHandle(PSZ moduleName, PHANDLE hand);
extern "C" DWORD XexGetProcedureAddress(HANDLE hand, DWORD dwOrdinal, PVOID Address);

#define MK360_PARTY_MAGIC   0x4D4B3634UL
#define MK360_PARTY_VERSION 1U
#define MK360_PARTY_HOST    0x01U

static HRESULT g_mk360_party_last_publish = (HRESULT)0x7FFFFFFF;

/* MK360_INDEPENDENT_LOGGER_MENU_V1 */
static bool g_mk360_party_logging_enabled = false;

static void mk360_party_logf(const char *fmt, ...) {
    if (!g_mk360_party_logging_enabled) return;
    char line[768];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(line, sizeof(line) - 1, fmt, ap);
    va_end(ap);
    line[sizeof(line) - 1] = 0;
    OutputDebugStringA(line);

    HANDLE f = CreateFileA("game:\\mk64-party-host.log", GENERIC_WRITE, FILE_SHARE_READ,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
    if (f != INVALID_HANDLE_VALUE) {
        SetFilePointer(f, 0, NULL, FILE_END);
        DWORD written = 0;
        WriteFile(f, line, (DWORD)strlen(line), &written, NULL);
        CloseHandle(f);
    }
}

static void mk360_party_reset_log(void) {
    if (!g_mk360_party_logging_enabled) return;
    HANDLE f = CreateFileA("game:\\mk64-party-host.log", GENERIC_WRITE, FILE_SHARE_READ,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
    if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
}

extern "C" int x360_party_logging_enabled(void) {
    return g_mk360_party_logging_enabled ? 1 : 0;
}

extern "C" void x360_party_set_logging(int enabled) {
    const bool on = enabled != 0;
    if (on == g_mk360_party_logging_enabled) return;
    g_mk360_party_logging_enabled = on;
    if (on) {
        mk360_party_reset_log();
        mk360_party_logf("PARTY LOGGING enabled\r\n");
    }
}


static PVOID mk360_party_xam_export(DWORD ordinal) {
    HANDLE xam = 0;
    if (XexGetModuleHandle((PSZ)"xam.xex", &xam) != ERROR_SUCCESS || !xam)
        return NULL;
    PVOID proc = NULL;
    if (XexGetProcedureAddress(xam, ordinal, &proc) != ERROR_SUCCESS)
        return NULL;
    return proc;
}

extern "C" HRESULT x360_party_publish_host(
    unsigned int publicIp,
    unsigned short port,
    unsigned int token)
{
    XPARTY_CUSTOM_DATA data;
    data.qwFirst =
        ((ULONGLONG)MK360_PARTY_MAGIC << 32) |
        ((ULONGLONG)MK360_PARTY_VERSION << 24) |
        ((ULONGLONG)MK360_PARTY_HOST << 16) |
        (ULONGLONG)port;
    data.qwSecond =
        ((ULONGLONG)publicIp << 32) |
        (ULONGLONG)token;

    HRESULT hr = XPartySetCustomData(0, &data);
    if (hr != g_mk360_party_last_publish) {
        mk360_party_logf(
            "PUBLISH host=%u.%u.%u.%u:%u token=%08X hr=0x%08X\r\n",
            (publicIp >> 24) & 0xFF,
            (publicIp >> 16) & 0xFF,
            (publicIp >> 8) & 0xFF,
            publicIp & 0xFF,
            (unsigned)port,
            (unsigned)token,
            (unsigned)hr);
        g_mk360_party_last_publish = hr;
    }
    return hr;
}

extern "C" void x360_party_clear_host(void) {
    XPARTY_CUSTOM_DATA data;
    ZeroMemory(&data, sizeof(data));
    HRESULT hr = XPartySetCustomData(0, &data);
    mk360_party_logf("CLEAR host custom data hr=0x%08X\r\n", (unsigned)hr);
    g_mk360_party_last_publish = (HRESULT)0x7FFFFFFF;
}

typedef DWORD (WINAPI *MK360_SHOW_PARTY_UI)(DWORD dwUserIndex);
typedef DWORD (WINAPI *MK360_SHOW_FRIENDS_UI)(DWORD dwUserIndex);

extern "C" int x360_party_is_active(void) {
    XPARTY_USER_LIST list;
    ZeroMemory(&list, sizeof(list));
    HRESULT hr = XPartyGetUserList(&list);
    return SUCCEEDED(hr) && list.dwUserCount > 0;
}

/*
 * State-aware Party social button:
 *
 * No Party:
 *   open normal Xbox Party blade so the host can choose Start LIVE Party.
 *
 * Party active:
 *   open Friends blade so the host can select a friend and choose the
 *   dashboard's Invite to Party action.
 *
 * We intentionally do not call the game-invite UI. MK360 Party custom data
 * remains the rendezvous channel; normal MK360 UDP remains the game transport.
 */
extern "C" DWORD x360_party_open_social_ui(void) {
    if (!x360_party_is_active()) {
        PVOID proc = mk360_party_xam_export(0x305); /* XamShowPartyUI, 773 */
        if (!proc) {
            mk360_party_logf("XamShowPartyUI(export 773) not found\r\n");
            return ERROR_PROC_NOT_FOUND;
        }
        MK360_SHOW_PARTY_UI fn = (MK360_SHOW_PARTY_UI)proc;
        DWORD result = fn(0);
        mk360_party_logf(
            "SOCIAL_UI no-party -> XamShowPartyUI(export 773) result=0x%08X\r\n",
            (unsigned)result);
        return result;
    }

    PVOID proc = mk360_party_xam_export(0x2BF); /* XamShowFriendsUI, 703 */
    if (!proc) {
        mk360_party_logf("XamShowFriendsUI(export 703) not found\r\n");
        return ERROR_PROC_NOT_FOUND;
    }

    MK360_SHOW_FRIENDS_UI fn = (MK360_SHOW_FRIENDS_UI)proc;
    DWORD result = fn(0);
    mk360_party_logf(
        "SOCIAL_UI party-active -> XamShowFriendsUI(export 703) result=0x%08X\r\n",
        (unsigned)result);
    return result;
}

extern "C" int x360_party_find_host(
    unsigned int *publicIp,
    unsigned short *port,
    unsigned int *token,
    char *gamerTag,
    unsigned int gamerTagSize)
{
    XPARTY_USER_LIST list;
    ZeroMemory(&list, sizeof(list));

    HRESULT hr = XPartyGetUserList(&list);
    if (FAILED(hr))
        return 0;

    for (DWORD i = 0; i < list.dwUserCount && i < XPARTY_MAX_USERS; ++i) {
        const XPARTY_USER_INFO &u = list.Users[i];
        if (u.dwFlags & XPARTY_USER_ISLOCAL)
            continue;

        ULONGLONG first = u.CustomData.qwFirst;
        ULONGLONG second = u.CustomData.qwSecond;
        DWORD magic = (DWORD)(first >> 32);
        unsigned version = (unsigned)((first >> 24) & 0xFF);
        unsigned flags = (unsigned)((first >> 16) & 0xFF);
        unsigned short p = (unsigned short)(first & 0xFFFF);
        unsigned int ip = (unsigned int)(second >> 32);
        unsigned int t = (unsigned int)(second & 0xFFFFFFFFULL);

        if (magic != MK360_PARTY_MAGIC ||
            version != MK360_PARTY_VERSION ||
            !(flags & MK360_PARTY_HOST) ||
            ip == 0 || p == 0)
            continue;

        if (publicIp) *publicIp = ip;
        if (port) *port = p;
        if (token) *token = t;

        if (gamerTag && gamerTagSize) {
            _snprintf(gamerTag, gamerTagSize - 1, "%s", u.GamerTag);
            gamerTag[gamerTagSize - 1] = 0;
        }
        return 1;
    }
    return 0;
}

static void mk360_live_invite_probe(void) {
    mk360_party_reset_log();

    XUSER_SIGNIN_STATE signin = XUserGetSigninState(0);
    XUID xuid = 0;
    DWORD xr = XUserGetXUID(0, &xuid);
    DWORD online = XOnlineStartup();

    mk360_party_logf("MK360 PARTY BRIDGE v10 PARTY-SOCIAL boot\r\n");
    mk360_party_logf(
        "signin=%u XUserGetXUID=0x%08X XUID=%08X%08X XOnlineStartup=0x%08X\r\n",
        (unsigned)signin,
        (unsigned)xr,
        (unsigned)(xuid >> 32),
        (unsigned)(xuid & 0xFFFFFFFFULL),
        (unsigned)online);
}
/* MK360_XLIVE_INVITE_PROBE_END */

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
    mk360_live_invite_probe();
    x360_net_boot_menu();
    if(!x360_audio_init())x360_log("MK64: audio output initialization failed\n");
    x360_log("MK64: compiling startup shaders\n");
    gfx_init(&gfx_xbox360_window_api,&gfx_xbox360_api,"Mario Kart 64",false);
    x360_log("MK64: starting native game threads\n");
    main_func();
    for(;;)Sleep(1000);
}
