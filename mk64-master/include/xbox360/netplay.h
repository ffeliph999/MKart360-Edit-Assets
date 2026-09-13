#ifndef MK64_NETPLAY_H
#define MK64_NETPLAY_H
#ifdef __cplusplus
extern "C" {
#endif
int x360_net_boot_menu(void);
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
