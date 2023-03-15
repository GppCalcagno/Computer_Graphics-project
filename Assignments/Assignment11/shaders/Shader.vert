#version 450

/*
Communication between  the Shaders and the  Pipeline occurs through  global variables
in and out variables are used to interface with the programmable or configurable part of the pipeline

*/

//Communication between the Shaders and the application occurs using Uniform Variables Blocks.
layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 worldMat;
	mat4 viewMat;
	mat4 prjMat;
} ubo;

layout(location = 0) in vec3 inPosition;

void main() {
	//just applied the WVP matrix to return clipping coordinates (because of prespective Projection)
	//WVP matrix is passed by the program to the shader
	//gl_Position is a vec4 variable that must be filled with the clipping coordinates of the corresponding vertex.
	gl_Position = ubo.prjMat *ubo.viewMat * ubo.worldMat * vec4(inPosition, 1.0);
}