#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 mMat;
	mat4 nMat;
} ubo;

/**** Modify from here *****/
/*
In particular, it allows to split vertex data into different arrays, stored in a binding,  each one containing some of the attributes.
here a single binding is used, different location are used to stored different vectors
*/

Description structureslayout(location=0) in vec3 inPosition; 	//position of the obj in local coordinates
layout(location=2) in vec2 inTexCoord; 	//texture coordinates to be passed to the frag 
layout(location=1) in vec3 inNorma; 	//normal coordinates

//to comunicate with the Fragment Shader
layout(location=0) out vec3 fragPos;	
layout(location=1) out vec3 fragNorm;
layout(location=2) out vec2 fragTexCoord;

/**** To here *****/

void main() {
	gl_Position = ubo.mvpMat * vec4(inPosition, 1.0); 
	fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
	fragNorm = mat3(ubo.nMat) * inNormal;
	fragTexCoord = inTexCoord;
}