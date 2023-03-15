/*
negative z-axis corresponds to the North of the 3D world, and that the positive x-axis to the East
Camera Matrix correspond to the orietation and position of the camera. View Matrix = Camera Matrix ^-1
*/


//Look - In - Direction model is used for first person application, the position of the camera is given in world coordinates, while the direction in computed with 3 angles
 // Create a look in direction matrix
 // Pos    -> Position of the camera
 // Angs.x -> direction (alpha)
 // Angs.y -> elevation (beta)
 // Angs.z -> roll (rho)
glm::mat4 LookInDirMat(glm::vec3 Pos, glm::vec3 Angs) {
	glm::mat4 out = glm::mat4(1.0);
	//the order of execution of rotation must be this. This indicates the orietnation of the camera1
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0), Angs.z, glm::vec3(0, 0, 1)); // Roll -> z axes	
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0), Angs.y, glm::vec3(1, 0, 0)); //Elevation -> x axes
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0), Angs.x, glm::vec3(0, 1, 0)); //Direction -> y axes
	glm::mat4 t = glm::translate(glm::mat4(1.0), Pos); //position of the camera

	out = t * Ry * Rx * Rz * out;
	out = inverse(out);

	return out;
}

//The look - at model is instead generally employed in third person applications
//to implement this matrix we have to specify the targer coordinate, the camera coordinate, and the orientation of the up direciton

 // Create a look at matrix
 // Pos    -> Position of the camera (c)
 // aim    -> Posizion of the target (a)
 // Roll   -> roll (rho)
glm::mat4 LookAtMat(glm::vec3 Pos, glm::vec3 aim, float Roll) {
	glm::mat4 out = glm::mat4(1.0);
	glm::vec3 up = glm::vec3(0, 1, 0); //Vulkan uses an y-Up system


	//simple Solution
	glm::mat4 LookM = glm::lookAt(Pos, aim,up);

	//calculate Vz - axes designed by the line from the position of the camera to the target object 
	glm::vec3 Vz = Pos - aim;
	Vz = glm::normalize(Vz);

	//calculate Vx - X-axis must be perpendicular to the z-axis
	glm::vec3 Vx = glm::cross(up, Vz);
	Vx = glm::normalize(Vx);

	//calculate Vy - Must be perpedicular both to x and z
	glm::vec3 Vy = glm::cross(Vz, Vx);
	Vy = glm::normalize(Vy);

	//add the last 0 in vec4 
	glm::vec4 vx = glm::vec4(Vx, 0);
	glm::vec4 vy = glm::vec4(Vy, 0);
	glm::vec4 vz = glm::vec4(Vz, 0);
	glm::vec4 vu = glm::vec4(Pos, 1);

	//create the Camera Matrix Mc usign the 4 column
	glm::mat4 Mc = glm::mat4(vx, vy, vz, vu);

	//invert the Mc To find The LookAt Matrix 
	//glm::mat4 LookM = inverse(Mc);


	//using the glm::radiant the movement was too slow, so probably it isn't required
	//use this rotation to "implmement" the camera roll.
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0),-Roll , glm::vec3(0, 0, 1));

	out = LookM*out;
	return out;
}



