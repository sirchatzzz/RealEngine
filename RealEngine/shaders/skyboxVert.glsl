#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 inVertex;

layout(location = 0) out vec3 texCoord;

layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 viewMatrix;


void main() {
    texCoord = inVertex.xyz;
    texCoord.x *= -1.0;

    gl_Position = projectionMatrix * viewMatrix  * inVertex;
}