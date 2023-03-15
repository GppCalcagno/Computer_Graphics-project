#version 450

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	mat4 view;
	mat4 proj;
	
	vec3 cameraPos;
	vec4 selector;
	
	float lightPow;
	float BoatLightPos;
	//float par;
} gubo;

layout(set=1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragViewDir;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in float sigma;


layout(location = 0) out vec4 outColor;

vec3 Oren_Navay(vec3 L, vec3 N, vec3 V, float sigma) {
	vec3 diffColor = texture(texSampler, fragTexCoord).rgb;
	float TetaI=acos(dot(L,N));
	float TetaR=acos(dot(V,N));
	float A=1-0.5*(pow(sigma,2)/(pow(sigma,2)+0.33));
	float B= 0.45*(pow(sigma,2)/(pow(sigma,2)+0.09));

	float alpha = max(TetaI,TetaR);
	float beta = min(TetaI,TetaR);

	float G= cos(sigma);
	vec3 L1 = diffColor* clamp(dot(L,N),0,1);
	vec3 OrenNavay =  L1*(A+B*G*sin(alpha)*tan(beta));

	return OrenNavay;
}

vec3 Phong(vec3 L, vec3 N, vec3 V, vec3 specColor, float specPower) {
	vec3 R = -reflect(L, N);
	vec3 Phong = specColor * pow(max(dot(V,R), 0.0f), specPower);

	return Phong;
}


void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	vec3 lightColor = vec3(1.0f, 1.0f, 0.9f);
	vec3 N = normalize(fragNorm);
	vec3 V = normalize(gubo.cameraPos-fragViewDir); 
	vec3 L; //depend on the type of light
	
	vec3 pos=fragViewDir;

	/************************************************* LIGHT MODELS **********************************************/

	// Direct light - Day
	vec3 dir2 = vec3(-0.7, 0.7, 0.0);
	vec3 Direct = lightColor * gubo.selector.z;
		
	// POINT LIGHT - Day
	vec3 PointLightPos = vec3(0.0,6.0,-2.0);	
	vec3 dir0 = normalize(PointLightPos-pos);
	float g=5;
	float beta= 1.0f;
	vec3 PointLight = lightColor * pow(g/length(PointLightPos - pos),beta)*gubo.selector.y;

	//SPOT LIGHT - boat light Evening and Night
	vec3 BoatLightPos = vec3(0.0, 0.2f, 0.1f);
	vec3 dir1 = normalize(BoatLightPos-pos);
	vec4 SpotParameter = vec4(0.75f, 0.88f, 3.0, 1.0f); //outer cone - innercone - g - beta	
	
	vec3 lightDir = -normalize(vec3(1.0*sin(gubo.BoatLightPos),0.0,1.0*cos(gubo.BoatLightPos)));
	
	vec3 color =lightColor * pow(SpotParameter.z/length(BoatLightPos - pos),SpotParameter.w); //color 
	float cones= clamp((dot(normalize(BoatLightPos - pos),lightDir)-SpotParameter.x)/(SpotParameter.y-SpotParameter.x),0,1); //cones
	vec3 SpotLight = color*cones*gubo.selector.x;

	/************************************************** BDRF - DIFFUSE *****************************************************/
	//Oren-Navay - point light
	L = dir0;
	vec3 OrenNavay1 = Oren_Navay(L, N, V, sigma);
	//Oren-Navay - direct light
	L = dir2;
	vec3 OrenNavay2 = Oren_Navay(L, N, V, sigma);
	//Lambert - spot light
	L = dir1;
	vec3 Lambert1 = diffColor * (max(dot(N, L),0.0f));

	/************************************************** BDRF - SPECULAR *****************************************************/
	// Phong specular - point light
	L = dir0;
	vec3 Phong0 = Phong(L, N, V, vec3(1.0f, 1.0f, 0.9f), 450.0f);
	// Phong specular - spot light
	L = (vec3(-1.0*sin(gubo.BoatLightPos),0.0,-1.0*cos(gubo.BoatLightPos)));
	vec3 Phong1 = Phong(L, N, V, vec3(1.0f, 1.0f, 0.9f), 150.0f);
	// Phong specular - direct light
	L = dir2;
	vec3 Phong2 = Phong(L, N, V, vec3(1.0f, 1.0f, 0.9f), 150.0f);
		
	/************************************************** AMBIENT *****************************************************/
	//Hemisferical 
	vec3 LowerCol=vec3(0,0,0.8);
	vec3 UpperCol=vec3(1.0f, 1.0f, 0.9f);
	vec3 d= vec3(0.0f,1.0f,0.0f);
	vec3 La = ((dot(N,d)+1)/2)*UpperCol + ((1-dot(N,d))/2)*LowerCol;
	vec3 HemiDir = La*diffColor*gubo.lightPow;
	//Ambient
	vec3 ambient  = gubo.lightPow* diffColor;
	/************************************************** OUT COLOR *****************************************************/
	
	outColor = vec4(HemiDir + PointLight*(OrenNavay1+ Phong0)+ SpotLight*(Lambert1 + Phong1) + Direct*(OrenNavay2 + Phong2), 1.0f);	
}