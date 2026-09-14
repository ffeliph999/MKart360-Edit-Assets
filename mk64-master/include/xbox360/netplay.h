#ifndef MK64_NETPLAY_H
#define MK64_NETPLAY_H
#ifdef __cplusplus
extern "C" {
#endif
int x360_net_boot_menu(void);
void x360_controls_load(void);
int x360_controls_save(void);
const char *x360_control_action(int action);
const char *x360_control_binding(int player,int action);
void x360_control_bind(int player,int action,int source);
void x360_control_defaults(int player);
int x360_control_stick(int player,int change);
int x360_control_deadzone(int player,int change);
unsigned int x360_controls_down(void);
/* Local presentation setting; never part of the synchronized game state. */
int x360_display_widescreen(void);
float x360_display_aspect(void);
int x360_net_active(void);
int x360_net_player_count(void);
int x360_net_local_slot(void);
unsigned int x360_net_frame(void);
void x360_net_controllers(void *pads,int count);
unsigned int x360_net_state_hash(void);
int x360_logging_enabled(void);
void x360_set_logging(int enabled);
#ifdef __cplusplus
}
#endif
#endif
