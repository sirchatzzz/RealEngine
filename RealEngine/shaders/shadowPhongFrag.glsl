#version 450
#extension GL_ARB_separate_shader_objects : enable

out vec4 fragColor;

in vec3 normal;
in vec2 texCoords;
in vec4 fragPosLightSpace;
in vec3 fragPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 lightDir;
float ShadowCalc(vec4 fragPosLightSpace);

void main() {
	vec3 color = texture(diffuseTexture, texCoords).rgb;
	vec3 normal = normalize(normal);
	vec3 lightColor = vec3(1.0);
	vec3 ambient = 0.09 * lightColor;
	lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 10.0);
	vec3 specular = spec * lightColor;
	float shadow = ShadowCalc(fragPosLightSpace);
	vec3 lightning = (ambient +(1.0 - shadow) * (diffuse + specular)) * color;

	fragColor = vec4(lightning, 1.0);
}

float ShadowCalc(vec4 fragPosLightSpace) {
	float shadow = 0.0;
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	//percentage-closer filtering (pcf)
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	const int halfKernelWidth = 6;
	for(int x = -halfKernelWidth; x <= halfKernelWidth; ++x)
	{
		for(int y = -halfKernelWidth; y <= halfKernelWidth; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - 0.004 > pcfDepth ? 0.6 : 0.0;
		}
	}
	shadow /= ((halfKernelWidth*2+1)*(halfKernelWidth*2+1));
	projCoords.z > 1.0 ? shadow = 0.0 : 1.0;

	return shadow;
}
