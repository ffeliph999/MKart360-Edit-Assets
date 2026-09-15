#ifndef MK64_RACE8_H
#define MK64_RACE8_H
#include "xbox360/netplay.h"
#ifdef __cplusplus
extern "C" {
#endif
void x360_race8_prepare(void);
void x360_race8_service(void);
void x360_race8_controls(void);
void x360_race8_spawn(void);
void x360_race8_cameras(void);
void x360_race8_render(void);
int x360_race8_rules(void);
int x360_race8_hud_line(int row, char *out, int size);
unsigned int x360_race8_hash(unsigned int hash);
void x360_net8_configure(void);
int x360_net8_character(int slot);
int x360_net8_course(void);
int x360_net8_cc(void);
void x360_net8_draw_hud(void);
#ifdef __cplusplus
}
#endif
#endif
