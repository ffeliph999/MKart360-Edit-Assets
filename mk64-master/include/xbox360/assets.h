#ifndef XBOX360_ASSETS_H
#define XBOX360_ASSETS_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char x360_rom[0xC00000];
extern unsigned char x360_game_heap[0x1000000];
int x360_load_local_rom(void);
/* A native pointer must not be truncated as if it were a ROM segment offset. */
static __inline uintptr_t x360_asset_dma_address(uintptr_t base,uintptr_t address) {
    return address<0x10000000U?base+(address&0xFFFFFFU):address;
}
static __inline void *x360_resolve_address(uintptr_t address,const uintptr_t *segments) {
    if(!address)return NULL;
    if(address<0x10000000U)return (void*)(segments[address>>24]+(address&0xFFFFFFU));
    /* Absolute pointers in original static display lists refer into the ROM's
     * main data image, whose RAM address differs from its file offset by C00. */
    if(address>=0x80000400U && address<0x800F7510U)return x360_rom+(address-0x80000000U)+0xC00U;
    return (void*)address;
}
#ifdef __cplusplus
}
#endif
#endif
