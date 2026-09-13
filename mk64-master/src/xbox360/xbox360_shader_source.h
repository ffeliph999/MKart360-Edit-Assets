#ifndef X360_SHADER_SOURCE_H
#define X360_SHADER_SOURCE_H
#include <string>
#include "xbox360/gfx_cc.h"
static std::string x360_operand(unsigned n, bool alpha) {
    if (!n) return alpha ? "0.0" : "float3(0,0,0)";
    if (n <= SHADER_INPUT_4) {
        char index = '0' + (char)(n - 1);
        return std::string("v.c") + index + (alpha ? ".a" : ".rgb");
    }
    if (n == SHADER_TEXEL0A) return alpha ? "t0.a" : "t0.aaa";
    return std::string(n == SHADER_TEXEL0 ? "t0" : "t1") + (alpha ? ".a" : ".rgb");
}
static std::string x360_formula(const CCFeatures &f, int channel) {
    return "(" + x360_operand(f.c[channel][0],channel!=0) + "-" +
        x360_operand(f.c[channel][1],channel!=0) + ")*" +
        x360_operand(f.c[channel][2],channel!=0) + "+" +
        x360_operand(f.c[channel][3],channel!=0);
}
static std::string x360_shader_source(const CCFeatures &f) {
    std::string s = "struct V { float4 p:POSITION;";
    if (f.used_textures[0] || f.used_textures[1]) s += "float2 uv:TEXCOORD0;";
    if (f.opt_fog) s += "float4 fog:TEXCOORD1;";
    for (int i=0;i<f.num_inputs;i++) {
        char n='0'+i, sem='2'+i;
        s += std::string(f.opt_alpha?"float4 c":"float3 c")+n+":TEXCOORD"+sem+";";
    }
    s += "}; V VS(V v) { return v; }\nsampler2D s0:register(s0); sampler2D s1:register(s1);\n";
    s += "float4 PS(V v";
    if(f.opt_noise && f.opt_alpha) s += ",float2 screen:VPOS";
    s += "):COLOR0 { float4 t0=float4(1,1,1,1),t1=t0;";
    if(f.used_textures[0]) s += "t0=tex2D(s0,v.uv);";
    if(f.used_textures[1]) s += "t1=tex2D(s1,v.uv);";
    s += "float4 result=float4("+x360_formula(f,0)+","+(f.opt_alpha?x360_formula(f,1):"1.0")+");";
    if(f.opt_texture_edge) s += "clip(result.a-0.3);result.a=1.0;";
    if(f.opt_noise && f.opt_alpha) s += "clip(result.a-frac(sin(dot(screen,float2(12.9898,78.233)))*43758.5453));result.a=1.0;";
    if(f.opt_fog) s += "result.rgb=lerp(result.rgb,v.fog.rgb,saturate(v.fog.a));";
    return s+"return saturate(result); }\n";
}
#endif
