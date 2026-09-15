#include <macros.h>
#include <defines.h>
#include <assets/common_data.h>

u16 common_tlut_finish_line_banner[] = {
	#include "assets/code/common_data/common_tlut_finish_line_banner.rgba16.inc.c"
};

u16 common_texture_particle_fire[] = {
	#include "assets/code/common_data/common_texture_particle_fire.rgba16.inc.c"
};

Vtx D_0D001200[] = {
    {{{   -80,    100,      0}, 0, {     0,    900}, {255, 255, 255, 255}}},
};

Vtx D_0D001210[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {254,   2,   0,   0}}},
};

Vtx D_0D001240[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {244, 137,   0,   0}}},
};

Vtx D_0D001270[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {231, 243,   0,   0}}},
};

Vtx D_0D0012A0[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {166, 254,  35,   0}}},
};

Vtx D_0D0012D0[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {  0, 156,  35,   0}}},
};

Vtx D_0D001300[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {  0, 148, 165,   0}}},
};

Vtx D_0D001330[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {  0,  80, 157,   0}}},
};

Vtx D_0D001360[] = {
    {{{     4,      0,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{     0,     20,      0}, 0, {     0,      0}, {  0,   0,   0,   0}}},
    {{{    -4,      0,      0}, 0, {     0,      0}, {  0,   0, 155,   0}}},
};

Vtx common_vtx_finish_line_banner[] = {
    {{{   -80,    100,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{   -40,    100,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{   -40,    115,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{   -80,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -80,     85,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{   -40,     85,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{   -40,    100,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{   -80,    100,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -40,    100,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{     0,    100,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{     0,    115,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{   -40,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -40,     85,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{     0,     85,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{     0,    100,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{   -40,    100,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     0,    100,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    40,    100,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{    40,    115,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{     0,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     0,     85,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    40,     85,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{    40,    100,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{     0,    100,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    40,    100,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    80,    100,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{    80,    115,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{    40,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    40,     85,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    80,     85,      0}, 0, {  2012,    990}, {255, 255, 255, 255}}},
    {{{    80,    100,      0}, 0, {  2012,      0}, {255, 255, 255, 255}}},
    {{{    40,    100,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
};

Vtx common_vtx_finish_post[] = {
    {{{   -92,      0,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{   -89,      0,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{   -89,    115,      0}, 0, {   990,      0}, {255, 255, 255, 255}}},
    {{{   -92,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    89,      0,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    92,      0,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{    92,    115,      0}, 0, {   990,      0}, {255, 255, 255, 255}}},
    {{{    89,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -88,    110,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{   -80,    110,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{   -80,    115,      0}, 0, {   990,    290}, {255, 255, 255, 255}}},
    {{{   -88,    115,      0}, 0, {     0,    290}, {255, 255, 255, 255}}},
    {{{   -88,     82,      0}, 0, {     0,    660}, {255, 255, 255, 255}}},
    {{{   -80,     85,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{   -80,     90,      0}, 0, {   990,    116}, {255, 255, 255, 255}}},
    {{{   -88,     87,      0}, 0, {     0,   -296}, {255, 255, 255, 255}}},
    {{{    80,    110,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{    88,    110,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    88,    115,      0}, 0, {     0,    224}, {255, 255, 255, 255}}},
    {{{    80,    115,      0}, 0, {   990,    224}, {255, 255, 255, 255}}},
    {{{    80,     85,      0}, 0, {     0,    990}, {255, 255, 255, 255}}},
    {{{    88,     82,      0}, 0, {   990,    990}, {255, 255, 255, 255}}},
    {{{    88,     87,      0}, 0, {   990,     22}, {255, 255, 255, 255}}},
    {{{    80,     90,      0}, 0, {     0,      2}, {255, 255, 255, 255}}},
};

Vtx D_0D001710[] = {
    {{{    80,     85,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -80,     85,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{   -80,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    80,    115,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
};

Gfx D_0D001750[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

Gfx D_0D001780[] = {
    gsSPVertex(D_0D001210, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D001798[] = {
    gsSPVertex(D_0D001240, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0017B0[] = {
    gsSPVertex(D_0D001270, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0017C8[] = {
    gsSPVertex(D_0D0012A0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0017E0[] = {
    gsSPVertex(D_0D0012D0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0017F8[] = {
    gsSPVertex(D_0D001300, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D001810[] = {
    gsSPVertex(D_0D001330, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D001828[] = {
    gsSPVertex(D_0D001360, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D001840[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal256(common_tlut_finish_line_banner),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x00FC, 0x007C),
    gsDPLoadTextureBlock(0x03004000, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(common_vtx_finish_line_banner, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsDPLoadTextureBlock(0x03004800, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsDPLoadTextureBlock(0x03005000, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsDPLoadTextureBlock(0x03005800, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsDPLoadTextureBlock(0x03006000, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsDPLoadTextureBlock(0x03006800, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
    gsDPLoadTextureBlock(0x03007000, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsDPLoadTextureBlock(0x03007800, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPEndDisplayList(),
};

Gfx common_model_finish_post[] = {
    gsSPClearGeometryMode(G_CULL_BACK),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, 0x03008000),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsSPVertex(common_vtx_finish_post, 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, 0x03008800),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

Gfx D_0D001B68[] = {
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPVertex(D_0D001710, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D001B90[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(D_0D001840),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPDisplayList(common_model_finish_post),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx D_0D001BD8[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPDisplayList(common_model_finish_post),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, SHADE, 0, 0, 0, TEXEL0, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(D_0D001B68),
    gsSPEndDisplayList(),
};

Gfx D_0D001C20[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(D_0D001840),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsSPDisplayList(common_model_finish_post),
    gsSPClearGeometryMode(G_FOG),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx D_0D001C88[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsSPDisplayList(common_model_finish_post),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, SHADE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsSPDisplayList(D_0D001B68),
    gsSPClearGeometryMode(G_FOG),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Vtx common_vtx_itembox[] = {
    {{{    -5,      0,     -5}, 0, {     0,      0}, {  0,   0,   0, 128}}},
    {{{    -5,      0,      5}, 0, {     0,      0}, {  0,   0,   0, 128}}},
    {{{     5,      0,      5}, 0, {     0,      0}, {  0,   0,   0, 128}}},
    {{{     5,      0,     -5}, 0, {     0,      0}, {  0,   0,   0, 128}}},
    {{{     3,     -5,      0}, 0, {  1024,   1945}, {255, 255, 255, 255}}},
    {{{     3,      5,      0}, 0, {  1024,   -102}, {255, 255, 255, 255}}},
    {{{    -3,      5,      0}, 0, {     0,   -102}, {255, 255, 255, 255}}},
    {{{    -3,     -5,      0}, 0, {     0,   1945}, {255, 255, 255, 255}}},
    {{{     5,      0,      5}, 0, {     0,      0}, {  0,   0, 255, 153}}},
    {{{     5,      0,     -5}, 0, {     0,      0}, {  0, 255, 255, 153}}},
    {{{     0,      7,      0}, 0, {     0,      0}, {  0, 255,   0, 153}}},
    {{{     5,      0,     -5}, 0, {     0,      0}, {  0, 255, 255, 153}}},
    {{{    -5,      0,     -5}, 0, {     0,      0}, {255, 130,   0, 153}}},
    {{{     0,      7,      0}, 0, {     0,      0}, {  0, 255,   0, 153}}},
    {{{    -5,      0,     -5}, 0, {     0,      0}, {255, 130,   0, 153}}},
    {{{    -5,      0,      5}, 0, {     0,      0}, {255, 255,   0, 153}}},
    {{{     0,      7,      0}, 0, {     0,      0}, {  0, 255,   0, 153}}},
    {{{    -5,      0,      5}, 0, {     0,      0}, {255, 255,   0, 153}}},
    {{{     5,      0,      5}, 0, {     0,      0}, {  0,   0, 255, 153}}},
    {{{     0,      7,      0}, 0, {     0,      0}, {  0, 255,   0, 153}}},
    {{{     5,      0,      5}, 0, {     0,      0}, {  0,   0, 255, 153}}},
    {{{    -5,      0,      5}, 0, {     0,      0}, {255, 255,   0, 153}}},
    {{{     0,     -7,      0}, 0, {     0,      0}, {255,   0,   4, 153}}},
    {{{     5,      0,     -5}, 0, {     0,      0}, {  0, 255, 255, 153}}},
    {{{     5,      0,      5}, 0, {     0,      0}, {  0,   0, 255, 153}}},
    {{{     0,     -7,      0}, 0, {     0,      0}, {255,   0,   4, 153}}},
    {{{    -5,      0,     -5}, 0, {     0,      0}, {255, 130,   0, 153}}},
    {{{     5,      0,     -5}, 0, {     0,      0}, {  0, 255, 255, 153}}},
    {{{     0,     -7,      0}, 0, {     0,      0}, {255,   0,   4, 153}}},
    {{{    -5,      0,      5}, 0, {     0,      0}, {255, 255,   0, 153}}},
    {{{    -5,      0,     -5}, 0, {     0,      0}, {255, 130,   0, 153}}},
    {{{     0,     -7,      0}, 0, {     0,      0}, {255,   0,   4, 153}}},
};

u16 common_texture_item_box_question_mark[] = {
	#include "assets/code/common_data/common_texture_item_box_question_mark.rgba16.inc.c"
};

Gfx D_0D002EE8[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 1, 1, G_OFF),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPNoOp(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(common_vtx_itembox, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Vtx common_fake_item_box_question_mark_vertices[] = {
    {{{    -3,      5,      0}, 0, {  1024,   2048}, {255, 255, 255, 255}}},
    {{{    -3,     -5,      0}, 0, {  1024,      0}, {255, 255, 255, 255}}},
    {{{     3,     -5,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     3,      5,      0}, 0, {     0,   2048}, {255, 255, 255, 255}}},
};

Gfx common_model_fake_itembox[] = {
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x00FC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, common_texture_item_box_question_mark),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 256),
    gsSPVertex(common_fake_item_box_question_mark_vertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

Gfx itemBoxQuestionMarkModel[] = {
    gsSPClearGeometryMode(G_CULL_BACK),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x00FC),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, common_texture_item_box_question_mark),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 256),
    gsSPVertex(&common_vtx_itembox[4], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

Gfx D_0D003090[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[8], 24, 0),
    gsSP1Triangle(9, 10, 11, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSP1Triangle(3, 4, 5, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(12, 13, 14, 0),
    gsSP1Triangle(15, 16, 17, 0),
    gsSP1Triangle(18, 19, 20, 0),
    gsSP1Triangle(21, 22, 23, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0030F8[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[8], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003128[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[11], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003158[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[14], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003188[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[17], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0031B8[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[20], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D0031E8[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[23], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003218[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[26], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003248[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPVertex(&common_vtx_itembox[29], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D003278[] = {
    gsSPDisplayList(D_0D003090),
    gsSPEndDisplayList(),
};

Gfx D_0D003288[] = {
    gsSPDisplayList(D_0D003278),
    gsSPEndDisplayList(),
};

Vtx common_vtx_banana[] = {
    {{{     4,     -3,      0}, 0, {  1228,   1024}, {255, 254, 254, 255}}},
    {{{     0,      4,      0}, 0, {   512,   -204}, {193, 255,   0, 255}}},
    {{{    -4,     -3,      0}, 0, {  -204,   1024}, {255, 254, 254, 255}}},
    {{{     0,     -3,      4}, 0, {  1228,   1024}, {211, 218, 173, 255}}},
    {{{     0,     -3,     -4}, 0, {  -204,   1024}, {211, 218, 173, 255}}},
};

Vtx common_vtx_flat_banana[] = {
    {{{     6,     -3,      0}, 0, {  2048,   1024}, {255, 254, 254, 255}}},
    {{{     0,      4,      0}, 0, {  1023,   -409}, {193, 255,   0, 255}}},
    {{{    -6,     -3,      0}, 0, {     0,   1024}, {255, 254, 254, 255}}},
    {{{     0,     -3,      6}, 0, {  2048,   1024}, {211, 218, 173, 255}}},
    {{{     0,      4,      0}, 0, {  1024,   -409}, {193, 255,   0, 255}}},
    {{{     0,     -3,     -6}, 0, {     0,   1024}, {211, 218, 173, 255}}},
};

u16 common_texture_banana[] = {
	#include "assets/code/common_data/common_texture_banana.rgba16.inc.c"
};

u16 common_texture_flat_banana[] = {
	#include "assets/code/common_data/common_texture_flat_banana.rgba16.inc.c"
};

Gfx common_model_banana[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, common_texture_banana),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPVertex(common_vtx_banana, 5, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(3, 1, 4, 0),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

Gfx common_model_flat_banana[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x00FC, 0x007C),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, common_texture_flat_banana),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 128),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPVertex(common_vtx_flat_banana, 6, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(3, 4, 5, 0),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPEndDisplayList(),
};

u16 common_tlut_trees_import[] = {
	#include "assets/code/common_data/common_tlut_trees_import.rgba16.inc.c"
};

u16 common_tlut_green_shell[] = {
	#include "assets/code/common_data/common_tlut_green_shell.rgba16.inc.c"
};

u16 common_tlut_blue_shell[] = {
	#include "assets/code/common_data/common_tlut_blue_shell.rgba16.inc.c"
};

Vtx common_data_seg13_vtx_5238[] = {
    {{{     3,      6,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    -3,      6,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    -3,      0,      0}, 0, {     0,   1920}, {255, 255, 255, 255}}},
    {{{     3,      0,      0}, 0, {  1984,   1920}, {255, 255, 255, 255}}},
};

Vtx common_data_seg13_vtx_5278[] = {
    {{{     3,      6,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    -3,      6,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    -3,      0,      0}, 0, {  1984,   1920}, {255, 255, 255, 255}}},
    {{{     3,      0,      0}, 0, {     0,   1920}, {255, 255, 255, 255}}},
};

Gfx D_0D0052B8[] = {
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 5, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 1, 1, G_OFF),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx D_0D005308[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D005338[] = {
    gsSPDisplayList(D_0D005308),
    gsSPVertex(common_data_seg13_vtx_5238, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx D_0D005368[] = {
    gsSPDisplayList(D_0D005308),
    gsSPVertex(common_data_seg13_vtx_5278, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx D_toads_turnpike_0D005398[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_toads_turnpike_0D0053B0[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_toads_turnpike_0D0053C8[] = {
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG | G_SHADING_SMOOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_toads_turnpike_0D0053F0[] = {
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPSetGeometryMode(G_FOG | G_SHADING_SMOOTH),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_toads_turnpike_0D005418[] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPClearGeometryMode(G_FOG),
    gsSPEndDisplayList(),
};

Vtx D_0D005430[] = {
    {{{    -2,     -2,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     1,     -2,      0}, 0, {   192,      0}, {255, 255, 255, 255}}},
    {{{     1,      1,      0}, 0, {   192,    192}, {255, 255, 255, 255}}},
    {{{    -2,      1,      0}, 0, {     0,    192}, {255, 255, 255, 255}}},
};

Vtx common_vtx_player_minimap_icon[] = {
    {{{    -4,     -4,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     3,     -4,      0}, 0, {   448,      0}, {255, 255, 255, 255}}},
    {{{     3,      3,      0}, 0, {   448,    448}, {255, 255, 255, 255}}},
    {{{    -4,      3,      0}, 0, {     0,    448}, {255, 255, 255, 255}}},
};

Vtx D_0D0054B0[] = {
    {{{    -4,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     3,     -8,      0}, 0, {   448,      0}, {255, 255, 255, 255}}},
    {{{     3,      7,      0}, 0, {   448,    960}, {255, 255, 255, 255}}},
    {{{    -4,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -4,     -8,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     3,     -8,      0}, 0, {   448,      0}, {255, 255, 128, 255}}},
    {{{     3,      7,      0}, 0, {   448,    960}, {255, 128,   0, 255}}},
    {{{    -4,      7,      0}, 0, {     0,    960}, {255, 128,   0, 255}}},
    {{{    -3,     -7,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{     4,     -7,      0}, 0, {   448,      0}, {  0,   0,   0, 255}}},
    {{{     4,      8,      0}, 0, {   448,    960}, {  0,   0,   0, 255}}},
    {{{    -3,      8,      0}, 0, {     0,    960}, {  0,   0,   0, 255}}},
    {{{    -4,    -80,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     3,    -80,      0}, 0, {   448,      0}, {255, 255, 128, 255}}},
    {{{     3,     79,      0}, 0, {   448,  10176}, {255, 128,   0, 255}}},
    {{{    -4,     79,      0}, 0, {     0,  10176}, {255, 128,   0, 255}}},
    {{{    -6,     -6,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     5,     -6,      0}, 0, {   704,      0}, {255, 255, 255, 255}}},
    {{{     5,      5,      0}, 0, {   704,    704}, {255, 255, 255, 255}}},
    {{{    -6,      5,      0}, 0, {     0,    704}, {255, 255, 255, 255}}},
    {{{    -6,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     5,     -8,      0}, 0, {   704,      0}, {255, 255, 255, 255}}},
    {{{     5,      7,      0}, 0, {   704,    960}, {255, 255, 255, 255}}},
    {{{    -6,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -6,     -8,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     5,     -8,      0}, 0, {   704,      0}, {255, 255, 128, 255}}},
    {{{     5,      7,      0}, 0, {   704,    960}, {255, 128,   0, 255}}},
    {{{    -6,      7,      0}, 0, {     0,    960}, {255, 128,   0, 255}}},
    {{{    -5,     -7,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{     6,     -7,      0}, 0, {   704,      0}, {  0,   0,   0, 255}}},
    {{{     6,      8,      0}, 0, {   704,    960}, {  0,   0,   0, 255}}},
    {{{    -5,      8,      0}, 0, {     0,    960}, {  0,   0,   0, 255}}},
    {{{    -6,    -80,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     5,    -80,      0}, 0, {   704,      0}, {255, 255, 255, 255}}},
    {{{     5,     79,      0}, 0, {   704,  10176}, {255, 255, 255, 255}}},
    {{{    -6,     79,      0}, 0, {     0,  10176}, {255, 255, 255, 255}}},
    {{{    -6,    -80,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     5,    -80,      0}, 0, {   704,      0}, {255, 255, 128, 255}}},
    {{{     5,     79,      0}, 0, {   704,  10176}, {255, 128,   0, 255}}},
    {{{    -6,     79,      0}, 0, {     0,  10176}, {255, 128,   0, 255}}},
    {{{    -6,    -96,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     5,    -96,      0}, 0, {   704,      0}, {255, 255, 128, 255}}},
    {{{     5,     95,      0}, 0, {   704,  12224}, {255, 128,   0, 255}}},
    {{{    -6,     95,      0}, 0, {     0,  12224}, {255, 128,   0, 255}}},
};

Vtx common_vtx_rectangle[] = {
    {{{    -8,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     7,     -8,      0}, 0, {   960,      0}, {255, 255, 255, 255}}},
    {{{     7,      7,      0}, 0, {   960,    960}, {255, 255, 255, 255}}},
    {{{    -8,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
};

Vtx D_0D0057B0[] = {
    {{{    -8,     -8,      0}, 0, {   960,      0}, {255, 255, 255, 255}}},
    {{{     7,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     7,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -8,      7,      0}, 0, {   960,    960}, {255, 255, 255, 255}}},
};

Vtx D_0D0057F0[] = {
    {{{    -8,      0,     -8}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     7,      0,     -8}, 0, {   960,      0}, {255, 255, 255, 255}}},
    {{{     7,      0,      7}, 0, {   960,    960}, {255, 255, 255, 255}}},
    {{{    -8,      0,      7}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -8,     -8,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     7,     -8,      0}, 0, {   960,      0}, {255, 255, 128, 255}}},
    {{{     7,      7,      0}, 0, {   960,    960}, {255, 128,   0, 255}}},
    {{{    -8,      7,      0}, 0, {     0,    960}, {255, 128,   0, 255}}},
    {{{    -6,     -6,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{     9,     -6,      0}, 0, {   960,      0}, {  0,   0,   0, 255}}},
    {{{     9,      9,      0}, 0, {   960,    960}, {  0,   0,   0, 255}}},
    {{{    -6,      9,      0}, 0, {     0,    960}, {  0,   0,   0, 255}}},
    {{{     0,     -8,      0}, 0, {   448,      0}, {  0,   0,   0, 255}}},
    {{{     8,      8,      0}, 0, {   960,    960}, {  0,   0,   0, 255}}},
    {{{    -8,      8,      0}, 0, {     0,    960}, {  0,   0,   0, 255}}},
};

Vtx common_data_seg13_vtx_58E0[] = {
    {{{    -8,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     7,     -8,      0}, 0, {   960,      0}, {255, 255, 255, 255}}},
    {{{     7,      7,      0}, 0, {   960,    960}, {255, 255, 255, 255}}},
    {{{    -8,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
};

Vtx D_0D005920[] = {
    {{{    -4,    -32,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     3,    -32,      0}, 0, {   960,      0}, {255, 255, 255, 255}}},
    {{{     3,     31,      0}, 0, {   960,    960}, {255, 255, 255, 255}}},
    {{{    -4,     31,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -8,    -80,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{     7,    -80,      0}, 0, {   960,      0}, {255, 255, 128, 255}}},
    {{{     7,     79,      0}, 0, {   960,  10176}, {255, 128,   0, 255}}},
    {{{    -8,     79,      0}, 0, {     0,  10176}, {255, 128,   0, 255}}},
    {{{   -10,    -10,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     9,    -10,      0}, 0, {  1216,      0}, {255, 255, 255, 255}}},
    {{{     9,      9,      0}, 0, {  1216,   1216}, {255, 255, 255, 255}}},
    {{{   -10,      9,      0}, 0, {     0,   1216}, {255, 255, 255, 255}}},
    {{{   -12,    -12,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    11,    -12,      0}, 0, {  1472,      0}, {255, 255, 255, 255}}},
    {{{    11,     11,      0}, 0, {  1472,   1472}, {255, 255, 255, 255}}},
    {{{   -12,     11,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -12,    -24,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    11,    -24,      0}, 0, {  1472,      0}, {255, 255, 255, 255}}},
    {{{    11,     23,      0}, 0, {  1472,   3008}, {255, 255, 255, 255}}},
    {{{   -12,     23,      0}, 0, {     0,   3008}, {255, 255, 255, 255}}},
    {{{   -14,    -14,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    13,    -14,      0}, 0, {  1728,      0}, {255, 255, 255, 255}}},
    {{{    13,     13,      0}, 0, {  1728,   1728}, {255, 255, 255, 255}}},
    {{{   -14,     13,      0}, 0, {     0,   1728}, {255, 255, 255, 255}}},
};

Vtx D_0D005AA0[] = {
    {{{   -16,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    15,     -8,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    15,      7,      0}, 0, {  1984,    960}, {255, 255, 255, 255}}},
    {{{   -16,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
};

Vtx D_0D005AE0[] = {
    {{{   -16,    -16,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    15,    -16,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    15,     15,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{   -16,     15,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D005B20[] = {
    {{{   -16,    -16,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    15,    -16,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    15,     15,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -16,     15,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D005B60[] = {
    {{{   -16,      0,    -16}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    15,      0,    -16}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    15,      0,     15}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{   -16,      0,     15}, 0, {     0,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D005BA0[] = {
    {{{     0,     18,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    16,     -9,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{   -16,     -9,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D005BD0[] = {
    {{{     0,     18,      0}, 0, {     0,      0}, {  0,   0, 120, 255}}},
    {{{    16,     -9,      0}, 0, {  1984,      0}, {  0,   0, 120, 255}}},
    {{{   -16,     -9,      0}, 0, {  1984,   1984}, {  0,   0, 120, 255}}},
};

Vtx D_0D005C00[] = {
    {{{     0,     18,      0}, 0, {     0,      0}, {213,  44, 102, 255}}},
    {{{    16,     -9,      0}, 0, {  1984,      0}, {102, 211,  43, 255}}},
    {{{   -16,     -9,      0}, 0, {  1984,   1984}, { 42, 153, 214, 255}}},
};

Vtx D_0D005C30[] = {
    {{{   -20,    -15,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    19,    -15,      0}, 0, {  2496,      0}, {255, 255, 255, 255}}},
    {{{    19,     16,      0}, 0, {  2496,   1984}, {255, 255, 255, 255}}},
    {{{   -20,     16,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -20,    -19,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    19,    -19,      0}, 0, {  2496,      0}, {255, 255, 255, 255}}},
    {{{    19,     20,      0}, 0, {  2496,   2496}, {255, 255, 255, 255}}},
    {{{   -20,     20,      0}, 0, {     0,   2496}, {255, 255, 255, 255}}},
    {{{   -24,     -8,      0}, 0, {     0,      0}, {255, 255, 128, 255}}},
    {{{    23,     -8,      0}, 0, {  3008,      0}, {255, 255, 128, 255}}},
    {{{    23,      7,      0}, 0, {  3008,    960}, {255, 128,   0, 255}}},
    {{{   -24,      7,      0}, 0, {     0,    960}, {255, 128,   0, 255}}},
    {{{   -24,     -8,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    23,     -8,      0}, 0, {  3008,      0}, {  0,   0,   0, 255}}},
    {{{    23,      7,      0}, 0, {  3008,    960}, {  0,   0,   0, 255}}},
    {{{   -24,      7,      0}, 0, {     0,    960}, {  0,   0,   0, 255}}},
    {{{   -24,    -19,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    23,    -19,      0}, 0, {  3008,      0}, {255, 255, 255, 255}}},
    {{{    23,     20,      0}, 0, {  3008,   2496}, {255, 255, 255, 255}}},
    {{{   -24,     20,      0}, 0, {     0,   2496}, {255, 255, 255, 255}}},
    {{{   -24,    -19,      0}, 0, {  3008,      0}, {255, 255, 255, 255}}},
    {{{    23,    -19,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    23,     20,      0}, 0, {     0,   2496}, {255, 255, 255, 255}}},
    {{{   -24,     20,      0}, 0, {  3008,   2496}, {255, 255, 255, 255}}},
    {{{   -24,    -19,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    23,    -19,      0}, 0, {  3008,      0}, {  0,   0,   0, 255}}},
    {{{    23,      0,      0}, 0, {  3008,   1216}, {  0,   0,   0, 255}}},
    {{{   -24,      0,      0}, 0, {     0,   1216}, {  0,   0,   0, 255}}},
    {{{   -24,      0,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    23,      0,      0}, 0, {  3008,      0}, {  0,   0,   0, 255}}},
    {{{    23,     19,      0}, 0, {  3008,   1216}, {  0,   0,   0, 255}}},
    {{{   -24,     19,      0}, 0, {     0,   1216}, {  0,   0,   0, 255}}},
    {{{   -24,    -23,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    23,    -23,      0}, 0, {  3008,      0}, {255, 255, 255, 255}}},
    {{{    23,      0,      0}, 0, {  3008,   1472}, {255, 255, 255, 255}}},
    {{{   -24,      0,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -24,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
};

Vtx D_0D005E80[] = {
    {{{    23,      0,      0}, 0, {  3008,      0}, {255, 255, 255, 255}}},
    {{{    23,     23,      0}, 0, {  3008,   1472}, {255, 255, 255, 255}}},
    {{{   -24,     23,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
};

Vtx common_vtx_lakitu[] = {
    {{{   -28,    -35,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    27,    -35,      0}, 0, {  3520,      0}, {255, 255, 255, 255}}},
    {{{    27,      0,      0}, 0, {  3520,   2240}, {255, 255, 255, 255}}},
    {{{   -28,      0,      0}, 0, {     0,   2240}, {255, 255, 255, 255}}},
    {{{   -28,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    27,      0,      0}, 0, {  3520,      0}, {255, 255, 255, 255}}},
    {{{    27,     35,      0}, 0, {  3520,   2240}, {255, 255, 255, 255}}},
    {{{   -28,     35,      0}, 0, {     0,   2240}, {255, 255, 255, 255}}},
};

Vtx D_0D005F30[] = {
    {{{   -10,    -35,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    45,    -35,      0}, 0, {  3520,      0}, {255, 255, 255, 255}}},
    {{{    45,      0,      0}, 0, {  3520,   2240}, {255, 255, 255, 255}}},
    {{{   -10,      0,      0}, 0, {     0,   2240}, {255, 255, 255, 255}}},
    {{{   -10,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    45,      0,      0}, 0, {  3520,      0}, {255, 255, 255, 255}}},
    {{{    45,     35,      0}, 0, {  3520,   2240}, {255, 255, 255, 255}}},
    {{{   -10,     35,      0}, 0, {     0,   2240}, {255, 255, 255, 255}}},
};

Vtx D_0D005FB0[] = {
    {{{   -32,    -16,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -16,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     15,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     15,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D005FF0[] = {
    {{{   -53,    -16,      0}, 0, {     0,      0}, {255,   0,   0, 255}}},
    {{{    10,    -16,      0}, 0, {  4032,      0}, {255,   0,   0, 255}}},
    {{{    10,     15,      0}, 0, {  4032,   1984}, {255,   0,   0, 255}}},
    {{{   -53,     15,      0}, 0, {     0,   1984}, {255,   0,   0, 255}}},
};

Vtx D_0D006030[] = {
    {{{   -32,    -32,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -32,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,      0}, 0, {  4032,   4032}, {255, 255, 255, 255}}},
    {{{   -32,     31,      0}, 0, {     0,   4032}, {255, 255, 255, 255}}},
    {{{   -32,    -32,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    31,    -32,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    31,     31,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{   -32,     31,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
};

Vtx common_vtx_hedgehog[] = {
    {{{   -32,    -31,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     31,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D006130[] = {
    {{{   -32,    -31,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     31,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D0061B0[] = {
    {{{   -32,    -31,    -12}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,    -12}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,    -12}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,    -12}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,    -12}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,    -12}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,    -12}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     31,    -12}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,    -31,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,    -31,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     31,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D0062B0[] = {
    {{{   -32,    -32,     20}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -32,     20}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     31,     20}, 0, {  4032,   4032}, {255, 255, 255, 255}}},
    {{{   -32,     31,     20}, 0, {     0,   4032}, {255, 255, 255, 255}}},
    {{{   -31,    -32,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     0,    -32,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{     0,     31,      0}, 0, {  1984,   4032}, {255, 255, 255, 255}}},
    {{{   -31,     31,      0}, 0, {     0,   4032}, {255, 255, 255, 255}}},
    {{{     1,    -32,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    32,    -32,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{    32,     31,      0}, 0, {  1984,   4032}, {255, 255, 255, 255}}},
    {{{     1,     31,      0}, 0, {     0,   4032}, {255, 255, 255, 255}}},
    {{{   -32,    -31,      0}, 0, {    64,     64}, {255, 255, 255, 255}}},
    {{{    31,    -31,      0}, 0, {  4096,     64}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {  4096,   2048}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {    64,   2048}, {255, 255, 255, 255}}},
    {{{   -32,      1,      0}, 0, {    64,     64}, {255, 255, 255, 255}}},
    {{{    31,      1,      0}, 0, {  4096,     64}, {255, 255, 255, 255}}},
    {{{    31,     32,      0}, 0, {  4096,   2048}, {255, 255, 255, 255}}},
    {{{   -32,     32,      0}, 0, {    64,   2048}, {255, 255, 255, 255}}},
    {{{   -32,    -48,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -48,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,    -16,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,    -16,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,    -16,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -16,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     15,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     15,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     15,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,     15,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     47,      0}, 0, {  4032,   1984}, {255, 255, 255, 255}}},
    {{{   -32,     47,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
};

Vtx D_0D0064B0[] = {
    {{{   -32,    -47,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,    -47,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {  4032,   3008}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {     0,   3008}, {255, 255, 255, 255}}},
    {{{   -32,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    31,      0,      0}, 0, {  4032,      0}, {255, 255, 255, 255}}},
    {{{    31,     47,      0}, 0, {  4032,   3008}, {255, 255, 255, 255}}},
    {{{   -32,     47,      0}, 0, {     0,   3008}, {255, 255, 255, 255}}},
    {{{   -36,    -23,      0}, 0, {     0,     32}, {255, 255, 255, 255}}},
    {{{    35,    -23,      0}, 0, {  4544,     32}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,   1504}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,   1504}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,     32}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,     32}, {255, 255, 255, 255}}},
    {{{    35,     23,      0}, 0, {  4544,   1504}, {255, 255, 255, 255}}},
    {{{   -36,     23,      0}, 0, {     0,   1504}, {255, 255, 255, 255}}},
    {{{   -36,    -23,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    35,    -23,      0}, 0, {  4544,      0}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,   1472}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,      0}, {255, 255, 255, 255}}},
    {{{    35,     23,      0}, 0, {  4544,   1472}, {255, 255, 255, 255}}},
    {{{   -36,     23,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -36,    -23,      0}, 0, {     0,     16}, {255, 255, 255, 255}}},
    {{{    35,    -23,      0}, 0, {  4544,     16}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,   1488}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,   1488}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,     16}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,     16}, {255, 255, 255, 255}}},
    {{{    35,     23,      0}, 0, {  4544,   1488}, {255, 255, 255, 255}}},
    {{{   -36,     23,      0}, 0, {     0,   1488}, {255, 255, 255, 255}}},
    {{{   -36,    -23,      0}, 0, {     0,     32}, {255, 255, 255, 255}}},
    {{{    35,    -23,      0}, 0, {  4544,     32}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,   1504}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,   1504}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,     32}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,     32}, {255, 255, 255, 255}}},
    {{{    35,     23,      0}, 0, {  4544,   1504}, {255, 255, 255, 255}}},
    {{{   -36,     23,      0}, 0, {     0,   1504}, {255, 255, 255, 255}}},
};

Vtx common_vtx_also_lakitu[] = {
    {{{   -36,    -27,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    35,    -27,      0}, 0, {  4544,      0}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,   1728}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,   1728}, {255, 255, 255, 255}}},
    {{{   -36,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    35,      0,      0}, 0, {  4544,      0}, {255, 255, 255, 255}}},
    {{{    35,     27,      0}, 0, {  4544,   1728}, {255, 255, 255, 255}}},
    {{{   -36,     27,      0}, 0, {     0,   1728}, {255, 255, 255, 255}}},
    {{{   -40,    -24,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    40,    -24,      0}, 0, {  5056,      0}, {255, 255, 255, 255}}},
    {{{    40,      0,      0}, 0, {  5056,   1472}, {255, 255, 255, 255}}},
    {{{   -40,      0,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -40,      0,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    40,      0,      0}, 0, {  5056,      0}, {255, 255, 255, 255}}},
    {{{    40,     24,      0}, 0, {  5056,   1472}, {255, 255, 255, 255}}},
    {{{   -40,     24,      0}, 0, {     0,   1472}, {255, 255, 255, 255}}},
    {{{   -48,     -8,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    47,     -8,      0}, 0, {  6080,      0}, {255, 255, 255, 255}}},
    {{{    47,      7,      0}, 0, {  6080,    960}, {255, 255, 255, 255}}},
    {{{   -48,      7,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{   -56,    -16,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    55,    -16,      0}, 0, {  7104,      0}, {255, 255, 255, 255}}},
    {{{    55,     15,      0}, 0, {  7104,   1984}, {255, 255, 255, 255}}},
    {{{   -56,     15,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{   -56,    -16,      0}, 0, {     0,      0}, {  0,   0,   0, 255}}},
    {{{    55,    -16,      0}, 0, {  7104,      0}, {  0,   0,   0, 255}}},
    {{{    55,     15,      0}, 0, {  7104,   1984}, {  0,   0,   0, 255}}},
    {{{   -56,     15,      0}, 0, {     0,   1984}, {  0,   0,   0, 255}}},
};

Vtx D_0D0068F0[] = {
    {{{   -64,    -32,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{    63,    -32,      0}, 0, {  8128,      0}, {255, 255, 255, 255}}},
    {{{    63,     31,      0}, 0, {  8128,   4032}, {255, 255, 255, 255}}},
    {{{   -64,     31,      0}, 0, {     0,   4032}, {255, 255, 255, 255}}},
};

Gfx D_0D006930[] = {
    gsSP1Triangle(0, 2, 1, 0),
    gsSPEndDisplayList(),
};

Gfx common_rectangle_display[] = {
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D006950[] = {
    gsSPVertex(common_vtx_player_minimap_icon, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006968[] = {
    gsSPVertex(D_0D0054B0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006980[] = {
    gsSPVertex(common_vtx_rectangle, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006998[] = {
    gsSPVertex(D_0D0057B0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D0069B0[] = {
    gsSPVertex(D_0D0057F0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D0069C8[] = {
    gsSPVertex(D_0D005AA0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D0069E0[] = {
    gsSPVertex(D_0D005AE0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D0069F8[] = {
    gsSPVertex(D_0D005B20, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006A10[] = {
    gsSPVertex(D_0D005B60, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006A28[] = {
    gsSPVertex(common_data_seg13_vtx_58E0, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

Gfx D_0D006A40[] = {
    gsSPVertex(D_0D005920, 4, 0),
    gsSPDisplayList(common_rectangle_display),
    gsSPEndDisplayList(),
};

u8 common_shadow_i4[] = {
	#include "assets/code/common_data/common_shadow_i4.i4.inc.c"
};

u8 D_0D006AD8[] = {
	#include "assets/code/common_data/D_0D006AD8.i4.inc.c"
};

u16 common_tlut_debug_font[] = {
	#include "assets/code/common_data/common_tlut_debug_font.tlut.inc.c"
};

u16 common_texture_debug_font[] = {
	#include "assets/code/common_data/common_texture_debug_font.tlut.inc.c"
};

Gfx D_0D0076F8[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPNoOp(),
    gsDPSetColorDither(G_CD_DISABLE),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx D_0D007780[] = {
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D0077A0[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx D_0D0077D0[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D0077F8[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_LIGHTING),
    gsSPClearGeometryMode(G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D007828[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D007850[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D007878[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D0078A0[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx D_0D0078D0[] = {
    gsSPDisplayList(D_0D007780),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx D_0D0078F8[] = {
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx D_0D007928[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx D_0D007948[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007968[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx D_0D007988[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx D_0D0079A8[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D0079C8[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D0079E8[] = {
    gsSPDisplayList(D_0D0078F8),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007A08[] = {
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx D_0D007A40[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007A60[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007A80[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007AA0[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007AC0[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007AE0[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007B00[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007B20[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetPrimColor(0, 0, 0x14, 0x14, 0x14, 0x00),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0),
    gsDPLoadTextureBlock_4b(common_shadow_i4, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(D_0D006980),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

Gfx D_0D007B98[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetPrimColor(0, 0, 0x14, 0x14, 0x14, 0x00),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0),
    gsDPLoadTextureBlock_4b(common_shadow_i4, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(D_0D0069B0),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

Gfx D_0D007C10[] = {
    gsSPDisplayList(D_0D007A08),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetPrimColor(0, 0, 0x1E, 0x0A, 0x00, 0xC8),
    gsDPSetCombineLERP(TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0),
    gsDPLoadTextureBlock(D_0D006AD8, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(D_0D006A10),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

Gfx D_0D007C88[] = {
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx D_0D007CB8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007CD8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007CF8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_AVERAGE),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007D18[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx D_0D007D38[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007D58[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007D78[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007D98[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D007DB8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007DD8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007DF8[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007E18[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007E38[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007E58[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007E78[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsSPEndDisplayList(),
};

Gfx D_0D007E98[] = {
    gsSPDisplayList(D_0D007C88),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsSPEndDisplayList(),
};

Gfx D_0D007EB8[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D007ED8[] = {
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPEndDisplayList(),
};

Gfx D_0D007EF8[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx D_0D007F18[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007F38[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D007F58[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007F78[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007F98[] = {
    gsSPDisplayList(D_0D007ED8),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPEndDisplayList(),
};

Gfx D_0D007FB8[] = {
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D007FE0[] = {
    gsSPDisplayList(D_0D007FB8),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D008000[] = {
    gsSPDisplayList(D_0D007FB8),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPEndDisplayList(),
};

Gfx D_0D008020[] = {
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsSPEndDisplayList(),
};

Gfx D_0D008040[] = {
    gsSPDisplayList(D_0D008020),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D008060[] = {
    gsSPDisplayList(D_0D008020),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
    gsSPEndDisplayList(),
};

Gfx D_0D008080[] = {
    gsSPDisplayList(D_0D007EF8),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(0, common_tlut_debug_font),
    gsDPLoadSync(),
    gsDPLoadTextureBlock_4b(common_texture_debug_font, G_IM_FMT_CI, 128, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx D_0D008108[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_COPY),
    gsSPEndDisplayList(),
};

Gfx D_0D008120[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};

Gfx D_0D008138[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsSPEndDisplayList(),
};

u8 common_grand_prix_human_item_curve[][100] = {
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
};

u8 common_grand_prix_cpu_item_curve[][100] = {
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
};

u8 common_versus_2_player_item_curve[][100] = {
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BOO, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH,
        ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BOO, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_BOO, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_BOO, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_BOO,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_BANANA_BUNCH, ITEM_TRIPLE_GREEN_SHELL, ITEM_RED_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_SUPER_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_RED_SHELL, ITEM_BANANA_BUNCH, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_RED_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_BANANA_BUNCH, ITEM_BLUE_SPINY_SHELL, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_BANANA_BUNCH, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_RED_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_RED_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_BANANA_BUNCH,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
};

u8 common_versus_3_player_item_curve[][100] = {
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
};

u8 common_versus_4_player_item_curve[][100] = {
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
    },
    {
        ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH,
        ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
        ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
    },
    {
        ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
        ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM, ITEM_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
    {
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL, ITEM_TRIPLE_RED_SHELL,
        ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL, ITEM_BLUE_SPINY_SHELL,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT,
        ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_THUNDERBOLT, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
        ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM, ITEM_TRIPLE_MUSHROOM,
        ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM, ITEM_SUPER_MUSHROOM,
    },
};

u8 common_battle_item_curve[][100] = {
    {
    ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA, ITEM_BANANA,
    ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_BANANA_BUNCH, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL, ITEM_GREEN_SHELL,
    ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
    ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL, ITEM_TRIPLE_GREEN_SHELL,
    ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
    ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL, ITEM_RED_SHELL,
    ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX,
    ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_FAKE_ITEM_BOX, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
    ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR,
    ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_STAR, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO, ITEM_BOO,
    },
};

Vtx D_0D008B78[] = {
    {{{     2,      2,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     2,     -2,      0}, 0, {     0,    960}, {255, 255, 255, 255}}},
    {{{    -2,     -2,      0}, 0, {   960,    960}, {255, 255, 255, 255}}},
    {{{    -2,      2,      0}, 0, {   960,      0}, {255, 255, 255, 255}}},
};

Vtx D_0D008BB8[] = {
    {{{     2,      4,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     2,      0,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{    -2,      0,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{    -2,      4,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
};

Vtx D_0D008BF8[] = {
    {{{     2,      2,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     2,     -2,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{    -2,     -2,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{    -2,      2,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
};

Vtx D_0D008C38[] = {
    {{{     2,      2,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
    {{{     2,     -2,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{    -2,     -2,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{    -2,      2,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
};

Gfx common_square_plain_render[] = {
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D008C90[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPEndDisplayList(),
};

Gfx common_setting_render_character[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPEndDisplayList(),
};

Gfx D_0D008D10[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPEndDisplayList(),
};

Gfx D_0D008D58[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPNoOp(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
    gsSPEndDisplayList(),
};

Gfx D_0D008DA0[] = {
    gsSPDisplayList(common_square_plain_render),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

Gfx D_0D008DB8[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPNoOp(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D008DF8[] = {
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(D_0D008B78, 4, 0),
    gsSPDisplayList(D_0D008DA0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D008E20[] = {
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(D_0D008BB8, 4, 0),
    gsSPDisplayList(D_0D008DA0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D008E48[] = {
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(D_0D008BF8, 4, 0),
    gsSPDisplayList(D_0D008DA0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Gfx D_0D008E70[] = {
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(D_0D008C38, 4, 0),
    gsSPDisplayList(D_0D008DA0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};

Mtx D_0D008E98 = {
    toFixedPointMatrix(1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0),
};

// WARNING: Overlap detected between 0x8f18 and 0x8ed8 with size 0x40
Mtx D_0D008ED8 = {
    toFixedPointMatrix(0.0, 410.0, 0.0, 0.0,
                       0.999985, 0.000046, 0.0, 0.0,
                       0.999985, 0.000046, 0.999985, 546.000061,
                       0.999985, 0.000046, 0.0000153, 0.0),
};

// WARNING: Overlap detected between 0x8f58 and 0x8f18 with size 0x40
CPUBehaviour D_0D008F18[] = {
    {     1,      3,      2},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D008F28[] = {
    {     1,      2,      6},
    {    11,     30,      1},
    {    55,     74,      1},
    {    90,    105,      1},
    {   139,    155,      1},
    {   177,    205,      1},
    {   225,    242,      1},
    {   292,    313,      1},
    {   352,    373,      1},
    {   452,    465,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D008F80[] = {
    {     1,      2,      6},
    {    10,     25,      1},
    {   190,    210,      1},
    {   270,    290,      1},
    {   410,    440,      1},
    {   540,    550,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D008FB8[] = {
    {     1,      2,      6},
    {    60,     80,      1},
    {   125,    140,      1},
    {   170,    185,      1},
    {   230,    240,      1},
    {   275,    285,      1},
    {   310,    320,      1},
    {   321,    349,      3},
    {   350,    360,      1},
    {   385,    415,      1},
    {   450,    468,      1},
    {   470,    477,      9},
    {   480,    485,     11},
    {   543,    546,      9},
    {   548,    550,     11},
    {   565,    568,      2},
    {   630,    631,      6},
    {   635,    640,     10},
    {   645,    655,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009058[] = {
    {     1,      2,      6},
    {    15,     30,      1},
    {    55,     65,      1},
    {   125,    150,      1},
    {   265,    270,      1},
    {   275,    285,      1},
    {   305,    320,      2},
    {   330,    340,      1},
    {   375,    385,      1},
    {   547,    570,      1},
    {   582,    600,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D0090B8[] = {
    {     1,      2,      6},
    {    70,     94,      1},
    {   120,    133,      1},
    {   150,    170,      1},
    {   249,    265,      1},
    {   360,    395,      1},
    {   635,    655,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D0090F8[] = {
    {     1,      2,      6},
    {    15,     30,      1},
    {    41,     63,      1},
    {   115,    155,      1},
    {   200,    215,      1},
    {   240,    241,      9},
    {   264,    265,     10},
    {   270,    290,      1},
    {   345,    375,      1},
    {   493,    544,      1},
    {   583,    605,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009158[] = {
    {     1,      2,      6},
    {    75,    100,      1},
    {   135,    150,      1},
    {   355,    390,      1},
    {   505,    525,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009188[] = {
    {     1,      2,      6},
    {    65,     80,      1},
    {   165,    166,      9},
    {   180,    210,      1},
    {   220,    221,     10},
    {   250,    275,      1},
    {   360,    380,      1},
    {   440,    480,      1},
    {   600,    601,      9},
    {   689,    690,     10},
    {   695,    725,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D0091E8[] = {
    {     1,      2,      6},
    {   128,    275,      1},
    {   320,    345,      1},
    {   465,    565,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009210[] = {
    {     1,      2,      6},
    {    75,    100,      1},
    {   175,    210,      1},
    {   275,    300,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009238[] = {
    {     1,      2,      6},
    {   106,    188,      1},
    {   220,    386,      1},
    {   583,    765,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009260[] = {
    {     1,      2,      6},
    {   245,    262,      1},
    {   585,    606,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009280[] = {
    {     1,      2,      6},
    {     7,     38,      1},
    {    36,     54,      1},
    {   129,    150,      1},
    {   380,    410,      1},
    {   425,    445,      1},
    {   456,    500,      1},
    {   594,    625,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D0092C8[] = {
    {     1,      2,      6},
    {     4,      5,      9},
    {   129,    130,     10},
    {   555,    560,      9},
    {   827,    832,     10},
    {   810,    845,      1},
    {   910,    993,      1},
    {  1390,   1600,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D009310[] = {
    {     1,      2,      6},
    {    80,    120,      1},
    {   200,    250,      1},
    {   252,    253,      9},
    {   273,    295,      1},
    {   300,    315,      1},
    {   333,    384,      1},
    {   400,    401,     10},
    {   405,    435,      1},
    {   445,    475,      1},
    {   485,    525,      1},
    {   585,    615,      1},
    {   685,    735,      1},
    {   783,    820,      1},
    {   956,    984,      1},
    {  1005,   1050,      1},
    {  1063,   1088,      1},
    {  1130,   1131,      9},
    {  1169,   1170,     10},
    {  1195,   1240,      1},
    {  1262,   1300,      1},
    {    -1,     -1,      0},
};

CPUBehaviour D_0D0093C0[] = {
    {     1,      2,      6},
    {    20,     40,      1},
    {   190,    191,      9},
    {   259,    260,     10},
    {   315,    335,      1},
    {   353,    370,      1},
    {   398,    430,      1},
    {   458,    485,      1},
    {   510,    535,      1},
    {   580,    660,      3},
    {    -1,     -1,      0},
};

Vec4f cpu_CurveTargetSpeed[] = {
    #include "assets/course_metadata/cpu_CurveTargetSpeed.inc.c"
};

Vec4f cpu_NormalTargetSpeed[] = {
    #include "assets/course_metadata/cpu_NormalTargetSpeed.inc.c"
};

Vec4f D_0D0096B8[] = {
    #include "assets/course_metadata/D_0D0096B8.inc.c"
};

Vec4f cpu_OffTrackTargetSpeed[] = {
    #include "assets/course_metadata/cpu_OffTrackTargetSpeed.inc.c"
};

u8 common_texture_speedometer[] = {
	#include "assets/code/common_data/common_texture_speedometer.i4.inc.c"
};

u8 common_texture_speedometer_needle[] = {
	#include "assets/code/common_data/common_texture_speedometer_needle.i4.inc.c"
};

u16 common_texture_hud_lap[] = {
	#include "assets/code/common_data/common_texture_hud_lap.rgba16.inc.c"
};

u16 common_texture_hud_123[] = {
	#include "assets/code/common_data/common_texture_hud_123.rgba16.inc.c"
};

u16 common_texture_hud_lap_time[] = {
	#include "assets/code/common_data/common_texture_hud_lap_time.rgba16.inc.c"
};

u16 common_texture_hud_lap_1_on_3[] = {
	#include "assets/code/common_data/common_texture_hud_lap_1_on_3.rgba16.inc.c"
};

u16 common_texture_hud_lap_2_on_3[] = {
	#include "assets/code/common_data/common_texture_hud_lap_2_on_3.rgba16.inc.c"
};

u16 common_texture_hud_lap_3_on_3[] = {
	#include "assets/code/common_data/common_texture_hud_lap_3_on_3.rgba16.inc.c"
};

u16 common_texture_hud_total_time[] = {
	#include "assets/code/common_data/common_texture_hud_total_time.rgba16.inc.c"
};

u16 common_texture_hud_time[] = {
	#include "assets/code/common_data/common_texture_hud_time.rgba16.inc.c"
};

u16 common_texture_hud_normal_digit[] = {
	#include "assets/code/common_data/common_texture_hud_normal_digit.rgba16.inc.c"
};

u8 common_texture_hud_place[][4096] = {
	{
		#include "assets/code/common_data/common_texture_hud_1st.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_2nd.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_3rd.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_4th.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_5th.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_6th.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_7th.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_8th.i4.inc.c"
	},
};
u8 D_0D015258[][2048] = {
	{
		#include "assets/code/common_data/common_texture_first_place.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_second_place.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_third_place.i4.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_fourth_place.i4.inc.c"
	},
};
u16 common_tlut_player_emblem[] = {
	#include "assets/code/common_data/common_tlut_player_emblem.tlut.inc.c"
};

u8 common_texture_player_emblem[][2048] = {
	{
		#include "assets/code/common_data/common_texture_player_emblem_1p.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_player_emblem_2p.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_player_emblem_3p.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_player_emblem_4p.ci8.inc.c"
	},
};
u16 common_tlut_hud_type_C_rank_font[] = {
	#include "assets/code/common_data/common_tlut_hud_type_C_rank_font.tlut.inc.c"
};

u8 common_texture_hud_type_C_rank_font[][256] = {
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_1.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_2.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_3.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_4.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_5.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_6.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_7.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_font_8.ci8.inc.c"
	},
};
u16 common_tlut_hud_type_C_rank_tiny_font[] = {
	#include "assets/code/common_data/common_tlut_hud_type_C_rank_tiny_font.tlut.inc.c"
};

u8 common_texture_hud_type_C_rank_tiny_font[][64] = {
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_0.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_1.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_2.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_3.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_4.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_5.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_6.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_7.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_8.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_hud_type_C_rank_tiny_font_9.ci8.inc.c"
	},
};
u8 common_texture_character_portrait_border[] = {
	#include "assets/code/common_data/common_texture_character_portrait_border.ia4.inc.c"
};

u16 common_tlut_portrait_mario[] = {
	#include "assets/code/common_data/common_tlut_portrait_mario.tlut.inc.c"
};

u16 common_tlut_portrait_luigi[] = {
	#include "assets/code/common_data/common_tlut_portrait_luigi.tlut.inc.c"
};

u16 common_tlut_portrait_peach[] = {
	#include "assets/code/common_data/common_tlut_portrait_peach.tlut.inc.c"
};

u16 common_tlut_portrait_toad[] = {
	#include "assets/code/common_data/common_tlut_portrait_toad.tlut.inc.c"
};

u16 common_tlut_portrait_yoshi[] = {
	#include "assets/code/common_data/common_tlut_portrait_yoshi.tlut.inc.c"
};

u16 common_tlut_portrait_donkey_kong[] = {
	#include "assets/code/common_data/common_tlut_portrait_donkey_kong.tlut.inc.c"
};

u16 common_tlut_portrait_wario[] = {
	#include "assets/code/common_data/common_tlut_portrait_wario.tlut.inc.c"
};

u16 common_tlut_portrait_bowser[] = {
	#include "assets/code/common_data/common_tlut_portrait_bowser.tlut.inc.c"
};

u16 common_tlut_portrait_bomb_kart_and_question_mark[] = {
	#include "assets/code/common_data/common_tlut_portrait_bomb_kart_and_question_mark.tlut.inc.c"
};

u8 common_texture_portrait_mario[] = {
	#include "assets/code/common_data/common_texture_portrait_mario.ci8.inc.c"
};

u8 common_texture_portrait_luigi[] = {
	#include "assets/code/common_data/common_texture_portrait_luigi.ci8.inc.c"
};

u8 common_texture_portrait_peach[] = {
	#include "assets/code/common_data/common_texture_portrait_peach.ci8.inc.c"
};

u8 common_texture_portrait_toad[] = {
	#include "assets/code/common_data/common_texture_portrait_toad.ci8.inc.c"
};

u8 common_texture_portrait_yoshi[] = {
	#include "assets/code/common_data/common_texture_portrait_yoshi.ci8.inc.c"
};

u8 common_texture_portrait_donkey_kong[] = {
	#include "assets/code/common_data/common_texture_portrait_donkey_kong.ci8.inc.c"
};

u8 common_texture_portrait_wario[] = {
	#include "assets/code/common_data/common_texture_portrait_wario.ci8.inc.c"
};

u8 common_texture_portrait_bowser[] = {
	#include "assets/code/common_data/common_texture_portrait_bowser.ci8.inc.c"
};

u8 common_texture_portrait_bomb_kart[] = {
	#include "assets/code/common_data/common_texture_portrait_bomb_kart.ci8.inc.c"
};

u8 common_texture_portrait_question_mark[] = {
	#include "assets/code/common_data/common_texture_portrait_question_mark.ci8.inc.c"
};

u16 common_tlut_item_window_none[] = {
	#include "assets/code/common_data/common_tlut_item_window_none.tlut.inc.c"
};

u16 common_tlut_item_window_banana[] = {
	#include "assets/code/common_data/common_tlut_item_window_banana.tlut.inc.c"
};

u16 common_tlut_item_window_banana_bunch[] = {
	#include "assets/code/common_data/common_tlut_item_window_banana_bunch.tlut.inc.c"
};

u16 common_tlut_item_window_mushroom[] = {
	#include "assets/code/common_data/common_tlut_item_window_mushroom.tlut.inc.c"
};

u16 common_tlut_item_window_double_mushroom[] = {
	#include "assets/code/common_data/common_tlut_item_window_double_mushroom.tlut.inc.c"
};

u16 common_tlut_item_window_triple_mushroom[] = {
	#include "assets/code/common_data/common_tlut_item_window_triple_mushroom.tlut.inc.c"
};

u16 common_tlut_item_window_super_mushroom[] = {
	#include "assets/code/common_data/common_tlut_item_window_super_mushroom.tlut.inc.c"
};

u16 common_tlut_item_window_blue_shell[] = {
	#include "assets/code/common_data/common_tlut_item_window_blue_shell.tlut.inc.c"
};

u16 common_tlut_item_window_boo[] = {
	#include "assets/code/common_data/common_tlut_item_window_boo.tlut.inc.c"
};

u16 common_tlut_item_window_green_shell[] = {
	#include "assets/code/common_data/common_tlut_item_window_green_shell.tlut.inc.c"
};

u16 common_tlut_item_window_triple_green_shell[] = {
	#include "assets/code/common_data/common_tlut_item_window_triple_green_shell.tlut.inc.c"
};

u16 common_tlut_item_window_red_shell[] = {
	#include "assets/code/common_data/common_tlut_item_window_red_shell.tlut.inc.c"
};

u16 common_tlut_item_window_triple_red_shell[] = {
	#include "assets/code/common_data/common_tlut_item_window_triple_red_shell.tlut.inc.c"
};

u16 common_tlut_item_window_star[] = {
	#include "assets/code/common_data/common_tlut_item_window_star.tlut.inc.c"
};

u16 common_tlut_item_window_thunder_bolt[] = {
	#include "assets/code/common_data/common_tlut_item_window_thunder_bolt.tlut.inc.c"
};

u16 common_tlut_item_window_fake_item_box[] = {
	#include "assets/code/common_data/common_tlut_item_window_fake_item_box.tlut.inc.c"
};

u8 common_texture_item_window_none[] = {
	#include "assets/code/common_data/common_texture_item_window_none.ci8.inc.c"
};

u8 common_texture_item_window_banana[] = {
	#include "assets/code/common_data/common_texture_item_window_banana.ci8.inc.c"
};

u8 common_texture_item_window_banana_bunch[] = {
	#include "assets/code/common_data/common_texture_item_window_banana_bunch.ci8.inc.c"
};

u8 common_texture_item_window_mushroom[] = {
	#include "assets/code/common_data/common_texture_item_window_mushroom.ci8.inc.c"
};

u8 common_texture_item_window_double_mushroom[] = {
	#include "assets/code/common_data/common_texture_item_window_double_mushroom.ci8.inc.c"
};

u8 common_texture_item_window_triple_mushroom[] = {
	#include "assets/code/common_data/common_texture_item_window_triple_mushroom.ci8.inc.c"
};

u8 common_texture_item_window_super_mushroom[] = {
	#include "assets/code/common_data/common_texture_item_window_super_mushroom.ci8.inc.c"
};

u8 common_texture_item_window_blue_shell[] = {
	#include "assets/code/common_data/common_texture_item_window_blue_shell.ci8.inc.c"
};

u8 common_texture_item_window_boo[] = {
	#include "assets/code/common_data/common_texture_item_window_boo.ci8.inc.c"
};

u8 common_texture_item_window_green_shell[] = {
	#include "assets/code/common_data/common_texture_item_window_green_shell.ci8.inc.c"
};

u8 common_texture_item_window_triple_green_shell[] = {
	#include "assets/code/common_data/common_texture_item_window_triple_green_shell.ci8.inc.c"
};

u8 common_texture_item_window_red_shell[] = {
	#include "assets/code/common_data/common_texture_item_window_red_shell.ci8.inc.c"
};

u8 common_texture_item_window_triple_red_shell[] = {
	#include "assets/code/common_data/common_texture_item_window_triple_red_shell.ci8.inc.c"
};

u8 common_texture_item_window_star[] = {
	#include "assets/code/common_data/common_texture_item_window_star.ci8.inc.c"
};

u8 common_texture_item_window_thunder_bolt[] = {
	#include "assets/code/common_data/common_texture_item_window_thunder_bolt.ci8.inc.c"
};

u8 common_texture_item_window_fake_item_box[] = {
	#include "assets/code/common_data/common_texture_item_window_fake_item_box.ci8.inc.c"
};

u16 common_tlut_lakitu_countdown[][256] = {
	{
		#include "assets/code/common_data/common_tlut_lakitu_no_lights.tlut.inc.c"
	},
	{
		#include "assets/code/common_data/common_tlut_lakitu_red_lights.tlut.inc.c"
	},
	{
		#include "assets/code/common_data/common_tlut_lakitu_blue_lights.tlut.inc.c"
	},
};
u16 common_tlut_lakitu_checkered_flag[] = {
	#include "assets/code/common_data/common_tlut_lakitu_checkered_flag.rgba16.inc.c"
};

u16 common_tlut_lakitu_second_lap[] = {
	#include "assets/code/common_data/common_tlut_lakitu_second_lap.rgba16.inc.c"
};

u16 common_tlut_lakitu_final_lap[] = {
	#include "assets/code/common_data/common_tlut_lakitu_final_lap.rgba16.inc.c"
};

u16 common_tlut_lakitu_reverse[] = {
	#include "assets/code/common_data/common_tlut_lakitu_reverse.rgba16.inc.c"
};

u16 common_tlut_lakitu_fishing[] = {
	#include "assets/code/common_data/common_tlut_lakitu_fishing.rgba16.inc.c"
};

u16 common_tlut_traffic_light[] = {
	#include "assets/code/common_data/common_tlut_traffic_light.tlut.inc.c"
};

u8 common_texture_traffic_light_01[] = {
	#include "assets/code/common_data/common_texture_traffic_light_01.ci8.inc.c"
};

u8 common_texture_traffic_light_02[] = {
	#include "assets/code/common_data/common_texture_traffic_light_02.ci8.inc.c"
};

u8 common_texture_traffic_light_03[] = {
	#include "assets/code/common_data/common_texture_traffic_light_03.ci8.inc.c"
};

u8 common_texture_traffic_light_04[] = {
	#include "assets/code/common_data/common_texture_traffic_light_04.ci8.inc.c"
};

u8 common_texture_traffic_light_05[] = {
	#include "assets/code/common_data/common_texture_traffic_light_05.ci8.inc.c"
};

u8 common_texture_traffic_light_06[] = {
	#include "assets/code/common_data/common_texture_traffic_light_06.ci8.inc.c"
};

u8 common_texture_traffic_light_07[] = {
	#include "assets/code/common_data/common_texture_traffic_light_07.ci8.inc.c"
};

u8 common_texture_traffic_light_08[] = {
	#include "assets/code/common_data/common_texture_traffic_light_08.ci8.inc.c"
};

u8 common_texture_traffic_light_09[] = {
	#include "assets/code/common_data/common_texture_traffic_light_09.ci8.inc.c"
};

u8 common_texture_traffic_light_10[] = {
	#include "assets/code/common_data/common_texture_traffic_light_10.ci8.inc.c"
};

u16 common_texture_particle_leaf[] = {
	#include "assets/code/common_data/common_texture_particle_leaf.rgba16.inc.c"
};

u16 common_texture_unused_particle_leaf[] = {
	#include "assets/code/common_data/common_texture_unused_particle_leaf.rgba16.inc.c"
};

u8 D_0D0293D8[] = {
	#include "assets/code/common_data/D_0D0293D8.i4.inc.c"
};

u8 D_0D029458[] = {
	#include "assets/code/common_data/D_0D029458.i8.inc.c"
};

u8 common_texture_bomb[][1024] = {
	{
		#include "assets/code/common_data/common_texture_bomb_1.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_bomb_2.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_bomb_3.ci8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_bomb_4.ci8.inc.c"
	},
};
u16 common_tlut_bomb[] = {
	#include "assets/code/common_data/common_tlut_bomb.tlut.inc.c"
};

u16 D_0D02AA58[] = {
	#include "assets/code/common_data/D_0D02AA58.rgba16.inc.c"
};

u8 common_texture_particle_spark[][1024] = {
	{
		#include "assets/code/common_data/common_texture_particle_spark_1.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_spark_2.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_spark_3.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_spark_4.i8.inc.c"
	},
};
u8 common_texture_particle_smoke[][1024] = {
	{
		#include "assets/code/common_data/common_texture_particle_smoke_1.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_smoke_2.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_smoke_3.i8.inc.c"
	},
	{
		#include "assets/code/common_data/common_texture_particle_smoke_4.i8.inc.c"
	},
};
u16 common_texture_minimap_finish_line[] = {
	#include "assets/code/common_data/common_texture_minimap_finish_line.rgba16.inc.c"
};

u16 common_texture_minimap_kart_mario[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_mario.rgba16.inc.c"
};

u16 common_texture_minimap_kart_luigi[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_luigi.rgba16.inc.c"
};

u16 common_texture_minimap_kart_yoshi[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_yoshi.rgba16.inc.c"
};

u16 common_texture_minimap_kart_toad[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_toad.rgba16.inc.c"
};

u16 common_texture_minimap_kart_donkey_kong[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_donkey_kong.rgba16.inc.c"
};

u16 common_texture_minimap_kart_wario[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_wario.rgba16.inc.c"
};

u16 common_texture_minimap_kart_peach[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_peach.rgba16.inc.c"
};

u16 common_texture_minimap_kart_bowser[] = {
	#include "assets/code/common_data/common_texture_minimap_kart_bowser.rgba16.inc.c"
};

u16 common_texture_minimap_progress_dot[] = {
	#include "assets/code/common_data/common_texture_minimap_progress_dot.rgba16.inc.c"
};
