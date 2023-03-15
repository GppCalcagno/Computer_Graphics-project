/*
Each polygon can be reduced to a set of triangles that shares some edges. A set of adjacent triangles is called a Mesh
Triangle lists encode each triangle as a set of three different coordinates.
Triangle strips encode a set of adjacent triangles that define a band-like surface.
Indexed primitives are defined by two arrays: the vertex array contains the position of the vertexs and Index Array contains the tiangle build on top of vertices

*/

// this function creates the geometries to be shown, and output thems
// in global variables M1_vertices and M1_indices to M4_vertices and M4_indices
#include <corecrt_math_defines.h>
void makeModels() {

//// M1 : Cube-------------------------------------------------------------------------------------------------------------
// Replace the code below, that creates a simple square, with the one to create a cube.

// Resizes the vertices array. Repalce the values with the correct number of
// 8 vertices of a cube 
// vertices components (3 * number of vertices)
M1_vertices.resize(3 * 8);

// n=0  vertex of M1
M1_vertices[0] = 0;
M1_vertices[1] = 0;
M1_vertices[2] = 0;

// n=1 vertex of M1
M1_vertices[3] = 1.0;
M1_vertices[4] = 0;
M1_vertices[5] = 0;

// n=2 vertex of M1
M1_vertices[6] = 1.0;
M1_vertices[7] = 1.0;
M1_vertices[8] = 0;

// n=3 vertex of M1
M1_vertices[9] =  0;
M1_vertices[10] = 1.0;
M1_vertices[11] = 0;

// n=4 vertex of M1
M1_vertices[12] = 0;
M1_vertices[13] = 0;
M1_vertices[14] = 1.0;


// n=5 vertex of M1
M1_vertices[15] = 1.0;
M1_vertices[16] = 0;
M1_vertices[17] = 1.0;

// n=6 vertex of M1
M1_vertices[18] = 1.0;
M1_vertices[19] = 1.0;
M1_vertices[20] = 1.0;

// n=7 vertex of M1
M1_vertices[21] = 0;
M1_vertices[22] = 1.0;
M1_vertices[23] = 1.0;



// Resizes the indices array. Repalce the values with the correct number of
// indices (3 * number of triangles)
M1_indices.resize(3 *2* 6);  //6 faces made of 2 triangle (3)

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
M1_indices[12] = 4;
M1_indices[13] = 7;
M1_indices[14] = 0;
//left triangle
M1_indices[15] = 3;
M1_indices[16] = 7;
M1_indices[17] = 0;

//Right triangle
M1_indices[18] = 2;
M1_indices[19] = 1;
M1_indices[20] = 6;
//Right triangle
M1_indices[21] = 1;
M1_indices[22] = 5;
M1_indices[23] = 6;

//Back triangle
M1_indices[24] = 0;
M1_indices[25] = 1;
M1_indices[26] = 3;
//Back triangle
M1_indices[27] = 3;
M1_indices[28] = 2;
M1_indices[29] = 1;

//Front triangle
M1_indices[30] = 4;
M1_indices[31] = 5;
M1_indices[32] = 7;
//Fronttriangle
M1_indices[33] = 5;
M1_indices[34] = 6;
M1_indices[35] = 7;


//// M2 : Cylinder ----------------------------------------------------------------------------------------------------------------
// Replace the code below, that creates a simple rotated square, with the one to create a cylinder.

int NSlices =32; 
float radius = 1;
float height = 1.5;
float cx = 0, cy=0, cz = -2;

// Resizes the vertices array. Repalce the values with the correct number of vertices components (3 * number of vertices)
M2_vertices.resize(((NSlices + 1)*3) *2);
//the vertex for 2 circle (NSlices + Middle) * (x,y,z) * 2 

M2_vertices[0] = cx ;
M2_vertices[1] = cy ;
M2_vertices[2] = cz ;

int SC = ((NSlices + 1) * 3);
M2_vertices[SC + 0] = cx;
M2_vertices[SC + 1] = cy +height;
M2_vertices[SC + 2] = cz;


for (int i = 0; i < NSlices; i++) {
	M2_vertices[(i + 1) * 3 + 0] = cx + radius * cos((float)i / NSlices * 2 * M_PI);	//x 
	M2_vertices[(i + 1) * 3 + 1] = M2_vertices[1];										//y	
	M2_vertices[(i + 1) * 3 + 2] = cz +radius * sin((float)i / NSlices * 2 * M_PI);	//z
}

for (int i = 0; i < NSlices; i++) {
	M2_vertices[(i + 1) * 3 + 0 + SC] = cx + radius * cos((float)i / NSlices * 2 * M_PI);	//x 
	M2_vertices[(i + 1) * 3 + 1 + SC] = M2_vertices[SC+ 1];									//y	
	M2_vertices[(i + 1) * 3 + 2 + SC] = cz + radius * sin((float)i / NSlices * 2 * M_PI);	//z
}

// Resizes the indices array. Repalce the values with the correct number of  indices (3 * number of triangles)
M2_indices.resize((3*NSlices)*2 + (3*NSlices) *2);
//(3*NSlices)*2 for 2 circle
//(3*NSlices)*2 for the rectangle 

//0 = lower middle , [1,NSlices] = lower Circle
//NSlices +1 = upper middle, [NSlices +2, 2*NSlices+1] = upper Circle

//lower Circle
for (int i = 0; i < NSlices; i++) {
	M2_indices[i  * 3 + 0] = 0; 
	M2_indices[i  * 3 + 1] = i + 1;
	M2_indices[i  * 3 + 2] = (i + 1) % NSlices + 1;
}

//Upper Circle
int SC2 = (3 * NSlices);
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + SC2] = NSlices+ 1;
	M2_indices[i * 3 + 1 + SC2] = i + 1 + NSlices+ 1;
	M2_indices[i * 3 + 2 + SC2] = (i + 1) % NSlices + NSlices + 2;
}
//lower based triangle
int Rec = (3 * NSlices) * 2;
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + Rec] = i + 1;
	M2_indices[i * 3 + 1 + Rec] = (i + 1) % NSlices +1;
	M2_indices[i * 3 + 2 + Rec] = (i + 1) % NSlices + NSlices +2;
}
//upper based triangle
int Rec2 = (3 * NSlices) * 3;
for (int i = 0; i < NSlices; i++) {
	M2_indices[i * 3 + 0 + Rec2] = i +2 + NSlices;
	M2_indices[i * 3 + 1 + Rec2] = (i + 1) % NSlices + NSlices + 2;
	M2_indices[i * 3 + 2 + Rec2] = i % NSlices +1;
}


//// M3 : Sphere -------------------------------------------------------------------------------------------------------------
// Replace the code below, that creates a simple triangle, with the one to create a sphere.
// Resizes the vertices array. Repalce the values with the correct number of vertices components (3 * number of vertices)

int NS = 32; //number of point for each layer 
int NL = 32; // number of layer
float rad = 1;
float cx2 = 0,cy2 = 0, cz2 = 0;

//consider a sphere made of NL layers  and NS slice.
//FOR N LAYER I HAVE N-1 "FULL LAYER" counting the orizzontal lines
M3_vertices.resize((2 + (NL-1) * NS) * 3);

// Vertices definitions
M3_vertices[0] = cx2;
M3_vertices[1] = cy2 + rad;
M3_vertices[2] = cz2;

M3_vertices[(2 + (NL - 1) * NS) * 3 -3] = cx2;
M3_vertices[((2 + (NL - 1) * NS) * 3) - 2] = cy2 - rad;
M3_vertices[((2 + (NL - 1) * NS) * 3) - 1] = cz2;

int k = 0;
//THIS START FROM ONE TO START DRAWING CIRCLE A LAYER BELOW, AND I END A LAYER BEFORE TO STOP A LAYER ABOVE 
for (int i = 1; i < NL; i++) {
	//std::cout << "START "<<i <<" :\n";
	for (int j = 0; j < NS; j++) {
		k++;
		M3_vertices[k * 3 + 0] = cx2 + rad *	sin((float)i / NL *M_PI)		*	cos((float)j / NS  *2* M_PI);
		M3_vertices[k * 3 + 1] = cy2 + rad *	cos((float)i / NL* M_PI); //locked
		M3_vertices[k * 3 + 2] = cz2 + rad *	sin((float)i / NL * M_PI)	*	 sin((float)j / NS  *2* M_PI);
		//std::cout <<k<<" : " << M3_vertices[k * 3 + 0] << " -\t\t" << M3_vertices[k * 3 + 1] << " -\t\t" << M3_vertices[k * 3 + 2] << " \n";
	}
	//std::cout << "END : \n";
}

// Resizes the indices array. Repalce the values with the correct number of  indices (3 * number of triangles)
// indices definitions
//for each layer NL i have NS*2 triangle  (first and last layer only 1 line of triangles)
M3_indices.resize(3*((NL-1)*NS*2));

//build the first layer
for (int i = 0; i < NS; i++) {
	M3_indices[i * 3 + 0] = 0;
	M3_indices[i * 3 + 1] = i+1;
	M3_indices[i * 3 + 2] = (i+1) % NS+ 1;
}

int lastVertex = ((2 + (NL - 1) * NS) * 3 - 3) / 3;
int startLastRow = 3 * ((NL - 1) * (NS * 2)) - NS * 3;

//build the last layer
for (int i = 0; i < NS; i++) {
	M3_indices[startLastRow +i * 3 + 0] = lastVertex;
	M3_indices[startLastRow +i * 3 + 1] = lastVertex -NS +i;
	M3_indices[startLastRow +i * 3 + 2] = lastVertex -NS + ((i + 1) %(NS));
}
//build the middle layer
k = 0;
for (int j = 1; j < NL-1; j++) {
	for (int i = 0; i < NS; i++) {
		M3_indices[k * 6 + 0 + NS*3] = i+1  + NS * (j-1);
		M3_indices[k * 6 + 1 + NS*3] = (i + 1) %NS +1 + NS * (j - 1);
		M3_indices[k * 6 + 2 + NS*3] = i + 1 + NS*j;

		M3_indices[k * 6 + 3 + NS * 3] = i + 1 + NS * j;
		M3_indices[k * 6 + 4 + NS * 3] = (i + 1) % NS + 1 + NS * (j-1);
		M3_indices[k * 6 + 5 + NS * 3] = (i + 1) % NS + 1 + NS * j;
		k++;
	}
}

//// M4 : Spring -------------------------------------------------------------------------------------------------------------

float rad4 = 1;	//radius of the Spring
float height4 = 0.7;  //distance between 2 layer of the sping (middle - middle)
int NS4 = 32;		//number of point for each layer of the Spring
int NL4 = 8;		//number of layer of the Spring

float rad4c = 0.2;	//radius of the circles
int NS4c = 8;		//Number of points of the circles

int cx4 = 0, cz4 = 0;
float cy4 = -(height4 * NL4 / 2) ; //to center the Spring

// Replace the code below, that creates a simple octahedron, with the one to create a spring.
//Number of spire * point of a spire * point of the circonference build on top of the spire
M4_vertices.resize(3*(NL4*NS4*NS4c));

//single point of the spire
std::vector<float> WorkPoint;
WorkPoint.resize(3 * (NS4 * NL4));
//useless for the final images
//create the point of the Spring
int k4 = 0;

//Draw the middle point of the spring
for (int j = 0; j < NL4; j++) {
	for (int i = 0; i < NS4; i++) {
		WorkPoint[k4 * 3 + 0] = cx4 + rad4 * cos((float)i / NS4 * 2 * M_PI);
		WorkPoint[k4 * 3 + 1] = cy4 + height4 * i / NS4  + (j)*height4;
		WorkPoint[k4 * 3 + 2] = cz4 + rad4 * sin((float)i / NS4 * 2 * M_PI);
		k4++;
	}
}
//create a oriented circonference for each point of the Spring
k4 = 0;
for (int i = 0; i < NL4 * NS4; i++) {
	for (int j = 0; j < NS4c; j++) {
		M4_vertices[k4 * 3 + 0] = WorkPoint[i * 3 + 0] + rad4c * cos((float)j / NS4c * 2 * M_PI) * cos(((float)(i% NS4) / NS4) *2* M_PI);
		M4_vertices[k4 * 3 + 1] = WorkPoint[i * 3 + 1] + rad4c * sin((float)j / NS4c * 2 * M_PI);
		M4_vertices[k4 * 3 + 2] = WorkPoint[i * 3 + 2] + rad4c * cos((float)j / NS4c * 2 * M_PI) * sin(((float)(i % NS4) / NS4) *2* M_PI);
		k4++; 
	}
}

// Vertices definitions
// Resizes the indices array. Repalce the values with the correct number of
// indices (3 * number of triangles)
//THE MINUS 1 IS USED BECAUSE THE LAST SET OF VERTICES CAN'T CONNECT TO THE "NEXT ONE"
M4_indices.resize(2 * 3 * (NS4 * NL4-1) * NS4c);

//create a "tube" between 2 consecutive circles
k4 = 0;
for (int i = 0; i < (NS4 * NL4-1); i++) {
	for (int j = 0; j < NS4c; j++) {

		M4_indices[k4 * 6 + 0] = (i * NS4c) + j;
		M4_indices[k4 * 6 + 1] = (i * NS4c) + (j + 1) % NS4c;
		M4_indices[k4 * 6 + 2] = ((i + 1) * NS4c) + j;

		M4_indices[k4 * 6 + 3] =((i + 1) * NS4c) + j;
		M4_indices[k4 * 6 + 4] =((i + 1) * NS4c) + (j + 1) % NS4c;
		M4_indices[k4 * 6 + 5] = (i * NS4c) + (j+1)% NS4c;
		k4++;
	}
}
}