#version 450

layout(set = 0, binding = 0) uniform GlobalUniformBufferBackground {
	mat4 view;
	mat4 proj;
	float LightPow;
} gubo;


layout(set=1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	
	//Ambient Light- we used it because the background doesn't need a complex illumination
	vec3 Ambient = diffColor*gubo.LightPow;
	outColor= vec4(Ambient,1.0f);
}