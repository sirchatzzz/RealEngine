#version 450
#extension GL_ARB_separate_shader_objects : enable


in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 normal;
out vec2 texCoords;
out vec4 fragPosLightSpace;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;


void main() {
	fragPos = vec3(model * vec4(aPos, 1.0));
	normal = transpose(inverse(mat3(model))) * aNormal;
	texCoords = aTexCoords;
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	gl_Position = projection * view * vec4(fragPos, 1.0);
}