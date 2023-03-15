/*
The common approach uses tables to assign different values to the parameters of
the shaders, depending on the positions of the internal points of the surface called texture

2D textures are applied to 3D objects, using a mapping relation that associates
each point on the surface with a point on the texture.

Texture are mapped using UV mapping or texture coordinates. 
A cartesian plane with [u=0;v=0] in the lower left angle: u: ?? , v: ?? 
*/




// this function creates the geometries to be shown, and output thems
// in global variables M1_vertices and M1_indices to M4_vertices and M4_indices

#include <corecrt_math_defines.h>
struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

std::vector<Vertex> M1_vertices;
std::vector<Vertex> M2_vertices;


void makeModels() {
	//// M1 : Cube
	M1_vertices.resize(8 * 3);

	//n=0 vertex of M1
	M1_vertices[0].pos = glm::vec3(0.0, 0.0, 0.0);
	M1_vertices[0].norm = glm::vec3(0.0, -1.0, 0.0); //down
	M1_vertices[0].UV = glm::vec2(0.125, 0.375);

	// n=1 vertex of M1
	M1_vertices[1].pos = glm::vec3(1.0, 0.0, 0.0);
	M1_vertices[1].norm = glm::vec3(0.0, -1.0, 0.0);//down
	M1_vertices[1].UV = glm::vec2(0.25, 0.375);

	// n=2 vertex of M1
	M1_vertices[2].pos = glm::vec3(1.0, 1.0, 0.0);
	M1_vertices[2].norm = glm::vec3(0.0, 1.0, 0.0); //up
	M1_vertices[2].UV = glm::vec2(0.25, 0.25);

	// n=3 vertex of M1
	M1_vertices[3].pos = glm::vec3(0.0, 1.0, 0.0);
	M1_vertices[3].norm = glm::vec3(0.0, 1.0, 0.0); //up
	M1_vertices[3].UV = glm::vec2(0.125, 0.25);

	// n=4 vertex of M1
	M1_vertices[4].pos = glm::vec3(0.0, 0.0, 1.0);
	M1_vertices[4].norm = glm::vec3(0.0, -1.0, 0.0); //down
	M1_vertices[4].UV = glm::vec2(0.125, 0.5);

	// n=5 vertex of M1
	M1_vertices[5].pos = glm::vec3(1.0, 0.0, 1.0);
	M1_vertices[5].norm = glm::vec3(0.0, -1.0, 0.0); //down
	M1_vertices[5].UV = glm::vec2(0.25, 0.5);

	// n=6 vertex of M1
	M1_vertices[6].pos = glm::vec3(1.0, 1.0, 1.0);
	M1_vertices[6].norm = glm::vec3(0.0, 1.0, 0.0); //up
	M1_vertices[6].UV = glm::vec2(0.25, 0.125);

	// n=7 vertex of M1
	M1_vertices[7].pos = glm::vec3(0.0, 1.0, 1.0);
	M1_vertices[7].norm = glm::vec3(0.0, 1.0, 0.0); //up
	M1_vertices[7].UV = glm::vec2(0.125, 0.125);

	/*-----------------------------------------*/

	//n=0A vertex of M1
	M1_vertices[0 + 8].pos = glm::vec3(0.0, 0.0, 0.0);
	M1_vertices[0 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left
	M1_vertices[0 + 8].UV = glm::vec2(0.125, 0.375);


	// n=1A vertex of M1
	M1_vertices[1 + 8].pos = glm::vec3(1.0, 0.0, 0.0);
	M1_vertices[1 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right
	M1_vertices[1 + 8].UV = glm::vec2(0.25, 0.375);

	// n=2A vertex of M1
	M1_vertices[2 + 8].pos = glm::vec3(1.0, 1.0, 0.0);
	M1_vertices[2 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right
	M1_vertices[2 + 8].UV = glm::vec2(0.25, 0.25);


	// n=3A vertex of M1
	M1_vertices[3 + 8].pos = glm::vec3(0.0, 1.0, 0.0);
	M1_vertices[3 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left
	M1_vertices[3 + 8].UV = glm::vec2(0.125, 0.25);

	// n=4A vertex of M1
	M1_vertices[4 + 8].pos = glm::vec3(0.0, 0.0, 1.0);
	M1_vertices[4 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left
	M1_vertices[4 + 8].UV = glm::vec2(0.0, 0.375);

	// n=5A vertex of M1
	M1_vertices[5 + 8].pos = glm::vec3(1.0, 0.0, 1.0);
	M1_vertices[5 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right
	M1_vertices[5 + 8].UV = glm::vec2(0.375, 0.375);

	// n=6A vertex of M1
	M1_vertices[6 + 8].pos = glm::vec3(1.0, 1.0, 1.0);
	M1_vertices[6 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right
	M1_vertices[6 + 8].UV = glm::vec2(0.375, 0.25);


	// n=7A vertex of M1
	M1_vertices[7 + 8].pos = glm::vec3(0.0, 1.0, 1.0);
	M1_vertices[7 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left
	M1_vertices[7 + 8].UV = glm::vec2(0.0, 0.25);

	/*-----------------------------------------*/

	//n=0B vertex of M1
	M1_vertices[0 + 16].pos = glm::vec3(0.0, 0.0, 0.0);
	M1_vertices[0 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back
	M1_vertices[0 + 16].UV = glm::vec2(0.125, 0.375);

	// n=1B vertex of M1
	M1_vertices[1 + 16].pos = glm::vec3(1.0, 0.0, 0.0);
	M1_vertices[1 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back
	M1_vertices[1 + 16].UV = glm::vec2(0.25, 0.375);

	// n=2B vertex of M1
	M1_vertices[2 + 16].pos = glm::vec3(1.0, 1.0, 0.0);
	M1_vertices[2 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back
	M1_vertices[2 + 16].UV = glm::vec2(0.25, 0.25);

	// n=3B vertex of M1
	M1_vertices[3 + 16].pos = glm::vec3(0.0, 1.0, 0.0);
	M1_vertices[3 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back
	M1_vertices[3 + 16].UV = glm::vec2(0.125, 0.25);

	// n=4B vertex of M1
	M1_vertices[4 + 16].pos = glm::vec3(0.0, 0.0, 1.0);
	M1_vertices[4 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front
	M1_vertices[4 + 16].UV = glm::vec2(0.125, 0.0);

	// n=5B vertex of M1
	M1_vertices[5 + 16].pos = glm::vec3(1.0, 0.0, 1.0);
	M1_vertices[5 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front
	M1_vertices[5 + 16].UV = glm::vec2(0.25, 0.0);

	// n=6B vertex of M1
	M1_vertices[6 + 16].pos = glm::vec3(1.0, 1.0, 1.0);
	M1_vertices[6 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front
	M1_vertices[6 + 16].UV = glm::vec2(0.25, 0.125);

	// n=7B vertex of M1
	M1_vertices[7 + 16].pos = glm::vec3(0.0, 1.0, 1.0);
	M1_vertices[7 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front
	M1_vertices[7 + 16].UV = glm::vec2(0.125, 0.125);



	// Resizes the indices array. Repalce the values with the correct number of
	// indices (3 * number of triangles)
	M1_indices.resize(3 * 2 * 6);  //6 faces made of 2 triangle (3)

	// down triangle
	M1_indices[0] = 0;
	M1_indices[1] = 1;
	M1_indices[2] = 5;
	// down triangle
	M1_indices[3] = 0;
	M1_indices[4] = 4;
	M1_indices[5] = 5;

	// Up triangle
	M1_indices[6] = 3;
	M1_indices[7] = 2;
	M1_indices[8] = 6;
	// Up triangle
	M1_indices[9] = 3;
	M1_indices[10] = 6;
	M1_indices[11] = 7;

	//left triangle
	M1_indices[12] = 4 + 8;
	M1_indices[13] = 7 + 8;
	M1_indices[14] = 0 + 8;
	//left triangle
	M1_indices[15] = 3 + 8;
	M1_indices[16] = 7 + 8;
	M1_indices[17] = 0 + 8;

	//Right triangle
	M1_indices[18] = 2 + 8;
	M1_indices[19] = 1 + 8;
	M1_indices[20] = 6 + 8;
	//Right triangle
	M1_indices[21] = 1 + 8;
	M1_indices[22] = 5 + 8;
	M1_indices[23] = 6 + 8;

	//Back triangle
	M1_indices[24] = 0 + 16;
	M1_indices[25] = 1 + 16;
	M1_indices[26] = 3 + 16;
	//Back triangle
	M1_indices[27] = 3 + 16;
	M1_indices[28] = 2 + 16;
	M1_indices[29] = 1 + 16;

	//Front triangle
	M1_indices[30] = 4 + 16;
	M1_indices[31] = 5 + 16;
	M1_indices[32] = 7 + 16;
	//Fronttriangle
	M1_indices[33] = 5 + 16;
	M1_indices[34] = 6 + 16;
	M1_indices[35] = 7 + 16;



	//// M2 : Cylinder
	// Replace the code below, that creates a simple rotated square, with the one to create a cylinder.
	int NSlices = 32;
	float radius = 0.5;
	float height = 1.5;
	float cx = 0, cy = 0, cz = 0;

	// Resizes the vertices array. Repalce the values with the correct number of
	// vertices
	M2_vertices.resize((NSlices + 1) * 4 +2); //Nslice +1 = circle 
	//2+ ->  initial and final point of the side of the can must we overlap to allow a uniform texture
	//*4 -> for each circonference i have to specify twice the vertexes

// Vertices definitions
	M2_vertices[0].pos = glm::vec3(cx, cy, cz);
	M2_vertices[0].norm = glm::vec3(0.0, -1.0, 0.0);
	M2_vertices[0].UV = glm::vec2(0.875, 0.125);

	int SC = (NSlices + 1);
	M2_vertices[SC].pos = glm::vec3(cx, cy + height, cz);
	M2_vertices[SC].norm = glm::vec3(0.0, +1.0, .0);
	M2_vertices[SC].UV = glm::vec2(0.625, 0.125);

	//lower circle
	for (int i = 0; i < NSlices; i++) {
		M2_vertices[(i + 1)].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy, cz + radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[(i + 1)].norm = glm::vec3(0.0, -1.0, .0);
		M2_vertices[(i + 1) + SC].UV = glm::vec2(0.875 + 0.125 * cos((float)i / NSlices * 2 * M_PI), 0.125 + 0.125 * sin((float)i / NSlices * 2 * M_PI));

	}
	//upper circle
	for (int i = 0; i < NSlices; i++) {
		M2_vertices[(i + 1) + SC].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy + height, cz + radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[(i + 1) + SC].norm = glm::vec3(0.0, 1.0, .0);
		M2_vertices[(i + 1)+ SC].UV = glm::vec2(0.625 + 0.125 * cos((float)i / NSlices * 2 * M_PI), 0.125 + 0.125* sin((float)i / NSlices * 2 * M_PI));
	}


	int SC2 = (2 * NSlices + 2);
	//lower wall
	for (int i = 0; i < NSlices; i++) {
		M2_vertices[i + SC2].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy, cz + radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[i + SC2].norm = glm::vec3(cos((float)i / NSlices  * 2 * M_PI), 0, radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[i + SC2].UV = glm::vec2(0.5 + 0.5 * sin((float)i / (NSlices) * M_PI / 2), 0.5); //use the lenght function of the circumference chord
	}

	M2_vertices[NSlices + SC2].pos = M2_vertices[SC2].pos;
	M2_vertices[NSlices + SC2].norm = M2_vertices[SC2].norm;
	M2_vertices[NSlices + SC2].UV = glm::vec2(1, 0.5);



	int SC3 = (3 * NSlices + 3);
	//upper wall
	for (int i = 0; i < NSlices; i++) {
		M2_vertices[i + SC3].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy + height, cz + radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[i + SC3].norm = glm::vec3(cos((float)i / NSlices * 2 * M_PI), 0, radius * sin((float)i / NSlices * 2 * M_PI));
		M2_vertices[i + SC3].UV = glm::vec2(0.5 + 0.5 * sin((float)i / (NSlices) * M_PI/2), 0.25); //use the lenght function of the circumference chord
	}

	M2_vertices[NSlices + SC3].pos = M2_vertices[SC3].pos;
	M2_vertices[NSlices + SC3].norm = M2_vertices[SC3].norm;
	M2_vertices[NSlices + SC3].UV = glm::vec2(1, 0.25);


	// Resizes the indices array. Repalce the values with the correct number of
	// indices (3 * number of triangles)
	M2_indices.resize((3 * NSlices) * 2 + (3 * NSlices) * 2);

	//lower Circle
	for (int i = 0; i < NSlices; i++) {
		M2_indices[i * 3 + 0] = 0;
		M2_indices[i * 3 + 1] = i + 1;
		M2_indices[i * 3 + 2] = (i + 1) % NSlices + 1;
	}

	//Upper Circle
	for (int i = 0; i < NSlices; i++) {
		M2_indices[i * 3 + 0 + (3 * NSlices)] = NSlices + 1;
		M2_indices[i * 3 + 1 + (3 * NSlices)] = i + 1 + NSlices + 1;
		M2_indices[i * 3 + 2 + (3 * NSlices)] = (i + 1) % NSlices + NSlices + 2;
	}
	//paretesis element are used to shift between the 2 different kind of circle (changed the normale vector)

	//lower based triangle
	int Rec = (3 * NSlices) * 2;
	for (int i = 0; i < NSlices; i++) {
		M2_indices[i * 3 + 0 + Rec] = i + 1 + (2 * NSlices + 1);
		M2_indices[i * 3 + 1 + Rec] = i + 2 + (2 * NSlices + 1);
		M2_indices[i * 3 + 2 + Rec] = i + 3 + NSlices + (2 * NSlices);
	}

	//upper based triangle
	int Rec2 = (3 * NSlices) * 3;
	for (int i = 0; i < NSlices; i++) {
		M2_indices[i * 3 + 0 + Rec2] = i + 4 + NSlices + (2 * NSlices);
		M2_indices[i * 3 + 1 + Rec2] = i + 3 + NSlices  + (2 * NSlices);
		M2_indices[i * 3 + 2 + Rec2] = i + 2 + (2 * NSlices + 1);
	}

}