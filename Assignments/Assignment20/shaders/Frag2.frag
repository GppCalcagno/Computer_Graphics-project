#version 450

layout(binding = 1) uniform sampler2D Ambient;

layout(binding = 2) uniform UniformBufferObject{
	vec3 lightDir;
	vec3 lightColor;
	vec3 eyePos;
}light;

layout(binding = 3) uniform sampler2D texel;

layout(location = 0) in vec3 fragViewDir;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

//here diffuse illumination has been baked (recorded) into a texture
//just an ambient term plus specular Blinn + directional light 

void main() {
	//ambient
	const vec3  AmbientC = texture(Ambient, fragTexCoord).rgb;

	//direct Light
	vec3 dir = light.lightDir;
	vec3 col = light.lightColor;
	
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(light.eyePos - fragViewDir);
	
	vec3 Direct = vec3(pow(max(dot(EyeDir, -reflect(dir, Norm)),0.0f), 64.0f));
	
	//blinn
	vec3 h0 = normalize( light.lightDir + EyeDir);
	vec3 Blinn0 = texture(texel, fragTexCoord).rgb*pow(clamp(dot(Norm,h0),0,1),200.0f*texture(texel, fragTexCoord).a);
	
	
	
	
	
	outColor = vec4((Direct + Blinn0 + AmbientC)*col, 1.0f);
}	