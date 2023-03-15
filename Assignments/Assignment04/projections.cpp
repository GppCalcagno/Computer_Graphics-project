/*
Aspect Ratio is the atio between Horizontal and Vertical Screen dimension ; a= Dx/Dy (measure in meters)
In order to reproduce correct imagines the bound of the screen in projection has to be coherent with the rateo : a= (r-l)/(t-b)
2 kind of projection: Parallel: Ortho, Axonometrix, Oblique and Perspective
we pass from WORD coordinates (x,y,z,w) to cartesian coordinate 
*/


//this is needed in the code to set the "depth of the projection" from to [0,1] and not to [-1,1]
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE  

float halfwidth = 2;
float nearP = -4;
float farP = 12;

//Orthogonal Projections are projections where the plane is either the xy, yz or zx - plane, and the projection rays are perpendicular to it
//axonometric projection avoid to hide plan perpendicular to the projection plane showing all 3 axis


// Create a matrix for Isometric projection with the given aspect ration a
//In isonometric projection the 3 axis contain a 120grade angle
//maintain the distance
glm::mat4 PO1(float a) {
	glm::mat4 out = glm::mat4(1);

	//projection Matrix, value calculated from the half-width,  
	//we use the GLM function and adapted it for VULKAN with the scale function Slide 4.23 
	//also in vulkan z[0,1] in openGL[-1,1]-> small z -> closer obj
	glm::mat4 projection = glm::scale(glm::mat4(1.0), glm::vec3(1, -1, 1)) *
		glm::ortho((float)-halfwidth, (float)halfwidth, -halfwidth / a, halfwidth / a, (float)nearP, (float)farP);
	
	//this rotations are used to get a isonometric prohjection
	float ang = glm::radians(45.0f);
	glm::mat4 Ry = glm::rotate(glm::mat4(1), ang, glm::vec3(0, 1, 0));

	ang = glm::radians(35.26f);
	glm::mat4 Rx = glm::rotate(glm::mat4(1), ang, glm::vec3(1, 0, 0));


	out = projection * Rx * Ry * out;

	return out;
}

// Create a matrix for Dimnetric projection (alpha = 45 degree)
//Dimetric projections have two different units: one for the x and z-axis and one for the y - axis
glm::mat4 PO2(float a) {
	glm::mat4 out = glm::mat4(1.0);

	//projection Matrix, value calculated from the half-width, 
	//we use the GLM function and adapted it for VULKAN with the scale function Slide 4.23
	//also in vulkan z[0,1] in openGL[-1,1]-> small z -> closer obj
	glm::mat4 projection = glm::scale(glm::mat4(1.0), glm::vec3(1, -1, 1)) *
		glm::ortho((float)-halfwidth, (float)halfwidth, -halfwidth / a, halfwidth / a, (float)nearP, (float)farP);

	//Dimetric projections are obtained by applying a rotation of ±45o
	//	around the y - axis, followed by an arbitrary rotation a around the
	//	x - axis, before applying the basic parallel projection
	float ang = glm::radians(45.0f);
	glm::mat4 Ry = glm::rotate(glm::mat4(1), ang, glm::vec3(0, 1, 0));

	glm::mat4 Rx = glm::rotate(glm::mat4(1), ang, glm::vec3(1, 0, 0));  //alpha

	out = projection * Rx * Ry * out;

	return out;
}

// Create a matrix for Trimetric projection (alpha = 45 degree, beta = 60 degree)
//Trimetric projections allow a different unit for the three axes
glm::mat4 PO3(float a) {
	glm::mat4 out = glm::mat4(1.0);


	//projection Matrix, value calculated from the half-width, 
	//we use the GLM function and adapted it for VULKAN with the scale function Slide 4.23
	//also in vulkan z[0,1] in openGL[-1,1]-> small z -> closer obj
	glm::mat4 projection = glm::scale(glm::mat4(1.0), glm::vec3(1, -1, 1)) *
		glm::ortho((float)-halfwidth, (float)halfwidth, -halfwidth / a, halfwidth / a, (float)nearP, (float)farP);

	//Trimetric projections are obtained by applying an arbitrary rotation 
	//b around the y - axis, followed by an arbitrary rotation a around the
	//	x - axis, before applying the parallel projection

	float beta = glm::radians(60.0f);
	glm::mat4 Ry = glm::rotate(glm::mat4(1), beta, glm::vec3(0, 1, 0));

	float alpha = glm::radians(45.0f);
	glm::mat4 Rx = glm::rotate(glm::mat4(1), alpha, glm::vec3(1, 0, 0));

	out = projection * Rx * Ry * out;


	return out;
}

//In oblique projections rays are parallel, but oblique with respect to the projection plane
//x and y axis are parallel to the screen, z nop
//Oblique projections can be obtained by applying a shear along the z - axis before theorthogonal projection

// Create a matrix for Cabinet projection (alpha = 45)
// with the given aspect ration a
glm::mat4 PO4(float a) {
	glm::mat4 out = glm::mat4(1.0);
	float reductionfactor = 0.5; //cabinet


	//projection Matrix, value calculated from the half-width, 
	//we use the GLM function and adapted it for VULKAN with the scale function Slide 4.23
	//also in vulkan z[0,1] in openGL[-1,1]-> small z -> closer obj
	glm::mat4 projection = glm::scale(glm::mat4(1.0), glm::vec3(1, -1, 1)) *
		glm::ortho((float)-halfwidth, (float)halfwidth, -halfwidth / a, halfwidth / a, (float)nearP, (float)farP);


	
	//used the hand-made shear matrix due to i dind't find the shear GLM function.
	//In Cabinet projection lenght is not mainteined
	glm::mat4 Sz = glm::mat4(1, 0, 0, 0, 
		0, 1, 0, 0,
		-reductionfactor * cos(3.14 / 4), -reductionfactor * sin(3.14 / 4), 1, 0,
		0, 0, 0, 1);
		
	out = projection * Sz * out;
	return out;
}

