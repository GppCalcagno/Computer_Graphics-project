/*
3 types of motion:
- Ground Motion are like Walk Procedure for a Camera Object
- Global Coordinates Model like the Groun One but motion are not affected on Obj Orientation
- Local Coordinate are similat to the fly Camera Model
*/

// Create the world matrix for the robot
//windows are used to get access to the 

glm::mat4 getRobotWorldMatrix(GLFWwindow* window) {

	//timer
	static auto startTime = std::chrono::high_resolution_clock::now();
	static float lastTime = 0.0f; //save the "last" time 
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>
		(currentTime - startTime).count();
	float deltaT = time - lastTime;
	lastTime = time;

	//inizialization (Static used to "save" values")
	const float SPEED = 1.0;
	static float Yaw = 0;
	static float Pitch = 0;
	static float Roll = 0;
	static glm::vec3 pos = glm::vec3(-3, 0, 2); //initialize and store Current Pos
	//initial world matrix
	static glm::mat4 out = glm::translate(glm::mat4(1), pos) * glm::eulerAngleYXZ(glm::radians(Yaw), glm::radians(Pitch), glm::radians(Roll));
	
	//initialize and store Current Orientation
	static glm::mat4 rot = glm::mat4(1);		
	
	if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP) ) {
		pos -= SPEED*glm::vec3(0, 0, 1) * deltaT; //update the position
		Yaw = 90.f; //update the Orientation of the Objs (rotate on the y coordinates)
		out = glm::translate(glm::mat4(1), pos) * glm::eulerAngleYXZ(glm::radians(Yaw), glm::radians(Pitch), glm::radians(Roll));
	}

	if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) {
		pos += SPEED * glm::vec3(0, 0, 1) * deltaT;
		Yaw = -90.f;
		out = glm::translate(glm::mat4(1), pos) * glm::eulerAngleYXZ(glm::radians(Yaw), glm::radians(Pitch), glm::radians(Roll));
	}

	if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) {
		pos -= SPEED * glm::vec3(1, 0, 0) * deltaT;
		Yaw = 180.f;
		out = glm::translate(glm::mat4(1), pos) * glm::eulerAngleYXZ(glm::radians(Yaw), glm::radians(Pitch), glm::radians(Roll));
	}

	if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) {
		pos += SPEED * glm::vec3(1, 0, 0) * deltaT;
		Yaw = 0.0f;
		out = glm::translate(glm::mat4(1), pos) * glm::eulerAngleYXZ(glm::radians(Yaw), glm::radians(Pitch), glm::radians(Roll));
	}

	return out;
}

