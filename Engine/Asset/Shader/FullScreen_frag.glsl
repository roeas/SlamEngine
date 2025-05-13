#version 460 core

#include <ToneMapping.glsl>

// Input
layout(location = 0) in vec2 v_uv;

// Output
layout(location = 0) out vec4 o_color;

// Uniform
layout(location = 0) uniform int u_toneMappingMode;

// Texture
layout(binding = 0) uniform sampler2D s_preResult;

vec3 ToneMapping(vec3 color)
{
    switch(u_toneMappingMode)
    {
        case SL_TONEMAPPING_CLAMP:
        {
            return Clamp(color);
        }
        case SL_TONEMAPPING_REINHARD:
        {
            return Reinhard(color);
        }
        case SL_TONEMAPPING_HABLEFILMIC:
        {
            return HableFilmic(color);
        }
        case SL_TONEMAPPING_ACES:
        {
            return ACES(color);
        }
        case SL_TONEMAPPING_ACES_SIMPLE:
        {
            return ACES_Simple(color);
        }
        case SL_TONEMAPPING_PBRNEUTRAL:
        {
            return PBRNeutral(color);
        }
        default:
        {
            return color;
        }
    }
}

void main()
{
    vec3 finalColor = texture(s_preResult, v_uv).xyz;

    // Tone mapping
    finalColor = clamp(ToneMapping(finalColor), 0.0, 1.0);

    // Gamma
    finalColor = pow(finalColor, vec3(0.45));

    o_color = vec4(finalColor, 1.0);
}
