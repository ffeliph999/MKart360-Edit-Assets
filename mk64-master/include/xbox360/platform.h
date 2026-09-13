#pragma once
/*
 * MK64 Xbox 360 platform bridge.
 * This intentionally keeps N64-facing types at the boundary so the game code
 * can remain mostly unchanged.
 */
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SPTask;
typedef struct {
    uint16_t button;
    int8_t stick_x;
    int8_t stick_y;
    uint8_t errno_;
} X360OSContPadCompat;

void x360_log(const char *message);
void x360_sleep_ms(unsigned milliseconds);
int  x360_platform_init(void);
void x360_platform_shutdown(void);
void x360_present_and_pace(void);
double x360_time_seconds(void);

void x360_read_controllers(void *pads, int count);

/* Feed an N64 display list (spTask->task.t.data_ptr) to the port renderer. */
void x360_exec_sp_task(struct SPTask *task);

/* Audio backend / HLE boundary. */
int  x360_audio_init(void);
int  x360_audio_buffered_samples(void);
int  x360_audio_desired_samples(void);
void x360_audio_submit_pcm(const void *pcm, unsigned bytes);

/* Temporary marker until the MK64 ABI/RSP audio command interpreter is wired. */
void x360_dispatch_audio_task(struct SPTask *task);

#ifdef __cplusplus
}
#endif
