	#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec3 lightPos;
	vec3 lightColor;
	vec3 eyePos;
	vec4 coneInOutDecayExp;
	vec4 selector;
} gubo;




/**** Modify from here *****/	

/*
the sign of the light direction is chosen to make the ray point toward the light source
A vector (lR, lG, lB) of RGB components defines the light  intensity for each wavelength, specifying its color
*/

/*
FOR EVERY POINT OF AN OBJECT, THE DIRECTION OF THE LIGHT AND ITS COLOR USED IN THE RENDERING EQUATION ARE EXPRESSED WITH CONSTANT VALUES
USED FOR FAR LIGHT SOURCE LIKE SUN 
*/
vec3 direct_light_dir(vec3 pos) {
	// Directional light direction
	return gubo.lightDir;
}

vec3 direct_light_color(vec3 pos) {
	// Directional light color
	return gubo.lightColor;
}

/*
POINT LIGHTS ARE SOURCES THAT EMIT LIGHT FROM FIXED POINTS IN THE SPACE AND HAVE NO DIRECTION
THEY ARE USED TO MODEL SOURCES THAT EMIT LIGHT IN ALL DIRECTION - characterized by the position and the color of the light
*/
vec3 point_light_dir(vec3 pos) {
	
	//vec3 dir = (pos-gubo.lightPos)/length(gubo.lightPos - pos);
	vec3 dir = normalize(gubo.lightPos - pos);
	return dir;
}

vec3 point_light_color(vec3 pos) {
	//point light are characterized by a exponential decay factor beta
	//it is also specified the value g, that represents the distance at which the light reduction is 1
	
	//formula 
	//I DON'T UNDERSTAND WELL WHAT IS W, I SUPPOSE IT COULD BE THE DECAY FACTOR  Slide 15.22
	//ubo.coneInOutDecayExp.w is a float component containing the denominator ??? exponent b for both spot and point lights
	vec3 col = gubo.lightColor * pow(gubo.coneInOutDecayExp.z/length(gubo.lightPos - pos),gubo.coneInOutDecayExp.w);
	return col;
}

/*
SPOT LIGHT ARE SPECIAL PROJECTORS THAT ARE USED TO ILLUMINATE SPECIFIC OBJECT OR LOCATION
Source are characterized by a direction and a posiiton
*/
vec3 spot_light_dir(vec3 pos) {
	vec3 dir = normalize(gubo.lightPos - pos);
	return dir;
}

vec3 spot_light_color(vec3 pos) {
	//Spot light are characteried by 2 angles that divide the illuminated area into 3 zones: constant, decay and absent
	//Cosine of the half-angles of the inner and outer cone are used
	vec3 point = gubo.lightColor * pow(gubo.coneInOutDecayExp.z/length(gubo.lightPos - pos),gubo.coneInOutDecayExp.w); //color 
	float num= clamp((dot(normalize(gubo.lightPos - pos),gubo.lightDir)-gubo.coneInOutDecayExp.x)/(gubo.coneInOutDecayExp.y-gubo.coneInOutDecayExp.x),0,1); //cones
	vec3 col = point*num;
	return col;
}

/**** To  here *****/





void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos.xyz - fragPos);
	
	vec3 lD;	// light direction from the light model
	vec3 lC;	// light color and intensity from the light model
	
	lD = direct_light_dir(fragPos) * gubo.selector.x +
	     point_light_dir(fragPos)  * gubo.selector.y +
	     spot_light_dir(fragPos)   * gubo.selector.z;
	lC = direct_light_color(fragPos) * gubo.selector.x +
	     point_light_color(fragPos)  * gubo.selector.y +
	     spot_light_color(fragPos)   * gubo.selector.z;

	vec3 Diffuse = (texture(texSampler, fragTexCoord).rgb * gubo.selector.w + vec3(1,1,1) * (1-gubo.selector.w)) * (max(dot(Norm, lD),0.0f) );
	vec3 Specular = vec3(pow(max(dot(EyeDir, -reflect(lD, Norm)),0.0f), 64.0f));
	//
	outColor = vec4((Diffuse + Specular) * lC, 1.0f);	
}