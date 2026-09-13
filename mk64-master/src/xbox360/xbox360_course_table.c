#include <ultra64.h>
#include <common_structs.h>
#include "courses/courseTable.h"
#include "xbox360/assets.h"
struct CourseTable gCourseTable[20] = {
    {x360_rom+0x8284D0, x360_rom+0x82B620, x360_rom+0x88FA10, x360_rom+0x89B510, x360_rom+0x88CD70, x360_rom+0x88D070, (CourseVtx*)0xF000000, 5757, (u8*)0xF0096F4, (u8*)0x6930, (u32*)0x9000000, 0, 0},
    {x360_rom+0x82B620, x360_rom+0x82DF40, x360_rom+0x89B510, x360_rom+0x8A7640, x360_rom+0x88D070, x360_rom+0x88D340, (CourseVtx*)0xF000000, 5560, (u8*)0xF00A0B4, (u8*)0x5AE8, (u32*)0x9000000, 1, 0},
    {x360_rom+0x82DF40, x360_rom+0x831DC0, x360_rom+0x8A7640, x360_rom+0x8B9630, x360_rom+0x88D340, x360_rom+0x88D6C0, (CourseVtx*)0xF000000, 9527, (u8*)0xF00E368, (u8*)0x9918, (u32*)0x9000000, 0, 0},
    {x360_rom+0x831DC0, x360_rom+0x835BA0, x360_rom+0x8B9630, x360_rom+0x8C2510, x360_rom+0x88D6C0, x360_rom+0x88D9C0, (CourseVtx*)0xF000000, 4945, (u8*)0xF0068E8, (u8*)0x7340, (u32*)0x9000000, 1, 0},
    {x360_rom+0x835BA0, x360_rom+0x83F740, x360_rom+0x8C2510, x360_rom+0x8CC900, x360_rom+0x88D9C0, x360_rom+0x88DAB0, (CourseVtx*)0xF000000, 3720, (u8*)0xF007D90, (u8*)0x8158, (u32*)0x9000000, 0, 0},
    {x360_rom+0x83F740, x360_rom+0x842E40, x360_rom+0x8CC900, x360_rom+0x8D8E50, x360_rom+0x88DAB0, x360_rom+0x88DB40, (CourseVtx*)0xF000000, 5529, (u8*)0xF009D24, (u8*)0x6648, (u32*)0x9000000, 0, 0},
    {x360_rom+0x842E40, x360_rom+0x84ABD0, x360_rom+0x8D8E50, x360_rom+0x8EC390, x360_rom+0x88DB40, x360_rom+0x88DC50, (CourseVtx*)0xF000000, 9376, (u8*)0xF00FD78, (u8*)0xB2B8, (u32*)0x9000000, 0, 0},
    {x360_rom+0x84ABD0, x360_rom+0x84E8E0, x360_rom+0x8EC390, x360_rom+0x8FE640, x360_rom+0x88DC50, x360_rom+0x88E120, (CourseVtx*)0xF000000, 8306, (u8*)0xF00EC60, (u8*)0xB128, (u32*)0x9000000, 0, 0},
    {x360_rom+0x84E8E0, x360_rom+0x852E20, x360_rom+0x8FE640, x360_rom+0x90B3E0, x360_rom+0x88E120, x360_rom+0x88E590, (CourseVtx*)0xF000000, 5936, (u8*)0xF009800, (u8*)0xC738, (u32*)0x9000000, 0, 0},
    {x360_rom+0x852E20, x360_rom+0x857E80, x360_rom+0x90B3E0, x360_rom+0x91B980, x360_rom+0x88E590, x360_rom+0x88E8D0, (CourseVtx*)0xF000000, 7972, (u8*)0xF00DAEC, (u8*)0x6738, (u32*)0x9000000, 0, 0},
    {x360_rom+0x857E80, x360_rom+0x8666A0, x360_rom+0x91B980, x360_rom+0x928C70, x360_rom+0x88E8D0, x360_rom+0x88ECD0, (CourseVtx*)0xF000000, 6359, (u8*)0xF00A5D0, (u8*)0x6B10, (u32*)0x9000000, 0, 0},
    {x360_rom+0x8666A0, x360_rom+0x86ECF0, x360_rom+0x928C70, x360_rom+0x936FD0, x360_rom+0x88ECD0, x360_rom+0x88EFB0, (CourseVtx*)0xF000000, 6393, (u8*)0xF00B394, (u8*)0xA678, (u32*)0x9000000, 0, 0},
    {x360_rom+0x86ECF0, x360_rom+0x872A00, x360_rom+0x936FD0, x360_rom+0x93CC60, x360_rom+0x88EFB0, x360_rom+0x88F2A0, (CourseVtx*)0xF000000, 2678, (u8*)0xF0049F8, (u8*)0x3850, (u32*)0x9000000, 0, 0},
    {x360_rom+0x872A00, x360_rom+0x8804A0, x360_rom+0x93CC60, x360_rom+0x9438C0, x360_rom+0x88F2A0, x360_rom+0x88F300, (CourseVtx*)0xF000000, 3111, (u8*)0xF005A5C, (u8*)0x2100, (u32*)0x9000000, 0, 0},
    {x360_rom+0x8804A0, x360_rom+0x885630, x360_rom+0x9438C0, x360_rom+0x951780, x360_rom+0x88F300, x360_rom+0x88F600, (CourseVtx*)0xF000000, 6067, (u8*)0xF00A9CC, (u8*)0xA4B8, (u32*)0x9000000, 0, 0},
    {x360_rom+0x885630, x360_rom+0x885780, x360_rom+0x951780, x360_rom+0x953890, x360_rom+0x88F600, x360_rom+0x88F680, (CourseVtx*)0xF000000, 1088, (u8*)0xF0018D8, (u8*)0x15D0, (u32*)0x9000000, 0, 0},
    {x360_rom+0x885780, x360_rom+0x8858A0, x360_rom+0x953890, x360_rom+0x955620, x360_rom+0x88F680, x360_rom+0x88F800, (CourseVtx*)0xF000000, 1086, (u8*)0xF001678, (u8*)0x1118, (u32*)0x9000000, 0, 0},
    {x360_rom+0x8858A0, x360_rom+0x885A10, x360_rom+0x955620, x360_rom+0x956670, x360_rom+0x88F800, x360_rom+0x88F830, (CourseVtx*)0xF000000, 555, (u8*)0xF000CD4, (u8*)0x748, (u32*)0x9000000, 0, 0},
    {x360_rom+0x885A10, x360_rom+0x88CC50, x360_rom+0x956670, x360_rom+0x963EF0, x360_rom+0x88F830, x360_rom+0x88F9C0, (CourseVtx*)0xF000000, 5679, (u8*)0xF00A45C, (u8*)0x9C20, (u32*)0x9000000, 0, 0},
    {x360_rom+0x88CC50, x360_rom+0x88CD70, x360_rom+0x963EF0, x360_rom+0x966260, x360_rom+0x88F9C0, x360_rom+0x88FA10, (CourseVtx*)0xF000000, 1165, (u8*)0xF001B84, (u8*)0x1078, (u32*)0x9000000, 0, 0},
};
