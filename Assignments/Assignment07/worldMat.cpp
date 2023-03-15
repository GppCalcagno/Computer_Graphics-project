/*
local Matrix -> orignal coordinates, World Matrix: tranformation matrix 
//Object Orientation can be represented in different ways: Euler Angle or Gimblal With Quaterion
A rotation defined by the Euler Angles, is perfect for "planar" movements
Gimbal are better for a fly mode movements

*/


//  Create a world matrix using position, Euler angles, and size
// Euler angles are passed in YPR parameter:
// YPR.x : Yaw
// YPR.y : Pitch
// YPR.z : Roll
glm::mat4 MakeWorldMatrixEuler(glm::vec3 pos, glm::vec3 YPR, glm::vec3 size) {
	glm::mat4 out = glm::mat4(1.0f);
	
	//this are transaltion and scale
	glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
	glm::mat4 S = glm::scale(glm::mat4(1.0), size);
	//curretion Orientation expressed in Euler Angle (that must have this order) 
	//Originally built for a z-Up Coordinates, we are in Y-Up ones
	glm::mat4 Mx = glm::rotate(glm::mat4(1), glm::radians(YPR.y), glm::vec3(1, 0, 0));
	glm::mat4 My = glm::rotate(glm::mat4(1), glm::radians(YPR.x), glm::vec3(0, 1, 0));
	glm::mat4 Mz = glm::rotate(glm::mat4(1), glm::radians(YPR.z), glm::vec3(0, 0, 1));
	
	//to let this work i have to add #include <glm/gtx/euler_angles.hpp> to the assignmento code
	glm::mat4 MEa = glm::eulerAngleYXZ(glm::radians(YPR.x),glm::radians(YPR.y),glm::radians(YPR.z));

	//World matrix are built applying at first the scaling, after the rotation and at the end the Transation
	//out = T*My * Mx * Mz* S * out;
	out = T * MEa * S * out;

	return out;
}

// Create a world matrix using position, quaternion angles, and size
//Quaternions are an extension of complex numbers that have three imaginary componen. Unitary Quaterion are used
glm::mat4 MakeWorldMatrixQuat(glm::vec3 pos, glm::quat rQ, glm::vec3 size) {
	glm::mat4 out = glm::mat4(1.0f);

	glm::mat4 T = glm::translate(glm::mat4(1.0), pos);
	glm::mat4 S = glm::scale(glm::mat4(1.0), size);

	rQ = glm::normalize(rQ); //to ensure rotate property (norm=1)

	//Quanterion can be directly converted in rotation matrix
	glm::mat4 MQ = glm::mat4(rQ);
	//the order of operation remain the same.
	out = T * MQ * S * out;

	return out;
}

