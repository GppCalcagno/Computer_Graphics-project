
/*
Perspective projections represent an object with a different size depending on its distance from the projection plane
As for parallel projections, a depth z of an object is required. Projection is done exploitin triangle 
similarity, current distance and distance from the projection plan
*/

//fow is the solid angle at the top of the view frustum, from it and the aspect ratio we can derive all the information needed
//In order to reproduce correct imagines the bound of the screen in projection has to be coherent with the rateo : a = (r - l) / (t - b)

float near = 0.1;
float far = 9.9;


// Create a matrix for Perspecrtive projection with the given aspect ratio a,
// and a FovY = 90o
glm::mat4 PO1(float a) {
	glm::mat4 out = glm::mat4(1.0);

	float FovY = 90.0f;

	float fovy = glm::radians(FovY);
	//out = M * S * T *U //u is the projection, the others are used to trasform the frustum into a "cube"
	out = glm::perspective(fovy, a, near, far);
	//this is needed because of the Conversion from open GL to Vulkan for matrices for Fov/Aspect ratio projection
	out[1][1] *= -1;

	return out;
}

// Create a matrix for Perspecrtive projection with the given aspect ratio a,
// and a FovY = 120o
glm::mat4 PO2(float a) {
	glm::mat4 out = glm::mat4(1.0);

	float FovY = 120.0f;
	float fovy = glm::radians(FovY);

	//out = M * S * T *U //u is the projection, the others are used to trasform the frustum into a "cube"
	out = glm::perspective(fovy, a, near, far);
	//this is needed because of the Conversion from open GL to Vulkan for matrices for Fov/Aspect ratio projection
	out[1][1] *= -1;
	return out;
}

// Create a matrix for Perspecrtive projection with the given aspect ratio a,
// and a FovY = 30o
glm::mat4 PO3(float a) {
	glm::mat4 out = glm::mat4(1.0);

	float FovY = 30.0f;
	float fovy = glm::radians(FovY);

	//out = M * S * T *U //u is the projection, the others are used to trasform the frustum into a "cube"
	out = glm::perspective(fovy, a, near, far);
	//this is needed because of the Conversion from open GL to Vulkan for matrices for Fov/Aspect ratio projection
	out[1][1] *= -1;
	return out;
}

// Create a matrix for Perspecrtive projection, with the given aspect ratio a.
// Here the perspective should only show the left side of the view
// with a FovY = 90o. Here r=0, and l, t and b should be computed
// to match both the aspect ratio and the FovY
glm::mat4 PO4(float a) {
	glm::mat4 out = glm::mat4(1.0);

	float fov = glm::radians(90.0f);

	//compute the boundaries
	float t = near * tan(fov/2);
	float b = -near * tan(fov/2);

	float r = 0;
	float l= -a*near*tan(fov/2);

	// frustum() function to compute the perspective projection matrix specifying the boundaries
	//Other matrix flips the y-axis to match the Vulkan conventions
	out = glm::scale(glm::mat4(1.0), glm::vec3(1, -1, 1)) *glm::frustum(l, r, b, t, near, far); //Slide5.40

	return out;
}

