#include <ultra64.h>
#include "main.h"
#include "xbox360/platform.h"
#include "xbox360/gfx_pc.h"

/*
 * MK64's SPTask points at the master F3DEX display list.
 * On Xbox we bypass the RSP and hand that command stream to the software
 * Fast3D/F3DEX interpreter.
 */
extern void x360_renderer_begin_frame(void);
extern void x360_renderer_end_frame(void);

void x360_exec_sp_task(struct SPTask *spTask) {
    if (spTask == 0) return;
    Gfx *dl = (Gfx*)spTask->task.t.data_ptr;
    static int firstFrame=1;
    if(firstFrame)x360_log("MK64: first display list begins\n");
    gfx_start_frame();
    gfx_run(dl);
    gfx_end_frame();
    if(firstFrame){firstFrame=0;x360_log("MK64: first frame presented\n");}
}
