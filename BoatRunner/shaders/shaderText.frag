#version 450

layout(set = 0, binding = 0) uniform GlobalUniformBufferText {
	mat4 view;
	mat4 proj;
} gubo;

layout(set=1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	// Ambient - Compex Illumination are not usefull  
	outColor= vec4(diffColor,1.0f);
	
}