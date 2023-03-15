 #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform GlobalUniformBufferObject {
	// Direct lights parameters (for diffuse and specular)
	vec3 lightDir0;
	vec3 lightColor0;
	vec3 lightDir1;
	vec3 lightColor1;
	vec3 lightDir2;
	vec3 lightColor2;
	vec3 lightDir3;
	vec3 lightColor3;

	// Ambient light parameters
	vec3 AmbColor;	// also Bottom color for Hemispheric light,
					//      Constant term for Spherical Harmonics light
	vec3 TopColor;	// also DyColor for Spherical Harmonics light
	vec3 DzColor;
	vec3 DxColor;
	
	// Other parameters (not relevant for the exercise)
	vec3 eyePos;
	vec4 selector;
} gubo;

/**** Modify from here *****/

layout(location=2) in vec2 fragTexCoord;
layout(location=1) in vec3 fragNorm;
layout(location=0) in vec3 fragPos;

layout(location=0) out vec4 outColor;

/*
Realistic rendering techniques tries to consider indirect lighting: illumination caused by lights that bounces from other objects.
*/


//Ambient Lightining is the simples approximation for indirect illumination
//ambient light emission factor (constant) accounts for the light reflected by al object
//ambient light reflection (fx) in this case is a constant

//ren Nayar Diffuse used to model the roughness of the material, that highlight the retroreflection of the materials 
//it requires three vectors: the direction of the light L, the normal vector N,  and the direction of the viewer V and the roughtness of the material sigma .
vec3 Case1_Color(vec3 N, vec3 V, vec3 Cd, vec3 Ca, float sigma) {
	// Oren Nayar Diffuse + Ambient
	// No Specular
	// One directional light (lightDir0 and lightColor0)
	//
	// Parameters are:
	//
	// vec3 N : normal vector
	// vec3 V : view direction
	// vec3 Cd : main color (diffuse color)
	// vec3 Ca : ambient color
	// float sigma : roughness of the material
	
	float TetaI=acos(dot(gubo.lightDir0,N));
	float TetaR=acos(dot(V,N));
	
	float A=1-0.5*(pow(sigma,2)/(pow(sigma,2)+0.33));
	float B= 0.45*(pow(sigma,2)/(pow(sigma,2)+0.09));
	
	float alpha = max(TetaI,TetaR);
	float beta = min(TetaI,TetaR);

	float G= cos(sigma);
	vec3 L1 = Cd * clamp(dot(gubo.lightDir0,N),0,1);
	vec3 OnerNavar =  L1*(A+B*G*sin(alpha)*tan(beta));
	
	//gubo.AmbColor ->la
	//X -> "In general correspond to the main color of the object"  - Extension of the BRDF for the indirect light fa()
	vec3 X = Ca; //but it could be also Cb -> notation ambiguity
	vec3 Ambient = gubo.AmbColor*X;

	//gubo.lightColor0 -> lightColor of the "direct light"
	//OnerNavar -> fr()
	return gubo.lightColor0*OnerNavar + Ambient;
}


//In Hemispheric 2 ambient light color are considered (grond and sky), "mixed" with a directional vector  d
vec3 Case2_Color(vec3 N, vec3 V, vec3 Cd, vec3 Ca) {
	// Lambert Diffuse + Hemispheric
	// No Specular
	// One directional light (lightDir0 and lightColor0)
	// Hemispheric light oriented along the y-axis
	//
	// Parameters are:
	//
	// vec3 N : normal vector
	// vec3 V : view direction
	// vec3 Cd : main color (diffuse color)
	// vec3 Ca : ambient color
	
	
	vec3 Lambert = Cd*max(0,dot(gubo.lightDir0,N));
	
	//direction vector d orients the blending of the two colors,oriented down to top
	//multiplied with the normal vector is used to mix the color 
	vec3 d= vec3(0.0f,1.0f,0.0f);
	vec3 La = ((dot(N,d)+1)/2)*gubo.TopColor + ((1-dot(N,d))/2)*gubo.AmbColor;
	vec3 HemiDir = La*Ca;
	
	return gubo.lightColor0*Lambert + HemiDir;
	
	
}

//Enconding the function la for image based component is done by Spherical Harmonics (Spectral Expansion)
// it can be approximate using a base color with 3 deviation releated to the direction of the light
vec3 Case3_Color(vec3 N, vec3 V, vec3 Cs, vec3 Ca, float gamma)  {
	// Spherical Harmonics
	// Blinn Specular
	// Four directional lights (lightDir0 to lightDir3, and lightColor0 to lightColor3)
	//
	// Parameters are:
	//
	// vec3 N : normal vector
	// vec3 V : view direction
	// vec3 Cs : specular color
	// vec3 Ca : ambient color
	// float gamma : Blinn exponent
	
	vec3 h0 = normalize( gubo.lightDir0 + V);
	vec3 Blinn0 = Cs*pow(clamp(dot(N,h0),0,1),gamma);
	
	vec3 h1 = normalize( gubo.lightDir1 + V);
	vec3 Blinn1 = Cs*pow(clamp(dot(N,h1),0,1),gamma);
	
	vec3 h2 = normalize( gubo.lightDir2 + V);
	vec3 Blinn2 = Cs*pow(clamp(dot(N,h2),0,1),gamma);
	
	vec3 h3 = normalize( gubo.lightDir3 + V);
	vec3 Blinn3 = Cs*pow(clamp(dot(N,h3),0,1),gamma);
	
	vec3 Blinn = gubo.lightColor0*Blinn0 + gubo.lightColor1*Blinn1 + gubo.lightColor2*Blinn2+ gubo.lightColor3*Blinn3 ;
	
	
	//like hemisferical but with more than 2 color -> the normal direction is used to calculate the color.
	//tipically the colors are computed with an approximation of real images and filter, but this function can be approximate also
	//using Spherical Harmonics:a basic color lC, plus three “deviation terms” along the three main axis x, y and z
	vec3 Harmonics = gubo.AmbColor + N.x*gubo.DxColor + N.y*gubo.TopColor + N.z*gubo.DzColor;
	
	return Blinn + Harmonics*Ca;
}



/**** To here *****/




void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos.xyz - fragPos);
	
	float AmbFact = 0.025;
	
	vec3 DifCol = texture(texSampler, fragTexCoord).rgb * gubo.selector.w +
				  vec3(1,1,1) * (1-gubo.selector.w);

	vec3 CompColor = gubo.selector.x *
						Case1_Color(Norm, EyeDir, DifCol, DifCol, 1.2f) +
					 gubo.selector.y *
						Case2_Color(Norm, EyeDir, DifCol, DifCol) +
					 gubo.selector.z *
						Case3_Color(Norm, EyeDir, vec3(1.0f,1.0f,1.0f), DifCol, 200.0f);
	
	outColor = vec4(CompColor, 1.0f);	
}