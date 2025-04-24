#version 460 core

// Input
layout(location = 0) out int o_entityID;

// Uniform
layout(location = 1) uniform int u_entityID;

void main()
{
    o_entityID = u_entityID;
}
