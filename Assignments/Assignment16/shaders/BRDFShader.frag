#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir0;
	vec3 lightColor0;
	vec3 lightDir1;
	vec3 lightColor1;
	vec3 lightDir2;
	vec3 lightColor2;
	vec3 lightDir3;
	vec3 lightColor3;
	vec3 eyePos;
	vec4 selector;
} gubo;

/**** Modify from here *****/

/*
The surface properties of one object can be encoded in a function called Bidirectional Reflectance Distribution Function (BRDF) 
	The diffuse Reflection : represent the main color of the objec
	The Specular Reflection: represent the reflections related to the seen point wr
*/


// in all BRDF parameters are:
// 	vec3 L : light direction
// 	vec3 N : normal vector
// 	vec3 V : view direction
// 	vec3 C : main color (diffuse color, or specular color)

//in Lambert_Diffuse each point of an object jit by a rey of light, reflects it with uniform probability in all the directions
//Amoutn of light reflected is proportional to cos(a), the angle between the lightDirection and the Normal
vec3 Lambert_Diffuse_BRDF(vec3 L, vec3 N, vec3 V, vec3 C) {
	// Lambert Diffuse BRDF model
	vec3 res = C*max(0,dot(L,N)); //the dot product is used to calculate the cos(a)
	return res;
}

//in Phong Model, the specular reflection has the same angle as the incoming ray with respect to the normal vector, but in opposite direction
//it computes the intensity of the reflection from cos(a), where a is the angular distance between the specular direction and the observer
//to contain it effect the term cos(a) is raised at a power gamma
vec3 Phong_Specular_BRDF(vec3 L, vec3 N, vec3 V, vec3 C, float gamma)  {
	// Phong Specular BRDF model
	// float gamma : exponent of the cosine term
	vec3 specVec = -reflect(L,N);  //reflection in opposite direction, so the minus is needed
	float intensity = clamp(dot(V,specVec),0,1); 
	vec3 res = C* pow(intensity,gamma);
	return res;
}

//Toon Shading simplify the output color range using only discrete values according to a set of thresholuds
//start to one of the previous technique (diffuse or specular) and use a thresholud to set the main color
vec3 Toon_Diffuse_BRDF(vec3 L, vec3 N, vec3 V, vec3 C, vec3 Cd, float thr) {
	// Toon Diffuse Brdf
	// vec3 Cd : color to be used in dark areas
	// float thr : color threshold
	float val = dot(L,N); //compute Lambert
	vec3 col;
	
	if(val>= thr)
		col=C;
	else{
		if(val<0)
			col= vec3(0,0,0);
		else
			col= Cd;
		
	}
	
	vec3 res=col;
	return res;
}

vec3 Toon_Specular_BRDF(vec3 L, vec3 N, vec3 V, vec3 C, float thr)  {
	// Directional light direction
	// additional parameter:
	// float thr : color threshold
	vec3 specVec = -reflect(L,N);  //reflection in opposite direction, so the minus is needed
	float intensity = clamp(dot(V,specVec),0,1); //compute Phong
	if(intensity>thr)
		return C;
	return vec3(0,0,0); //second color could be this considering the pictures
}

//used to model the roughness of the material, that highlight the retroreflection of the materials 
//it requires three vectors: the direction of the light L, the normal vector N,  and the direction of the viewer V and the roughtness of the material sigma .
vec3 Oren_Nayar_Diffuse_BRDF(vec3 L, vec3 N, vec3 V, vec3 C, float sigma) {
	// Directional light direction
	// additional parameter:
	// float sigma : roughness of the material
	
	float TetaI=acos(dot(L,N));
	float TetaR=acos(dot(V,N));
	
	float A=1-0.5*(pow(sigma,2)/(pow(sigma,2)+0.33));
	float B= 0.45*(pow(sigma,2)/(pow(sigma,2)+0.09));
	
	float alpha = max(TetaI,TetaR);
	float beta = min(TetaI,TetaR);

	float G= cos(sigma);
	vec3 L1 = C* clamp(dot(L,N),0,1);
	vec3 res =  L1*(A+B*G*sin(alpha)*tan(beta));

	return res;
}


/**** To here *****/




vec3 BRDF_D(vec3 L, vec3 N, vec3 V, vec3 C) {
	return gubo.selector.x * Lambert_Diffuse_BRDF(L, N, V, C) +
		   (1.0 - gubo.selector.x) * (1.0 - gubo.selector.y) * 
		   			Toon_Diffuse_BRDF(L, N, V, C, 0.2f * C, 0.5) +
		   (1.0 - gubo.selector.x) * (gubo.selector.y) * 
		   			Oren_Nayar_Diffuse_BRDF(L, N, V, C, 1.5);
}

vec3 BRDF_S(vec3 L, vec3 N, vec3 V, vec3 C) {
	return Phong_Specular_BRDF(L, N, V, C, 200.0f) * gubo.selector.z +
		   (1.0 - gubo.selector.x) * (1.0 - gubo.selector.y) * 
		   			Toon_Specular_BRDF(L, N, V, vec3(1,1,1), 0.97f);
}

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos.xyz - fragPos);
	
	float AmbFact = 0.025;
	
	vec3 DiffColor = texture(texSampler, fragTexCoord).rgb * gubo.selector.w + vec3(1,1,1) * (1-gubo.selector.w);
	vec3 Diffuse = vec3(0,0,0);
	
	Diffuse += BRDF_D(gubo.lightDir0, Norm, EyeDir, DiffColor) * gubo.lightColor0;
	Diffuse += BRDF_D(gubo.lightDir1, Norm, EyeDir, DiffColor) * gubo.lightColor1;
	Diffuse += BRDF_D(gubo.lightDir2, Norm, EyeDir, DiffColor) * gubo.lightColor2;
	Diffuse += BRDF_D(gubo.lightDir3, Norm, EyeDir, DiffColor) * gubo.lightColor3;
	
	vec3 Specular = vec3(0,0,0);
	Specular += BRDF_S(gubo.lightDir0, Norm, EyeDir, vec3(1,1,1)) * gubo.lightColor0;
	Specular += BRDF_S(gubo.lightDir1, Norm, EyeDir, vec3(1,1,1)) * gubo.lightColor1;
	Specular += BRDF_S(gubo.lightDir2, Norm, EyeDir, vec3(1,1,1)) * gubo.lightColor2;
	Specular += BRDF_S(gubo.lightDir3, Norm, EyeDir, vec3(1,1,1)) * gubo.lightColor3;

	vec3 Ambient = AmbFact * DiffColor;
	
	outColor = vec4(Diffuse + Specular + Ambient, 1.0f);	
}