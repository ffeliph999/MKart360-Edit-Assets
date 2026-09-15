#include "xbox360/race8.h"
#ifdef XBOX360_PORT
#include "xbox360/netplay.h"
#endif
#ifndef GCC
#define gRaceState_AS_U16
#endif
#include <ultra64.h>
#ifdef XBOX360_PORT
extern void x360_progress(int stage);
#else
#define x360_progress(stage) ((void)0)
#endif
#include <PR/os.h>
#include <PR/ucode.h>
#include <macros.h>
#include <decode.h>
#include <mk64.h>
#include <course.h>

#include "profiler.h"
#include "main.h"
#ifdef XBOX360_PORT
#include "xbox360/platform.h"
#endif
#include "racing/memory.h"
#include "menus.h"
#include <segments.h>
#include <common_structs.h>
#include <defines.h>
#include "buffers.h"
#include "camera.h"
#include "profiler.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "render_objects.h"
#include "effects.h"
#include "code_80281780.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "code_80280000.h"
#include "podium_ceremony_actors.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "profiler.h"
#include "player_controller.h"
#include "render_player.h"
#include "render_courses.h"
#include "actors.h"
#include "replays.h"
#include <debug.h>
#include "crash_screen.h"
#include "buffers/gfx_output_buffer.h"

void func_80091B78(void);
void audio_init(void);
void create_debug_thread(void);
void start_debug_thread(void);
struct SPTask* create_next_audio_frame_task(void);

struct VblankHandler* gVblankHandler1 = NULL;
struct VblankHandler* gVblankHandler2 = NULL;

struct SPTask* gActiveSPTask = NULL;
struct SPTask* sCurrentAudioSPTask = NULL;
struct SPTask* sCurrentDisplaySPTask = NULL;
struct SPTask* sNextAudioSPTask = NULL;
struct SPTask* sNextDisplaySPTask = NULL;

struct Controller gControllers[NUM_PLAYERS + 1];
struct Controller* gControllerOne = &gControllers[0];
struct Controller* gControllerTwo = &gControllers[1];
struct Controller* gControllerThree = &gControllers[2];
struct Controller* gControllerFour = &gControllers[3];
struct Controller* gControllerFive = &gControllers[8]; // All physical controllers combined.`
struct Controller* gControllerSix = &gControllers[5];
struct Controller* gControllerSeven = &gControllers[6];
struct Controller* gControllerEight = &gControllers[7];

Player gPlayers[NUM_PLAYERS];
Player* gPlayerOne = &gPlayers[0];
Player* gPlayerTwo = &gPlayers[1];
Player* gPlayerThree = &gPlayers[2];
Player* gPlayerFour = &gPlayers[3];
Player* gPlayerFive = &gPlayers[4];
Player* gPlayerSix = &gPlayers[5];
Player* gPlayerSeven = &gPlayers[6];
Player* gPlayerEight = &gPlayers[7];

Player* gPlayerOneCopy = &gPlayers[0];
Player* gPlayerTwoCopy = &gPlayers[1];
UNUSED Player* gPlayerThreeCopy = &gPlayers[2];
UNUSED Player* gPlayerFourCopy = &gPlayers[3];

UNUSED s32 D_800FD850[3];
struct GfxPool gGfxPools[2];
struct GfxPool* gGfxPool;

UNUSED s32 gfxPool_padding; // is this necessary?
struct VblankHandler gGameVblankHandler;
struct VblankHandler sSoundVblankHandler;
OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
OSMesgQueue sSoundMesgQueue;
OSMesg sSoundMesgBuf[1];
OSMesg gDmaMesgBuf[1], gGameMesgBuf;
OSMesg gGfxMesgBuf[1];
UNUSED OSMesg D_8014F010, D_8014F014;
OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
OSMesg gMainReceivedMesg;
OSIoMesg gDmaIoMesg;
OSMesgQueue gSIEventMesgQueue;
OSMesg gSIEventMesgBuf[3];

OSContStatus gControllerStatuses[4];
OSContPad gControllerPads[8];
u8 gControllerBits;
// Contains a 32x32 grid of indices into gCollisionIndices containing indices into gCollisionMesh
CollisionGrid gCollisionGrid[1024];
u16 gNumActors;
u16 gMatrixObjectCount;
s32 gTickSpeed;
f32 D_80150118;

u16 wasSoftReset;
u16 D_8015011E;

s32 D_80150120;
s32 gGotoMode;
UNUSED s32 D_80150128;
UNUSED s32 D_8015012C;
f32 gCameraZoom[8]; // look like to be the fov of each character
UNUSED s32 D_80150140;
UNUSED s32 D_80150144;
f32 gScreenAspect;
f32 gCourseFarPersp;
f32 gCourseNearPersp;
UNUSED f32 D_80150154;

struct D_80150158 gD_80150158[16];
uintptr_t gSegmentTable[16];
Gfx* gDisplayListHead;

struct SPTask* gGfxSPTask;
s32 D_801502A0;
s32 D_801502A4;
u16* gPhysicalFramebuffers[3];
uintptr_t gPhysicalZBuffer;
UNUSED u32 D_801502B8;
UNUSED u32 D_801502BC;
Mat4 D_801502C0;

s32 padding[2048];

u16 D_80152300[8];
u16 D_80152308;

UNUSED OSThread paddingThread;
OSThread gIdleThread;
ALIGNED8 u8 gIdleThreadStack[STACKSIZE]; // Based on sm64 and padding between bss symbols.
OSThread gVideoThread;
ALIGNED8 u8 gVideoThreadStack[STACKSIZE];
UNUSED OSThread D_80156820;
UNUSED ALIGNED8 u8 D_8015680_Stack[STACKSIZE];
OSThread gGameLoopThread;
ALIGNED8 u8 gGameLoopThreadStack[STACKSIZE];
OSThread gAudioThread;
ALIGNED8 u8 gAudioThreadStack[STACKSIZE];
UNUSED OSThread D_8015CD30;
UNUSED ALIGNED8 u8 D_8015CD30_Stack[STACKSIZE / 2];

ALIGNED8 u8 gGfxSPTaskYieldBuffer[4352];
ALIGNED8 u32 gGfxSPTaskStack[256];
OSMesg gPIMesgBuf[32];
OSMesgQueue gPIMesgQueue;

s32 gGamestate = 0xFFFF;
// gRaceState is externed as an s32 in other files. D_800DC514 is only used in main.c, likely a developer mistake.
u16 gRaceState = RACE_NONE;
u16 D_800DC514 = 0;
u16 creditsRenderMode = 0; // Renders the whole track. Displays red if used in normal race mode.
u16 gDemoMode = DEMO_MODE_INACTIVE;
u16 gEnableDebugMode = ENABLE_DEBUG_MODE;
s32 gGamestateNext = 7; // = COURSE_DATA_MENU?;
UNUSED s32 D_800DC528 = 1;
s32 gActiveScreenMode = SCREEN_MODE_1P;
s32 gScreenModeSelection = SCREEN_MODE_1P;
UNUSED s32 D_800DC534 = 0;
s32 gPlayerCountSelection1 = 2;

s32 gModeSelection = GRAND_PRIX;
s32 D_800DC540 = 0;
s32 D_800DC544 = 0;
s32 gCCSelection = CC_50;
s32 gGlobalTimer = 0;
UNUSED s32 D_800DC550 = 0;
UNUSED s32 D_800DC554 = 0;
UNUSED s32 D_800DC558 = 0;
// Framebuffer rendering values (max 3)
u16 sRenderedFramebuffer = 0;
u16 sRenderingFramebuffer = 0;
UNUSED u16 D_800DC564 = 0;
s32 D_800DC568 = 0;
s32 D_800DC56C[8] = { 0 };
s16 sNumVBlanks = 0;
UNUSED s16 D_800DC590 = 0;
f32 gVBlankTimer = 0.0f;
f32 gCourseTimer = 0.0f;

void create_thread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}
void isPrintfInit(void);
void main_func(void) {
#ifdef VERSION_EU
    osTvType = TV_TYPE_PAL;
#endif
    osInitialize();
#ifdef DEBUG
    isPrintfInit(); // init osSyncPrintf
#endif
    create_thread(&gIdleThread, 1, &thread1_idle, NULL, gIdleThreadStack + ARRAY_COUNT(gIdleThreadStack), 100);
    osStartThread(&gIdleThread);
}

/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(void* arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
#ifdef VERSION_EU
    osViSetMode(&osViModeTable[OS_VI_PAL_LAN1]);
#else // VERSION_US
    if (osTvType == TV_TYPE_NTSC) {
        osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    } else {
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    }
#endif
    osViBlack(true);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    wasSoftReset = (s16) osResetType;
    create_debug_thread();
    start_debug_thread();
    create_thread(&gVideoThread, 3, &thread3_video, arg, gVideoThreadStack + ARRAY_COUNT(gVideoThreadStack), 100);
    osStartThread(&gVideoThread);
    osSetThreadPri(NULL, 0);

    // Idle without consuming an Xbox hardware thread.
    while (true) {
#ifdef XBOX360_PORT
        x360_sleep_ms(1000);
#else
        ;
#endif
    }
}

void setup_mesg_queues(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSPTaskMesgQueue, gSPTaskMesgBuf, ARRAY_COUNT(gSPTaskMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);
    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
}

void start_sptask(s32 taskType) {
    if (taskType == M_AUDTASK) {
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        gActiveSPTask = sCurrentDisplaySPTask;
    }
    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the number of players
 **/
void create_gfx_task_structure(void) {
    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_boot = rspF3DBootStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    // The split-screen multiplayer racing state uses F3DLX which has a simple subpixel calculation.
    // Singleplayer race mode and all other game states use F3DEX.
    // http://n64devkit.square7.ch/n64man/ucode/gspF3DEX.htm
    if (gGamestate != RACING || gPlayerCountSelection1 == 1) {
        gGfxSPTask->task.t.ucode = gspF3DEXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DEXDataStart;
    } else {
        gGfxSPTask->task.t.ucode = gspF3DLXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DLXDataStart;
    }
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64*) &gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = (u64*) &gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    gGfxSPTask->task.t.data_ptr = (u64*) gGfxPool->gfxPool;
    gGfxSPTask->task.t.data_size = (gDisplayListHead - gGfxPool->gfxPool) * sizeof(Gfx);
    func_8008C214();
    gGfxSPTask->task.t.yield_data_ptr = (u64*) &gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
}

void init_controllers(void) {
    osCreateMesgQueue(&gSIEventMesgQueue, &gSIEventMesgBuf[0], ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, (OSMesg) 0x33333333);
    osContInit(&gSIEventMesgQueue, &gControllerBits, gControllerStatuses);
    if ((gControllerBits & 1) == 0) {
        sIsController1Unplugged = true;
    } else {
        sIsController1Unplugged = false;
    }
}

void update_controller(s32 index) {
    struct Controller* controller = &gControllers[index];
    u16 stick;

    if (sIsController1Unplugged) {
        return;
    }

    controller->rawStickX = gControllerPads[index].stick_x;
    controller->rawStickY = gControllerPads[index].stick_y;

    if ((gControllerPads[index].button & D_CBUTTONS) != 0) {
        gControllerPads[index].button |= Z_TRIG;
    }
    controller->buttonPressed = gControllerPads[index].button & (gControllerPads[index].button ^ controller->button);
    controller->buttonDepressed = controller->button & (gControllerPads[index].button ^ controller->button);
    controller->button = gControllerPads[index].button;

    stick = 0;
    if (controller->rawStickX < -50) {
        stick |= L_JPAD;
    }
    if (controller->rawStickX > 50) {
        stick |= R_JPAD;
    }
    if (controller->rawStickY < -50) {
        stick |= D_JPAD;
    }
    if (controller->rawStickY > 50) {
        stick |= U_JPAD;
    }
    controller->stickPressed = stick & (stick ^ controller->stickDirection);
    controller->stickDepressed = controller->stickDirection & (stick ^ controller->stickDirection);
    controller->stickDirection = stick;
}

void read_controllers(void) {
#ifndef XBOX360_PORT
    OSMesg msg;

    osContStartReadData(&gSIEventMesgQueue);
    osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);
    osContGetReadData(gControllerPads);
#else
    if(x360_net_active()) gVBlankTimer=(f32)x360_net_frame()/30.0f;
    x360_read_controllers(gControllerPads, x360_net8_active()?8:4);
#endif
    update_controller(0);
    update_controller(1);
    update_controller(2);
    update_controller(3);
#ifdef XBOX360_PORT
    if(x360_net8_active()){int pad;for(pad=4;pad<8;++pad)update_controller(pad);}
#endif
    gControllerFive->button = (s16) (((gControllerOne->button | gControllerTwo->button) | gControllerThree->button) |
                                     gControllerFour->button);
    gControllerFive->buttonPressed =
        (s16) (((gControllerOne->buttonPressed | gControllerTwo->buttonPressed) | gControllerThree->buttonPressed) |
               gControllerFour->buttonPressed);
    gControllerFive->buttonDepressed = (s16) (((gControllerOne->buttonDepressed | gControllerTwo->buttonDepressed) |
                                               gControllerThree->buttonDepressed) |
                                              gControllerFour->buttonDepressed);
    gControllerFive->stickDirection =
        (s16) (((gControllerOne->stickDirection | gControllerTwo->stickDirection) | gControllerThree->stickDirection) |
               gControllerFour->stickDirection);
    gControllerFive->stickPressed =
        (s16) (((gControllerOne->stickPressed | gControllerTwo->stickPressed) | gControllerThree->stickPressed) |
               gControllerFour->stickPressed);
    gControllerFive->stickDepressed =
        (s16) (((gControllerOne->stickDepressed | gControllerTwo->stickDepressed) | gControllerThree->stickDepressed) |
               gControllerFour->stickDepressed);
}

void func_80000BEC(void) {
    gPhysicalZBuffer = VIRTUAL_TO_PHYSICAL(&gZBuffer);
}

void dispatch_audio_sptask(struct SPTask* spTask) {
#ifdef XBOX360_PORT
    x360_dispatch_audio_task(spTask);
#else
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, spTask, OS_MESG_NOBLOCK);
#endif
}

void exec_display_list(struct SPTask* spTask) {
#ifdef XBOX360_PORT
    x360_exec_sp_task(spTask);
#else
    osWritebackDCacheAll();
    spTask->state = SPTASK_STATE_NOT_STARTED;
    if (sCurrentDisplaySPTask == NULL) {
        sCurrentDisplaySPTask = spTask;
        sNextDisplaySPTask = NULL;
        osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
    } else {
        sNextDisplaySPTask = spTask;
    }
#endif
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void init_rcp(void) {
    move_segment_table_to_dmem();
    init_rdp();
    set_viewport();
    select_framebuffer();
    init_z_buffer();
}

/**
 * End the master display list and initialize the graphics task structure for the next frame to be rendered.
 */
void end_master_display_list(void) {
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
    create_gfx_task_structure();
}

// clear_frame_buffer from SM64, with a few edits
//! @todo Why did void* work for matching
void* clear_framebuffer(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void rendering_init(void) {
    gGfxPool = &gGfxPools[0];
    set_segment_base_addr(1, gGfxPool);
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->gfxPool;
    init_rcp();
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gGfxPool->spTask);
    sRenderingFramebuffer++;
    gGlobalTimer++;
}

void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    gDisplayListHead = gGfxPool->gfxPool;
    gGfxSPTask = &gGfxPool->spTask;
}

/**
 * Send current master display list for rendering.
 * Tell the VI which colour framebuffer to display.
 * Yields to the VI framerate twice, locking the game at 30 FPS.
 * Selects the next framebuffer to be rendered and displayed.
 */
void display_and_vsync(void) {
#ifdef XBOX360_PORT
    profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
    exec_display_list(&gGfxPool->spTask);
    profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
    /* Native gfx_end_frame presents and paces exactly once. */
    profiler_log_thread5_time(THREAD5_END);

    if (++sRenderedFramebuffer == 3) sRenderedFramebuffer = 0;
    if (++sRenderingFramebuffer == 3) sRenderingFramebuffer = 0;
    gGlobalTimer++;
#else
    profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
    osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    exec_display_list(&gGfxPool->spTask);
    profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    osViSwapBuffer((void*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
    profiler_log_thread5_time(THREAD5_END);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    crash_screen_set_framebuffer(gPhysicalFramebuffers[sRenderedFramebuffer]);

    if (++sRenderedFramebuffer == 3) sRenderedFramebuffer = 0;
    if (++sRenderingFramebuffer == 3) sRenderingFramebuffer = 0;
    gGlobalTimer++;
#endif
}

void init_segment_ending_sequences(void) {
#ifndef XBOX360_PORT
    bzero((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osWritebackDCacheAll();
    dma_copy((u8*) SEG_ENDING, (u8*) SEG_ENDING_ROM_START, SEG_ENDING_ROM_SIZE);
    osInvalICache((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osInvalDCache((void*) SEG_ENDING, SEG_ENDING_SIZE);
#endif
}

void init_segment_racing(void) {
#ifndef XBOX360_PORT
    bzero((void*) SEG_RACING, SEG_RACING_SIZE);
    osWritebackDCacheAll();
    dma_copy((u8*) SEG_RACING, (u8*) SEG_RACING_ROM_START, SEG_RACING_ROM_SIZE);
    osInvalICache((void*) SEG_RACING, SEG_RACING_SIZE);
    osInvalDCache((void*) SEG_RACING, SEG_RACING_SIZE);
#endif
}

void dma_copy(u8* dest, u8* romAddr, size_t size) {

    osInvalDCache(dest, size);
    while (size > 0x100) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, 0x100, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
        size -= 0x100;
        romAddr += 0x100;
        dest += 0x100;
    }
    if (size != 0) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    }
}

/**
 * Setup main segments and framebuffers.
 */
void setup_game_memory(void) {
    UNUSED u32 pad[2];
    ptrdiff_t commonCourseDataSize; // Compressed mio0 size
    uintptr_t textureSegSize;
    ptrdiff_t textureSegStart;
    uintptr_t allocatedMemory;
    UNUSED s32 unknown_padding;

    init_segment_racing();
    gHeapEndPtr = SEG_RACING;
    set_segment_base_addr(0, (void*) SEG_START);

    // Memory pool size of 0xAB630
    initialize_memory_pool(MEMORY_POOL_START, MEMORY_POOL_END);

    func_80000BEC();

#ifndef XBOX360_PORT
    // Initialize trig tables segment
    osInvalDCache((void*) TRIG_TABLES, TRIG_TABLES_SIZE);
    osPiStartDma(&gDmaIoMesg, 0, 0, TRIG_TABLES_ROM_START, (void*) TRIG_TABLES, TRIG_TABLES_SIZE, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

#endif

    set_segment_base_addr(2, (void*) load_data(SEG_DATA_START, SEG_DATA_END));

    commonCourseDataSize = COMMON_TEXTURES_SIZE;
    commonCourseDataSize = ALIGN16(commonCourseDataSize);

#ifdef AVOID_UB
    textureSegStart = (ptrdiff_t) SEG_RACING - commonCourseDataSize;
#else
    textureSegStart = SEG_RACING - commonCourseDataSize;
#endif
    osPiStartDma(&gDmaIoMesg, 0, 0, COMMON_TEXTURES_ROM_START, (void*) textureSegStart, commonCourseDataSize,
                 &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    textureSegSize = *(uintptr_t*) (textureSegStart + 4);
    textureSegSize = ALIGN16(textureSegSize);
    allocatedMemory = gNextFreeMemoryAddress;
    mio0decode((u8*) textureSegStart, (u8*) allocatedMemory);
    set_segment_base_addr(0xD, (void*) allocatedMemory);

    gNextFreeMemoryAddress += textureSegSize;

    // Common course data does not get reloaded when the race state resets.
    // Therefore, only reset the memory ptr to after the common course data.
    gFreeMemoryResetAnchor = gNextFreeMemoryAddress;
}

/**
 * @brief
 *
 */
void game_init_clear_framebuffer(void) {
    gGamestateNext = 0; // = START_MENU_FROM_QUIT?
    clear_framebuffer(0);
}

void race_logic_loop(void) {
    s16 i;
    u16 rotY;

    if(x360_net8_active()) x360_race8_controls();
    gMatrixObjectCount = 0;
    gMatrixEffectCount = 0;
    if (gIsGamePaused != 0 && !x360_net8_active()) {
        func_80290B14();
    }
    if (gIsInQuitToMenuTransition != 0) {
        func_802A38B4();
        return;
    }

    if (sNumVBlanks >= 6) {
        sNumVBlanks = 5;
    }
    if (sNumVBlanks < 0) {
        sNumVBlanks = 1;
    }
    func_802A4EF4();

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            gTickSpeed = 2;
            if(!x360_net8_active()) replays_loop();
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    handle_a_press_for_all_players_during_race();
                    if(x360_net8_active()) x360_race8_cameras();
                    else func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80028F70();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            sNumVBlanks = 0;
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            D_8015F788 = 0;
            if(x360_net8_active()) x360_race8_render();
            else render_player_one_1p_screen();
            if (!gEnableDebugMode) {
                D_800DC514 = false;
            } else {
                if (D_800DC514) {

                    if ((gControllerOne->buttonPressed & R_TRIG) && (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                        D_800DC514 = false;
                    }

                    rotY = camera1->rot[1];
                    gDebugPathCount = D_800DC5EC->pathCounter;
                    if (rotY < DEGREES(45)) {
                        func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
                    } else if (rotY < DEGREES(135)) {
                        func_80057A50(40, 100, "EAST   ", gDebugPathCount);
                    } else if (rotY < DEGREES(225)) {
                        func_80057A50(40, 100, "NORTH  ", gDebugPathCount);
                    } else if (rotY < DEGREES(315)) {
                        func_80057A50(40, 100, "WEST   ", gDebugPathCount);
                    } else {
                        func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
                    }

                } else {
                    if ((gControllerOne->buttonPressed & L_TRIG) && (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                        D_800DC514 = true;
                    }
                }
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
#ifdef XBOX360_PORT
            /*
             * Xbox presents this port at a fixed 30 Hz. Always run exactly two
             * 60 Hz MK64 logic ticks per presented frame. The original N64
             * DK Jungle 2P compensation used three ticks because that mode
             * rendered slower on N64 hardware; keeping it here makes the
             * entire race simulation run at 1.5x speed.
             */
            gTickSpeed = 2;
#else
            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                gTickSpeed = 3;
            } else {
                gTickSpeed = 2;
            }
#endif
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    handle_a_press_for_all_players_during_race();
                    if(x360_net8_active()) x360_race8_cameras();
                    else func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029060();
                    func_8001EE98(gPlayerTwoCopy, camera2, 1);
                    func_80029150();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            sNumVBlanks = 0;
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_2p_screen_vertical();
                render_player_one_2p_screen_vertical();
            } else {
                render_player_one_2p_screen_vertical();
                render_player_two_2p_screen_vertical();
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:

#ifdef XBOX360_PORT
            /*
             * Match the native 30 Hz Xbox presentation clock: two 60 Hz
             * simulation ticks for every course, including DK Jungle.
             */
            gTickSpeed = 2;
#else
            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                gTickSpeed = 3;
            } else {
                gTickSpeed = 2;
            }
#endif

            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    handle_a_press_for_all_players_during_race();
                    if(x360_net8_active()) x360_race8_cameras();
                    else func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029060();
                    func_8001EE98(gPlayerTwoCopy, camera2, 1);
                    func_80029150();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            sNumVBlanks = (u16) 0;
            func_8005A070();
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_2p_screen_horizontal();
                render_player_one_2p_screen_horizontal();
            } else {
                render_player_one_2p_screen_horizontal();
                render_player_two_2p_screen_horizontal();
            }

            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
#ifdef XBOX360_PORT
            /*
             * B23C: the native Xbox renderer is explicitly paced at 30 FPS.
             * The original N64 3P/4P branches raise gTickSpeed to 3 (or 4 on
             * DK Jungle) to compensate for the lower render rate those modes
             * could sustain on N64 hardware. Keeping those compensation values
             * while rendering at a fixed 30 FPS makes simulation/race time run
             * at 1.5x (or 2x) speed. Two 1/60-second logic ticks per 30-Hz
             * frame preserves normal game speed while keeping the proven
             * 30-FPS Xbox presentation and netplay pacing unchanged.
             */
            gTickSpeed = 2;
#else
            if (gPlayerCountSelection1 == 3) {
                switch (gCurrentCourseId) {
                    case COURSE_BOWSER_CASTLE:
                    case COURSE_MOO_MOO_FARM:
                    case COURSE_SKYSCRAPER:
                    case COURSE_DK_JUNGLE:
                        gTickSpeed = 3;
                        break;
                    default:
                        gTickSpeed = 2;
                        break;
                }
            } else {
                // Four players
                switch (gCurrentCourseId) {
                    case COURSE_BLOCK_FORT:
                    case COURSE_DOUBLE_DECK:
                    case COURSE_BIG_DONUT:
                        gTickSpeed = 2;
                        break;
                    case COURSE_DK_JUNGLE:
                        gTickSpeed = 4;
                        break;
                    default:
                        gTickSpeed = 3;
                        break;
                }
            }
#endif
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    handle_a_press_for_all_players_during_race();
                    if(x360_net8_active()) x360_race8_cameras();
                    else func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029158();
                    func_8001EE98(gPlayerTwo, camera2, 1);
                    func_800291E8();
                    func_8001EE98(gPlayerThree, camera3, 2);
                    func_800291F0();
                    func_8001EE98(gPlayerFour, camera4, 3);
                    func_800291F8();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            sNumVBlanks = 0;
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_one_3p_4p_screen();
            } else if (gPlayerWinningIndex == 1) {
                render_player_one_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_two_3p_4p_screen();
            } else if (gPlayerWinningIndex == 2) {
                render_player_one_3p_4p_screen();
                render_player_two_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_three_3p_4p_screen();
            } else {
                render_player_one_3p_4p_screen();
                render_player_two_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
            }
            break;
    }

    if (!gEnableDebugMode) {
        gEnableResourceMeters = 0;
    } else {
        if (gEnableResourceMeters) {
            resource_display();
            if ((!(gControllerOne->button & L_TRIG)) && (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                gEnableResourceMeters = 0;
            }
        } else {
            if ((!(gControllerOne->button & L_TRIG)) && (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                gEnableResourceMeters = 1;
            }
        }
    }
    func_802A4300();
#ifdef XBOX360_PORT
    /*
     * MK64_ALL_ONLINE_HUD_AUDIO_PHASE_V9
     * Never select native 2P/3P/4P HUD layout from total ONLINE player count.
     * x360_race8_render_hud() maps the actual local slot(s) to a native 1P or
     * horizontal-2P presentation.
     */
    if (x360_net_active()) {
        x360_race8_render_hud();
    } else {
        func_800591B4();
    }
#else
    func_800591B4();
#endif
    func_80093E20();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
}

/**
 * mk64's game loop depends on a series of states.
 * It runs a wide branching series of code based on these states.
 * State 1) Clear framebuffer
 * State 2) Run menus
 * State 3) Process race related logic
 * State 4) Ending sequence
 * State 5) Credits
 *
 * Note that the state doesn't flip-flop at random but is permanent
 * until the state changes (ie. Exit menus and start a race).
 */

void game_state_handler(void) {
#if DVDL
    if ((gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) && (gControllerOne->button & Z_TRIG) &&
        (gControllerOne->button & A_BUTTON)) {
        gGamestateNext = CREDITS_SEQUENCE;
    } else if ((gControllerOne->button & L_TRIG) && (gControllerOne->button & R_TRIG) &&
               (gControllerOne->button & Z_TRIG) && (gControllerOne->button & B_BUTTON)) {
        gGamestateNext = ENDING;
    }
#endif

    switch (gGamestate) {
        case 7:
            game_init_clear_framebuffer();
            break;
        case START_MENU_FROM_QUIT:
        case MAIN_MENU_FROM_QUIT:
        case PLAYER_SELECT_MENU_FROM_QUIT:
        case COURSE_SELECT_MENU_FROM_QUIT:
            // Display black
            osViBlack(0);
            update_menus();
            init_rcp();
            func_80094A64(gGfxPool);
#if DVDL
            display_dvdl();
#endif
            break;
        case RACING:
            race_logic_loop();
            break;
        case ENDING:
            podium_ceremony_loop();
            break;
        case CREDITS_SEQUENCE:
            credits_loop();
            break;
    }
}

void interrupt_gfx_sptask(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void receive_new_tasks(void) {
    UNUSED s32 pad;
    struct SPTask* spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg*) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }
    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void set_vblank_handler(s32 index, struct VblankHandler* handler, OSMesgQueue* queue, OSMesg* msg) {
    handler->queue = queue;
    handler->msg = msg;
    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
    }
}

void start_gfx_sptask(void) {
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
        sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        profiler_log_gfx_time(TASKS_QUEUED);
        start_sptask(M_GFXTASK);
    }
}

void handle_vblank(void) {
    if(!x360_net_active()) gVBlankTimer += V_BlANK_TIMER_ITER;
    sNumVBlanks++;

    receive_new_tasks();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            interrupt_gfx_sptask();
        } else {
            profiler_log_vblank_time();
            start_sptask(M_AUDTASK);
        }
    } else {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
            sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            profiler_log_gfx_time(TASKS_QUEUED);
            start_sptask(M_GFXTASK);
        }
    }

/* This is where I would put my rumble code... If I had any. */
#if ENABLE_RUMBLE
    rumble_thread_update_vi();
#endif

    if (gVblankHandler1 != NULL) {
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (gVblankHandler2 != NULL) {
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void handle_dp_complete(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    profiler_log_gfx_time(RDP_COMPLETE);
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

void handle_sp_complete(void) {
    struct SPTask* curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // handle_vblank tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded((OSTask*) curSPTask) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            profiler_log_gfx_time(RSP_COMPLETE);
        }
        // Start the audio task, as expected by handle_vblank.
        profiler_log_vblank_time();
        start_sptask(M_AUDTASK);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            // After audio tasks come gfx tasks.
            profiler_log_vblank_time();
            if (sCurrentDisplaySPTask != NULL) {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
                    if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED) {
                        profiler_log_gfx_time(TASKS_QUEUED);
                    }
                    start_sptask(M_GFXTASK);
                }
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_log_gfx_time(RSP_COMPLETE);
        }
    };
}

#ifdef XBOX360_PORT
static OSMesgQueue x360AudioReadyQueue;
static OSMesg x360AudioReadyMessage[1];
#endif

void thread3_video(UNUSED void* arg0) {
    s32 i;
    u64* framebuffer1;
    OSMesg msg;
    UNUSED s32 pad[4];

    gPhysicalFramebuffers[0] = (u16*) &gFramebuffer0;
    gPhysicalFramebuffers[1] = (u16*) &gFramebuffer1;
    gPhysicalFramebuffers[2] = (u16*) &gFramebuffer2;

    // Clear framebuffer.
    framebuffer1 = (u64*) &gFramebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }
    setup_mesg_queues();
    x360_log("MK64: setting up game memory\n");
    setup_game_memory();
    x360_log("MK64: game memory ready\n");

#ifdef XBOX360_PORT
    osCreateMesgQueue(&x360AudioReadyQueue,x360AudioReadyMessage,1);
#endif
    create_thread(&gAudioThread, 4, &thread4_audio, 0, gAudioThreadStack + ARRAY_COUNT(gAudioThreadStack), 20);
    osStartThread(&gAudioThread);
#ifdef XBOX360_PORT
    /* N64 priority ordering does not serialize initialization on native cores. */
    osRecvMesg(&x360AudioReadyQueue,NULL,OS_MESG_BLOCK);
    x360_log("MK64: audio ready; starting game loop\n");
#endif

    create_thread(&gGameLoopThread, 5, &thread5_game_loop, 0, gGameLoopThreadStack + ARRAY_COUNT(gGameLoopThreadStack),
                  10);
    osStartThread(&gGameLoopThread);

    while (true) {
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_sptask();
                break;
        }
    }
}

void func_800025D4(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    set_perspective_and_aspect_ratio();
}

void func_80002600(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    set_perspective_and_aspect_ratio();
}

void func_8000262C(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    set_perspective_and_aspect_ratio();
}

void func_80002658(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    set_perspective_and_aspect_ratio();
}

/**
 * Sets courseId to NULL if
 *
 *
 */
void update_gamestate(void) {
    switch (gGamestate) {
        case START_MENU_FROM_QUIT:
            func_80002658();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case MAIN_MENU_FROM_QUIT:
            func_800025D4();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            func_80002600();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            func_8000262C();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case RACING:
            /**
             * @bug Reloading this segment makes random_u16() deterministic for player spawn order.
             * In laymens terms, random_u16() outputs the same value every time.
             */
            init_segment_racing();
            setup_race();
            break;
        case ENDING:
            gCurrentlyLoadedCourseId = COURSE_NULL;
            init_segment_ending_sequences();
            load_ceremony_cutscene();
            break;
        case CREDITS_SEQUENCE:
            gCurrentlyLoadedCourseId = COURSE_NULL;
            init_segment_racing();
            init_segment_ending_sequences();
            load_credits();
            break;
    }
}

void thread5_game_loop(UNUSED void* arg) {
    osCreateMesgQueue(&gGfxVblankQueue, gGfxMesgBuf, 1);
    osCreateMesgQueue(&gGameVblankQueue, &gGameMesgBuf, 1);
    init_controllers();
    if (!wasSoftReset) {
        clear_nmi_buffer();
    }

    set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) OS_EVENT_SW2);
    // These variables track stats such as player wins.
    // In the event of a console reset, it remembers them.
    nmi_gVersusResults2P = &pAppNmiBuffer[0]; // 2  u8's, tracks number of times player 1/2 won a VS race
    nmi_gVersusResults3P =
        &pAppNmiBuffer[2]; // 9  u8's, 3x3, tracks number of times player 1/2/3   has placed in 1st/2nd/3rd in a VS race
    nmi_gVersusResults4P = &pAppNmiBuffer[11]; // 12 u8's, 4x3, tracks number of times player 1/2/3/4 has placed in 1st/2nd/3rd
                                       // in a VS race
    gNmiUnknown4 = &pAppNmiBuffer[23]; // 2  u8's, tracking number of Battle mode wins by player 1/2
    gNmiUnknown5 = &pAppNmiBuffer[25]; // 3  u8's, tracking number of Battle mode wins by player 1/2/3
    gNmiUnknown6 = &pAppNmiBuffer[28]; // 4  u8's, tracking number of Battle mode wins by player 1/2/3/4
    rendering_init();
    x360_log("MK64: initial controller read begins\n");
    read_controllers();
    x360_log("MK64: game audio state initialization begins\n");
    func_800C5CB8();
    x360_log("MK64: game audio state ready\n");
    if(x360_net8_active()) x360_race8_prepare();

    while (true) {
        /*
         * MK64_ALL_ONLINE_HUD_AUDIO_PHASE_V9
         *
         * Offline keeps the original ordering. Online defers this game-audio
         * state update until immediately after read_controllers(), because that
         * function is the lockstep frame barrier. This prevents a faster peer
         * from advancing its next audio state before waiting for a slower
         * split-screen peer.
         */
#ifdef XBOX360_PORT
        if (!x360_net_active()) {
#endif
            if(gGlobalTimer<5)x360_log("MK64: loop audio update begins\n");
            x360_progress(1);
            func_800CB2C4();
            if(gGlobalTimer<5)x360_log("MK64: loop audio update complete\n");
#ifdef XBOX360_PORT
        }
#endif

        if(x360_net8_active()) x360_race8_service();
        // Update the gamestate if it has changed (racing, menus, credits, etc.).
        if (gGamestateNext != gGamestate) {
            gGamestate = gGamestateNext;
            if(gGlobalTimer<5)x360_log("MK64: game state transition begins\n");
            x360_progress(2);
            update_gamestate();
            if(gGlobalTimer<5)x360_log("MK64: game state transition complete\n");
        }
        profiler_log_thread5_time(THREAD5_START);
        config_gfx_pool();
        x360_progress(3);
        read_controllers();

#ifdef XBOX360_PORT
        /*
         * read_controllers() has now consumed the same complete network frame
         * on every peer. Advance the game-owned audio state from this common
         * phase point before race simulation/rendering begins.
         *
         * The separate 60 Hz audio output thread is intentionally untouched.
         */
        if (x360_net_active()) {
            if(gGlobalTimer<5)x360_log("MK64: online post-barrier audio update begins\n");
            x360_progress(1);
            func_800CB2C4();
            if(gGlobalTimer<5)x360_log("MK64: online post-barrier audio update complete\n");
        }
#endif

        if(gGlobalTimer<5)x360_log("MK64: game state handler begins\n");
        x360_progress(4);
        game_state_handler();
        if(gGlobalTimer<5)x360_log("MK64: game state handler complete\n");
        end_master_display_list();
        if(gGlobalTimer<5)x360_log("MK64: loop display list begins\n");
        x360_progress(5);
        display_and_vsync();
        x360_progress(6);
        if(gGlobalTimer<6)x360_log("MK64: loop frame complete\n");
    }
}

/**
 * Sound processing thread. Runs at 50 or 60 FPS according to osTvType.
 */
void thread4_audio(UNUSED void* arg) {
    UNUSED u32 unused[3];
    x360_log("MK64: audio initialization begins\n");
    audio_init();
    x360_log("MK64: audio initialization complete\n");
    osCreateMesgQueue(&sSoundMesgQueue, sSoundMesgBuf, ARRAY_COUNT(sSoundMesgBuf));
    set_vblank_handler(1, &sSoundVblankHandler, &sSoundMesgQueue, (OSMesg) 512);
#ifdef XBOX360_PORT
    osSendMesg(&x360AudioReadyQueue,(OSMesg)1,OS_MESG_BLOCK);
#endif

    while (true) {
        OSMesg msg;
        struct SPTask* spTask;

        osRecvMesg(&sSoundMesgQueue, &msg, OS_MESG_BLOCK);

        profiler_log_thread4_time();

        spTask = create_next_audio_frame_task();
        if (spTask != NULL) {
            dispatch_audio_sptask(spTask);
        }
        profiler_log_thread4_time();
    }
}

#ifdef XBOX360_PORT
/* Hash values, not structure padding or machine addresses. A mismatch stops play. */
static u32 net_hash_bytes(u32 h,const void *data,unsigned int size) {
    const u8 *p=(const u8*)data;while(size--)h=(h^*p++)*16777619U;return h;
}
unsigned int x360_net_state_hash(void) {
    extern u16 gRandomSeed16;
    u32 h=2166136261U;int i;int players=x360_net_player_count();
    h=net_hash_bytes(h,&gGlobalTimer,sizeof(gGlobalTimer));
    h=net_hash_bytes(h,&gGamestate,sizeof(gGamestate));
    h=net_hash_bytes(h,&gModeSelection,sizeof(gModeSelection));
    h=net_hash_bytes(h,&gRandomSeed16,sizeof(gRandomSeed16));
    if(players<2)players=2;
    if(players>8)players=8;
    for(i=0;i<players;++i){
        h=net_hash_bytes(h,&gPlayers[i].type,sizeof(gPlayers[i].type));
        h=net_hash_bytes(h,gPlayers[i].pos,sizeof(gPlayers[i].pos));
        h=net_hash_bytes(h,gPlayers[i].velocity,sizeof(gPlayers[i].velocity));
        h=net_hash_bytes(h,&gPlayers[i].lapCount,sizeof(gPlayers[i].lapCount));
        h=net_hash_bytes(h,&gPlayers[i].effects,sizeof(gPlayers[i].effects));
    }return x360_net8_active()?x360_race8_hash(h):h;
}
#endif
