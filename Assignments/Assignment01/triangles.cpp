/**************
 Function Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)

 draws a triangle connecting points (x1,y1)  (x2, y2) and (x3, y3), colored with the (r,g,b) specified color.
 vertex are expressed in Normalized screen coordinates, (i.e. in range [-1,+1]), following the Vulkan convention.
 The red, green and blue (r,g,b) values are in the range [0,1].
 
 Using function Triangle(...) try to draw a children house, similar to the one below.
 
 First remove the two function below (given only as an example), and replace them with your own.
 Since it is a C program, you can use for loops if you think it can be helpful in your drawing.
 The for() command below that draws a sun like shape in exact center of the screen near the top,
 is an example on how you can use it: please remove it, and replace it with your own code.
 It is not necessery to use a for loop if you do not really need it: it has been put here only as
 an example to help students not familiar with C++.
***************/

//here Normalized Screen Coordinates are used -> top Left [x=-1,y=-1] x:to right ,y:to down  
//instead screen uses pixel coordinates -> [0,pixel-1] but same "orientation"


//this is a lamda function to draw circle
auto circle = [&](float x, float y, float sizeC, float r, float g, float b) {
	for (int i = 0; i < 32; i++) {
		Triangle(x + sizeC * cos(6.28 / 32 * (i - 0.5)), y + sizeC * sin(6.28 / 32 * (i - 0.5)),
			x, y,
			x + sizeC * cos(6.28 / 32 * (i + 0.5)), y + sizeC * sin(6.28 / 32 * (i + 0.5)),
			r, g, b);
	}
};
//this is a lamda function to draw rectengle
auto rectangle = [&](float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b) {
	// (x3,y3) and (x4,y4) must be the different points
	Triangle(x1, y1, x2, y2, x3, y3, r, g, b);
	Triangle(x1, y1, x2, y2, x4, y4, r, g, b);
};
//this is a lamda function to draw clouds
//scale is used to modify the dimension of the clouds 
//this imply also the modification of the space between the 3 circle that buil the cloud, so not all value of scale are feasible 
auto cloud = [&](float x, float y, float scale, float r, float g, float b) {
	circle(x, y, 0.1 * scale, r, g, b);
	circle(x - 0.1*scale, y, 0.09 * scale , r, g, b);
	circle(x + 0.12 * scale, y - 0.025 * scale,0.1*scale, r, g, b);
};

rectangle(-1, 1, 1, -1, -1,-1, 1,1,    1, 1, 1); //hide background


// house BLUE
rectangle(0.05, 0.2, 0.65, 0.8, 0.05, 0.8, 0.65, 0.2, 0, 0, 1);
// ROOF
Triangle(-0.05, 0.2, 0.35, -0.45, 0.75, 0.2,   1,0,0); 
// DOOR
rectangle(0.3, 0.5, 0.4, 0.8, 0.4, 0.5, 0.3, 0.8, 1, 1, 0);
// left WINDOWS 
rectangle(0.15, 0.25, 0.25, 0.35, 0.15, 0.35, 0.25, 0.25, 1, 1, 0);
// right WINDOWS 
rectangle(0.45, 0.25, 0.55, 0.35, 0.45, 0.35, 0.55, 0.25, 1, 1, 0);
// Grass
rectangle(-1, 1, 1, 0.8, -1, 0.8, 1, 1, 0, 1, 0);

//right  Flower
Triangle(0.74,0.8, 0.76,0.8, 0.75,0.7,	0,1,0);
circle(0.75, 0.67, 0.06,  1, 0.5, 0.7);//pink
circle(0.75, 0.67, 0.02, 1, 1, 0); //yellow


//left  Flower
Triangle(-0.3, 0.8, -0.34, 0.8, -0.32, 0.7, 0, 1, 0);
circle(-0.32, 0.67, 0.07,  1, 0.5, 0); //orange
circle(-0.32, 0.67, 0.02,  1, 1, 0); //yellow

//LEFT CLOUD
cloud(0, -0.2, 1, 0.3, 0.5, 1);

//SUN
circle(0.67, -0.65, 0.11, 1, 0.5, 0);
Triangle(0.7, -0.82,	0.74, -0.81,	0.73, -0.88,	1, 0.5, 0);
Triangle(0.54, -0.68,	0.52, -0.72,	0.47, -0.7,		1, 0.5, 0);
Triangle(0.8, -0.63,	0.8, -0.68,		0.86, -0.64,	1, 0.5, 0);
Triangle(0.6, -0.8,		0.55, -0.78,	0.535, -0.85,	1, 0.5, 0);
Triangle(0.79, -0.76,	0.82, -0.73,	0.86, -0.78,	1, 0.5, 0);
cloud(0.7, -0.5, 1.3, 0.3, 0.5, 1);