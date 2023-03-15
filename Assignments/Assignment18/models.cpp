/*
meshes are polygonal objects with sharp edges that, with special rendering techniques, can appear smooth.
The rendering equation is very sensitive to the direction of the normal vector. 
he basis of smooth rendering is to “fake” the geometrical normal vector, and use an artificial one
The Gouraud (per vertex) shading technique interpolate on vertex color
In Phong shading alogirthm The vertex normal vectors are interpolated to approximate the normal vector

*/







// this function creates the geometries to be shown, and output thems
// in global variables M1_vertices and M1_indices to M4_vertices and M4_indices

#include <corecrt_math_defines.h>
struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
};

std::vector<Vertex> M1_vertices;
std::vector<Vertex> M2_vertices;
std::vector<Vertex> M3_vertices;
std::vector<Vertex> M4_vertices;

//y -> up
//x -> right
//z -> front


void makeModels() {
//// M1 : Cube
// Replace the code below, that creates a simple square, with the one to create a cube.

// Resizes the vertices array. Repalce the values with the correct number of
// vertices
M1_vertices.resize(8*3);

//n=0 vertex of M1
M1_vertices[0].pos = glm::vec3(0.0, 0.0, 0.0); 
M1_vertices[0].norm = glm::vec3(0.0,-1.0,0.0); //down

// n=1 vertex of M1
M1_vertices[1].pos = glm::vec3(1.0, 0.0, 0.0);
M1_vertices[1].norm = glm::vec3(0.0, -1.0, 0.0);//down

// n=2 vertex of M1
M1_vertices[2].pos = glm::vec3(1.0, 1.0, 0.0);
M1_vertices[2].norm = glm::vec3(0.0, 1.0, 0.0); //up

// n=3 vertex of M1
M1_vertices[3].pos = glm::vec3(0.0, 1.0, 0.0);
M1_vertices[3].norm = glm::vec3(0.0, 1.0, 0.0); //up

// n=4 vertex of M1
M1_vertices[4].pos = glm::vec3(0.0, 0.0, 1.0);
M1_vertices[4].norm = glm::vec3(0.0, -1.0, 0.0); //down

// n=5 vertex of M1
M1_vertices[5].pos = glm::vec3(1.0, 0.0, 1.0);
M1_vertices[5].norm = glm::vec3(0.0, -1.0, 0.0); //down

// n=6 vertex of M1
M1_vertices[6].pos = glm::vec3(1.0, 1.0, 1.0);
M1_vertices[6].norm = glm::vec3(0.0, 1.0, 0.0); //up

// n=7 vertex of M1
M1_vertices[7].pos = glm::vec3(0.0, 1.0, 1.0);
M1_vertices[7].norm = glm::vec3(0.0, 1.0, 0.0); //up

/*-----------------------------------------*/

//n=0A vertex of M1
M1_vertices[0 + 8].pos = glm::vec3(0.0, 0.0, 0.0);
M1_vertices[0 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left

// n=1A vertex of M1
M1_vertices[1 + 8].pos = glm::vec3(1.0, 0.0, 0.0);
M1_vertices[1 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right

// n=2A vertex of M1
M1_vertices[2 + 8].pos = glm::vec3(1.0, 1.0, 0.0);
M1_vertices[2 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right

// n=3A vertex of M1
M1_vertices[3 + 8].pos = glm::vec3(0.0, 1.0, 0.0);
M1_vertices[3 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left

// n=4A vertex of M1
M1_vertices[4 + 8].pos = glm::vec3(0.0, 0.0, 1.0);
M1_vertices[4 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left

// n=5A vertex of M1
M1_vertices[5 + 8].pos = glm::vec3(1.0, 0.0, 1.0);
M1_vertices[5 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right

// n=6A vertex of M1
M1_vertices[6 + 8].pos = glm::vec3(1.0, 1.0, 1.0);
M1_vertices[6 + 8].norm = glm::vec3(1.0, 0.0, 0.0); //right

// n=7A vertex of M1
M1_vertices[7 + 8].pos = glm::vec3(0.0, 1.0, 1.0); 
M1_vertices[7 + 8].norm = glm::vec3(-1.0, 0.0, 0.0); //left

/*-----------------------------------------*/

//n=0B vertex of M1
M1_vertices[0 + 16].pos = glm::vec3(0.0, 0.0, 0.0);
M1_vertices[0 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back

// n=1B vertex of M1
M1_vertices[1 + 16].pos = glm::vec3(1.0, 0.0, 0.0);
M1_vertices[1 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back

// n=2B vertex of M1
M1_vertices[2 + 16].pos = glm::vec3(1.0, 1.0, 0.0);
M1_vertices[2 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back

// n=3B vertex of M1
M1_vertices[3 + 16].pos = glm::vec3(0.0, 1.0, 0.0);
M1_vertices[3 + 16].norm = glm::vec3(0.0, 0.0, -1.0); //back

// n=4B vertex of M1
M1_vertices[4 + 16].pos = glm::vec3(0.0, 0.0, 1.0);
M1_vertices[4 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front

// n=5B vertex of M1
M1_vertices[5 + 16].pos = glm::vec3(1.0, 0.0, 1.0);
M1_vertices[5 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front

// n=6B vertex of M1
M1_vertices[6 + 16].pos = glm::vec3(1.0, 1.0, 1.0);
M1_vertices[6 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front

// n=7B vertex of M1
M1_vertices[7 + 16].pos = glm::vec3(0.0, 1.0, 1.0); 
M1_vertices[7 + 16].norm = glm::vec3(0.0, 0.0, 1.0); //front


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

int NSlices = 16;
float radius = 1;
float height = 1.5;
float cx = 0, cy = 0, cz = -2;

// Resizes the vertices array. Repalce the values with the correct number of
// vertices
M2_vertices.resize((NSlices + 1) * 4); 
//Nslice +1 = circle. 2 used for the normal vector of up and down circle, 2 for the lateral area

//0: middle down -> circle[1,NS] -> NS+1: middle up -> [NS+2, 2NS+1], [2NS+2, 3ns+1], [3NS+2,4NS+1]
//c,0,0,0,0,C,1,1,1,1,2,2,2,2,3,3,3,3

// Vertices definitions
M2_vertices[0].pos  = glm::vec3(cx,cy,cz);
M2_vertices[0].norm = glm::vec3(0.0,-1.0,0.0);

int SC = (NSlices+1);
M2_vertices[SC].pos  = glm::vec3(cx,cy+height,cz);
M2_vertices[SC].norm = glm::vec3(0.0,+1.0,.0);

//lower circle
for (int i = 0; i < NSlices; i++) {
	M2_vertices[(i + 1)].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy, cz + radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[(i + 1)].norm = glm::vec3(0.0, -1.0, .0); 

}
//upper circle
for (int i = 0; i < NSlices; i++) {
	M2_vertices[(i + 1) + SC].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy + height, cz + radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[(i + 1) + SC].norm = glm::vec3(0.0,1.0, .0);
}

int SC2 = (2*NSlices + 2);
//lower wall
for (int i = 0; i < NSlices; i++) {
	M2_vertices[i  + SC2].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy, cz + radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[i  + SC2].norm = glm::vec3(cos((float)i / NSlices * 2 * M_PI), 0, radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[i + SC2].norm = normalize(M2_vertices[i + SC2].norm); //in principle already normalized
}

int SC3 = (3 * NSlices+2);
//lower wall
for (int i = 0; i < NSlices; i++) {
	M2_vertices[i + SC3].pos = glm::vec3(cx + radius * cos((float)i / NSlices * 2 * M_PI), cy + height, cz + radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[i  + SC3].norm = glm::vec3(cos((float)i / NSlices * 2 * M_PI), 0, radius * sin((float)i / NSlices * 2 * M_PI));
	M2_vertices[i + SC3].norm = normalize(M2_vertices[i + SC3].norm);//in principle already normalized
}



// Resizes the indices array. Repalce the values with the correct number of
// indices (3 * number of triangles)
//LOWER AND UPPER CIRCLE + 2* LATERAL TRIANGLE 
M2_indices.resize((3 * NSlices) * 2 + (3 * NSlices) * 2);

//lower Circle
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0] = 0 ;
	M2_indices[i * 3 + 1] = i + 1 ;
	M2_indices[i * 3 + 2] = (i + 1) % NSlices + 1;
}

//Upper Circle
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + (3 * NSlices)] = NSlices + 1;
	M2_indices[i * 3 + 1 + (3 * NSlices)] = i + 1 + NSlices + 1 ;
	M2_indices[i * 3 + 2 + (3 * NSlices)] = (i + 1) % NSlices + NSlices + 2 ;
}
//paretesis element are used to shift between the 2 different kind of circle (changed the normale vector)

//lower based triangle
int Rec = (3 * NSlices) * 2;
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + Rec] = i + 1 + (2*NSlices+1) ;
	M2_indices[i * 3 + 1 + Rec] = (i + 1) % NSlices + 1 + (2*NSlices + 1);
	M2_indices[i * 3 + 2 + Rec] = (i + 1) % NSlices + NSlices + 2 + (2*NSlices);
}
//upper based triangle
int Rec2 = (3 * NSlices) * 3;
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + Rec2] = i + 2 + NSlices + (2 * NSlices);
	M2_indices[i * 3 + 1 + Rec2] = (i + 1) % NSlices + NSlices + 2 + (2 * NSlices);
	M2_indices[i * 3 + 2 + Rec2] = i % NSlices + 1 + (2 * NSlices+1);
}

//// M3 : Sphere
// Replace the code below, that creates a simple triangle, with the one to create a sphere.

int NS = 32; //number of point for each layer 
int NL = 32; // number of layer
float rad = 1;
float cx2 = 0, cy2 = 0, cz2 = 0;


// Resizes the vertices array. Repalce the values with the correct number of vertices
//FOR N LAYER I HAVE N-1 "FULL LAYER"
M3_vertices.resize((2 + (NL - 1) * NS));

// Vertices definitions
M3_vertices[0].pos = glm::vec3(cx2, cy2 + rad, cz2);
M3_vertices[0].norm = glm::vec3(0.0, 1.0, 0.0);

M3_vertices[(1 + (NL - 1) * NS)].pos = glm::vec3(cx2, cy2 - rad, cz2);
M3_vertices[(1 + (NL - 1) * NS)].norm = glm::vec3(0.0, -1.0, 0.0);

int k = 0;
for (int i = 1; i < NL; i++) {
	//std::cout << "START "<<i <<" :\n";
	for (int j = 0; j < NS; j++) {
		k++;
		M3_vertices[k].pos = glm::vec3( cx2 + rad * sin((float)i / NL * M_PI) * cos((float)j / NS * 2 * M_PI), cy2 + rad * cos((float)i / NL * M_PI), cz2 + rad * sin((float)i / NL * M_PI) * sin((float)j / NS * 2 * M_PI));
		M3_vertices[k].norm = glm::vec3(sin((float)i / NL * M_PI) * cos((float)j / NS * 2 * M_PI), cos((float)i / NL * M_PI), sin((float)i / NL * M_PI) * sin((float)j / NS * 2 * M_PI));
		M3_vertices[k].norm = normalize(M3_vertices[k].norm);//in principle already normalized
	}
	//std::cout << "END : \n";
}



// Resizes the indices array. Repalce the values with the correct number of
// indices (3 * number of triangles)
//for each layer NL i have NS*2 triangle (first and last layer only 1 line of triangles)
M3_indices.resize(3 * ((NL - 1) * NS * 2));

//build the first layer
for (int i = 0; i < NS; i++) {
	M3_indices[i * 3 + 0] = 0;
	M3_indices[i * 3 + 1] = i + 1;
	M3_indices[i * 3 + 2] = (i + 1) % NS + 1;
}

int lastVertex = ((2 + (NL - 1) * NS) * 3 - 3) / 3;
int startLastRow = 3 * ((NL - 1) * (NS * 2)) - NS * 3;

//build the last layer
for (int i = 0; i < NS; i++) {
	M3_indices[startLastRow + i * 3 + 0] = lastVertex;
	M3_indices[startLastRow + i * 3 + 1] = lastVertex - NS + i;
	M3_indices[startLastRow + i * 3 + 2] = lastVertex - NS + ((i + 1) % (NS));
}
//build the middle layer
k = 0;
for (int j = 1; j < NL - 1; j++) {
	for (int i = 0; i < NS; i++) {
		M3_indices[k * 6 + 0 + NS * 3] = i + 1 + NS * (j - 1);
		M3_indices[k * 6 + 1 + NS * 3] = (i + 1) % NS + 1 + NS * (j - 1);
		M3_indices[k * 6 + 2 + NS * 3] = i + 1 + NS * j;

		M3_indices[k * 6 + 3 + NS * 3] = i + 1 + NS * j;
		M3_indices[k * 6 + 4 + NS * 3] = (i + 1) % NS + 1 + NS * (j - 1);
		M3_indices[k * 6 + 5 + NS * 3] = (i + 1) % NS + 1 + NS * j;
		k++;
	}
}
//// M4 : Spring
// Replace the code below, that creates a simple octahedron, with the one to create a spring.

float rad4 = 1;	//radius of the Spring
float height4 = 0.7;  //distance between 2 layer of the sping (middle - middle)
int NS4 = 16;		//number of point for each layer of the Spring
int NL4 = 4;		//number of layer of the Spring

float rad4c = 0.2;	//radius of the circles
int NS4c = 16;		//Number of points of the circles

int cx4 = 0, cz4 = 0;
float cy4 = -(height4 * NL4 / 2); //to center the Spring


// Vertices definitions
//Number of spire * point of a spire * point of the circonference build on top of the spire
M4_vertices.resize(NS4 * NL4 * NS4c);
//single point of the spire
std::vector<Vertex> WorkPoint;
WorkPoint.resize(NS4 * NL4);

//create the point of the Spring. These points are useless for the final construction of the object, but are helpful to built the figure
int k4 = 0;
for (int j = 0; j < NL4; j++) {
	for (int i = 0; i < NS4; i++) {
		WorkPoint[k4].pos = glm::vec3(cx4 + rad4 * cos((float)i / NS4 * 2 * M_PI), cy4 + height4 * i / NS4 + (j)*height4, cz4 + rad4 * sin((float)i / NS4 * 2 * M_PI));
		k4++;
	}

}
//create a circle for each point of the Spring
k4 = 0;
for (int i = 0; i < NL4 * NS4; i++) {
	for (int j = 0; j < NS4c; j++) {
		M4_vertices[k4].pos = glm::vec3(WorkPoint[i].pos[0] + rad4c * cos((float)j / NS4c * 2 * M_PI) * cos(((float)(i % NS4) / NS4) * 2 * M_PI), WorkPoint[i].pos[1] + rad4c * sin((float)j / NS4c * 2 * M_PI), WorkPoint[i].pos[2] + rad4c * cos((float)j / NS4c * 2 * M_PI) * sin(((float)(i % NS4) / NS4) * 2 * M_PI));
		M4_vertices[k4].norm = glm::vec3(cos((float)j / NS4c * 2 * M_PI) * cos(((float)(i % NS4) / NS4) * 2 * M_PI), sin((float)j / NS4c * 2 * M_PI), cos((float)j / NS4c * 2 * M_PI) * sin(((float)(i % NS4) / NS4) * 2 * M_PI));
		M4_vertices[k4].norm = normalize(M4_vertices[k4].norm);//in principle already normalized
		k4++;
	}
}

// Resizes the indices array. Repalce the values with the correct number of
// indices (3 * number of triangles)
//THE MINUS 1 IS USED BECAUSE THE LAST SET OF VERTICES CAN'T CONNECT TO THE "NEXT ONE"
M4_indices.resize(2 * 3 * (NS4 * NL4-1) * NS4c);

//create a "tube" between 2 consecutive circles
k4 = 0;
for (int i = 0; i < (NS4 * NL4 - 1); i++) {
	for (int j = 0; j < NS4c; j++) {

		M4_indices[k4 * 6 + 0] = (i * NS4c) + j;
		M4_indices[k4 * 6 + 1] = (i * NS4c) + (j + 1) % NS4c;
		M4_indices[k4 * 6 + 2] = ((i + 1) * NS4c) + j;

		M4_indices[k4 * 6 + 3] = ((i + 1) * NS4c) + j;
		M4_indices[k4 * 6 + 4] = ((i + 1) * NS4c) + (j + 1) % NS4c;
		M4_indices[k4 * 6 + 5] = (i * NS4c) + (j + 1) % NS4c;
		k4++;
	}
}

}