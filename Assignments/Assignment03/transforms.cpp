/*
//MATRIX ON THE LEFT CONVETION
//LAST OPERATION IS THE FIRST PERFORMED
//We can bring back an obj after a tranformation applying the inverse tranformatior (or the inverse of the tranformation)
//performing a rotation among a general axexs can be seen as bring back the object alligned with the origin and a common axes, and then bring it back
Generally compute the composite matrix can improve the performance 

*/

// Rotate 45 degrees around an arbitrary axis passing through (1,0,-1). 
	//The x-axis can be aligned to the arbitrary axis after a rotation of 30 degrees around the z-axis, 
	//and then -60 degrees around the y-axis.
	
	//HOW TO:
	//i moved the axes to the origin and then perform a rotation, after i bring it in the right position (and orientation)
	//this becuase to perfrom a correct rotation (using common method) i have to bring the object in the origin, perform a rotation and then bring it back

	glm::mat4 MT1 = glm::mat4(1);
	float ang1;
	//T(1,0,-1)^-1
	glm::mat4 MT1_1 = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 1)); 
	//Ry(-60)^-1
	ang1 = glm::radians(60.0f);
	glm::mat4 MT1_2 = glm::rotate(glm::mat4(1), ang1, glm::vec3(0, 1, 0));
	//Rz(30)^-1
	ang1 = glm::radians(-30.0f);
	glm::mat4 MT1_3 = glm::rotate(glm::mat4(1), ang1, glm::vec3(0, 0, 1));
	//Rx(45)
	ang1 = glm::radians(45.0f);
	glm::mat4 MT1_4 = glm::rotate(glm::mat4(1), ang1, glm::vec3(1, 0, 0));
	//Rz(30)
	ang1 = glm::radians(30.0f);
	glm::mat4 MT1_5 = glm::rotate(glm::mat4(1), ang1, glm::vec3(0, 0,1)); 
	//Ry(60)
	ang1 = glm::radians(-60.0f);
	glm::mat4 MT1_6 = glm::rotate(glm::mat4(1), ang1, glm::vec3(0, 1,0)); 
	//T(1,0,-1)
	glm::mat4 MT1_7 = glm::translate(glm::mat4(1), glm::vec3(1, 0, -1)); 

	MT1 = MT1_7* MT1_6*MT1_5*MT1_4 * MT1_3 * MT1_2 * MT1_1 * MT1;
	/* -------------------------------------------------------------------- */
	 			   
	// Half the size of an object, using as fixed point (5,0,-2)
	
	//HOW TO:
	//as rotation i moved the fixed point in the origin and than performed a scale 
	//same for the scale
	glm::mat4 MT2 = glm::mat4(1);

	//T(5,0,-2)^-1
	glm::mat4 MT2_1 = glm::translate(glm::mat4(1), glm::vec3(-5, 0, 2));
	//scale
	glm::mat4 MT2_2 = glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5));
	//T(5,0,-2)
	glm::mat4 MT2_3 = glm::translate(glm::mat4(1), glm::vec3(5, 0, -2));

	MT2 = MT2_3 * MT2_2 * MT2_1 * MT2;
	/* -------------------------------------------------------------------- */
			   
	// Mirror the starship along a plane passing through (1,1,1), and obtained rotating 15 degree around the x axis the xz plane
	glm::mat4 MT3 = glm::mat4(1);

	//HOW TO:
	//move the axix in the origin and in the right orientation, thant perform the mirror and restore the original position
	//remenber that mirroring is a particualar scaling tranformation

	//T(1,1,1)^-1
	glm::mat4 MT3_1 = glm::translate(glm::mat4(1), glm::vec3(-1, -1, -1));

	//rotating x assis -15 grad
	float ang3 = glm::radians(-15.0f);
	glm::mat4 MT3_2 = glm::rotate(glm::mat4(1), ang3, glm::vec3(1, 0, 0));

	//mirroring along the axis xz
	glm::mat4 MT3_3 = glm::scale(glm::mat4(1), glm::vec3(1,-1,1));

	//rotating x assis 15 grad
	ang3 = glm::radians(15.0f);
	glm::mat4 MT3_4 = glm::rotate(glm::mat4(1), ang3, glm::vec3(1, 0, 0));

	//T(1,1,1)
	glm::mat4 MT3_5 = glm::translate(glm::mat4(1), glm::vec3(1, 1, 1));


	MT3 = MT3_5 * MT3_4 * MT3_3 * MT3_2 * MT3_1 * MT3;

				  
	// Apply the inverse of the following sequence of transforms: Translation of (0, 0, 5) then rotation of 30 degree around the Y axis, and finally a uniform scaling of a factor of 3.
	glm::mat4 MT4 = glm::mat4(1);
	float ang4;

	//HOW TO:
	//remember = (A*B)^-1 = B^-1 * A^-1 
	//Initial: S3*R2*T1*M		->  Inverse: (S3*R2*T1)^-1
	//another method could be do the inverse operation directly with the opposite moovement
	
	//T(0,0,5)
	glm::mat4 MT4_1 = glm::translate(glm::mat4(1), glm::vec3(0,0, 5));
	
	//R(30)
	ang4 = glm::radians(30.0f);
	glm::mat4 MT4_2 = glm::rotate(glm::mat4(1), ang4, glm::vec3(0, 1, 0));

	//scaling
	glm::mat4 MT4_3 = glm::scale(glm::mat4(1), glm::vec3(3));

	//inverse operation
	glm::mat4 REVERSE = MT4_3 * MT4_2 * MT4_1;
	REVERSE = inverse(REVERSE);

	MT4 = REVERSE * MT4;

	



