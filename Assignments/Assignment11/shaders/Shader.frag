#version 450
//Communication between the Shaders and the application occurs using Uniform Variables Blocks.
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform GlobalUniformBufferObject {
	float time;
} t;

void main() {
	outColor = vec4(0.0, 0.0, 0.0, 1.0f);


	//i used some "noicy" sinuisoidal function to let the color be between [0,1] and pseudo - random 
	//outColor is A global variable to communicate with the fixed part of the shader
	outColor[0] = 0.5 + 0.5*cos(t.time*3 *3.14 + t.time*3)*sin(t.time*2 * 3.14) ;
	outColor[1] = 0.5 + 0.5*cos(t.time *3.14)*sin(t.time*2 * 3.14 + t.time *2);
	outColor[2] = 0.5 + 0.5*cos(t.time*3 *3.14)*sin(t.time*1.5 * 3.14);
}