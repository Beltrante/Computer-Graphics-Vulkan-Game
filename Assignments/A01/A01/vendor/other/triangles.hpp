

void SetupTriangles(Assignment01 *A)
{
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
 
    //change tangramSide to increase or decrease cat dimension
	const double floor = 1-0.15;
	const double start = -1+0.2;
	const double tangramSide = 0.8;
	const double square =  tangramSide/4;
	const double diag = sqrt(2) * tangramSide;
	const double footSize = diag/2-square*2;
	const double neck = floor-square*4-footSize; 

	A->Triangle(start, floor-square, start+square*2, floor-square, start+square, floor, 0, 0, 1);
	A->Triangle(start+square, floor, start+square*2, floor-square, start+square*3, floor, 0, 1, 0);
	A->Triangle(start+square*3, floor, start+square*3+diag/2, floor, start+square*3, floor-diag/2, 0, 1, 1);
	A->Triangle(start+square*3, floor-diag/2, start+square*5, floor-footSize, start+square*5, floor-square*4-footSize, 1, 0, 0);
	
	A->Triangle(start+square*5,neck, start+square*5, neck+diag/2, start+square*5+diag/4, neck+diag/4, 1, 0, 1);
   
    A->Triangle(start+square*5, neck,start+square*6, neck+square, start+square*6, neck-square, 1, 1, 0);
	A->Triangle(start+square*6, neck+square,start+square*6, neck-square, start+square*7, neck, 0.2, 0.7,0.7);
	A->Triangle(start+square*5, neck,start+square*5, neck-square*2, start+square*6, neck-square*1, 0.5, 0.8, 0.2);
	A->Triangle(start+square*7, neck,start+square*7, neck-square*2, start+square*6, neck-square*1, 0.6, 0.4, 0.8);

	

}
