#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform UniformBufferObject {
	vec3 lightDir;
	vec3 lightPos;
	vec3 lightColor;
	vec4 lightParams;
	vec3 eyePos;
} light;


layout(binding = 3) uniform sampler2D texel;

layout(location = 0) in vec3 fragViewDir;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

//diffuse Lambert with specular Phong + spot light
void main() {
	
	
	//spot light
	vec3 dir = normalize(light.lightPos - fragViewDir);
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(light.eyePos - fragViewDir);
	
	vec3 colSpot = light.lightColor*(pow(light.lightParams.w/length(light.lightPos - fragViewDir),light.lightParams.z));
	float num = clamp((dot(normalize(light.lightPos - fragViewDir),light.lightDir)-light.lightParams.y)/(light.lightParams.x-light.lightParams.y),0,1); 
	
	colSpot = colSpot*num;
	
	vec3 Spot = vec3(pow(max(dot(EyeDir, -reflect(dir, Norm)),0.0f), 64.0f));
	
	vec3 R = -reflect(light.lightDir, Norm);
	vec3 V = normalize(fragViewDir);
	// Lambert diffuse
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb; //vec3(1.0f, 1.0f, 1.0f);
	vec3 diffuse  = diffColor * max(dot(Norm,light.lightDir), 0.0f);
	
	// Phong specular
	vec3 PhongColor = texture(texel, fragTexCoord).rgb;
	vec3 Phong = PhongColor * pow(max(dot(R,V), 0.0f), 200.0f*texture(texel, fragTexCoord).a);

	outColor = vec4((Spot + Phong + diffuse)*colSpot, 1.0f);
}	