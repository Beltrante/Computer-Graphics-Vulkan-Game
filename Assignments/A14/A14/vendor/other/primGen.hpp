# define M_PI           3.14159265358979323846

typedef struct Vect {
	float x;
	float y;
	float z;
}Vect;

Vect CylinderNorm (int i,float half) {
	float dx = sin(i/half*M_PI);
	float dz = cos(i/half*M_PI);
	Vect norm = {dx, 0.0, dz};
	return norm;
}

Vect SurfaceNorm (float x, float z) {
	float dx_1 = 1;
	float dx_2 = (cos(x) * cos(z));
	float dx_3 = 0.0;

    float dz_1 =  0.0;
	float dz_2 =  (sin(x) * -sin(z));
	float dz_3 = 1;
	
	Vect productVect = {dx_2*dz_3 - dx_3*dz_2,
				   dx_3*dz_1 - dx_1*dz_3,
				   dx_1*dz_2 - dx_2*dz_1};

	float magnitude = sqrt(productVect.x*productVect.x + productVect.y*productVect.y + productVect.z*productVect.z);
	Vect norm = {-productVect.x/magnitude,  -productVect.y/magnitude, -productVect.z/magnitude};
	return norm;
}



void Assignment14::createCubeMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
/*	vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 0.1f}, {-0.5f, 0.0f, 0.866f}});// vertex 3 - Position and Normal

	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	
	// The primitive built here is a cube centered in the origin, whose edge lenght is 2.
*/	
	// Fill array vPos with the positions of the vertices of the mesh
	// vDef.push_back({{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 0 - Position and Normal
	// vDef.push_back({{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 1 - Position and Normal
	// vDef.push_back({{-1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});	// vertex 2 - Position and Normal
	// vDef.push_back({{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});// vertex 3 - Position and Normal
	// Fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	// vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle

	///////////////////////////////////////////////////
	//face back
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	
	vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	
	vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});	
	vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});
	//face front
	vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	
	vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	
	vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});	
	vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
	//face right
	vDef.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
	//face left
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});	
	vDef.push_back({{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});
	//face top
	vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});	
	vDef.push_back({{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});	
	vDef.push_back({{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});	
	vDef.push_back({{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
	//face bottom
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});	
	vDef.push_back({{1.0f, -1.0f, -1.0f}, {0.0f,-1.0f, 0.0f}});	
	vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f,-1.0f, 0.0f}});	
	vDef.push_back({{1.0f, -1.0f, 1.0f}, {0.0f,-1.0f, 0.0f}});

    ///////////////////////////////////////////////
	//fill back
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	//fill front
	vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);	// First triangle
	vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7);	// Second triangle
	// right
	vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);	// First triangle
	vIdx.push_back(9); vIdx.push_back(10); vIdx.push_back(11);	// Second triangle
	// left
	vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);	// First triangle
	vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(15);	// Second triangle
	// top
	vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);	// First triangle
	vIdx.push_back(17); vIdx.push_back(18); vIdx.push_back(19);	// Second triangle
	// bottom
	vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);	// First triangle
	vIdx.push_back(21); vIdx.push_back(22); vIdx.push_back(23);	// Second triangle
}

void Assignment14::createFunctionMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.
	
	// Fill array vPos with the positions of the vertices of the mesh
	// vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	// vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	// vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
    int k = 0;
	float increment = 0.1;
	float max = 6/increment;
	for (float x = -3; x <= 3; x+=increment) {
		for (float z = -3; z <= 3; z+=increment) {
			float y = sin(x) * cos(z);
			Vect norm = SurfaceNorm(x,z);
			vDef.push_back({{x, y, z}, {norm.x, norm.y, norm.z}});
			k++;
		}
	}

	// vIdx.push_back(0);
	// vIdx.push_back(1);
	// vIdx.push_back(61);
	// vIdx.push_back(1);
	// vIdx.push_back(61);
	// vIdx.push_back(62);
	for (float z = 0; z < max; z++) {
		for (float x = 0; x < max; x++) {
			vIdx.push_back(x+z*(max+1));
			vIdx.push_back(x+1+z*(max+1));
			vIdx.push_back(x+max+1+z*(max+1));
			vIdx.push_back(x+1+z*(max+1));
			vIdx.push_back(x+max+1+z*(max+1));
			vIdx.push_back(x+max+1+1+z*(max+1));
		}
	}

	// for (int i = 0; i < k-6/increment; i++) {
	// 	vIdx.push_back(i);
	// 	vIdx.push_back(i+1);
	// 	vIdx.push_back(i+60);
	// 	vIdx.push_back(i+1);
	// 	vIdx.push_back(i+60);
	// 	vIdx.push_back(i+60+1);

	// }
}


void Assignment14::createCylinderMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.
    /////////////////////////////////
	// Draws a Cylinder
	// Top circle
	float x,y,z;
	int number = 36;
	float half = number/2;
	Vect norm;
	for (int i = 0; i < number; i++) {
		vDef.push_back({{sin(i/half*M_PI), 1.0, cos(i/half*M_PI)}, {0.0, 1.0, 0.0}});
	}
	// Top Center
	for (int i = 0; i < number; i++) {
		x = sin(i/half*M_PI);
		y = 1;
		z = cos(i/half*M_PI);
		norm = CylinderNorm(i,half);
		vDef.push_back({{x, y, z}, {norm.x, norm.y, norm.z}});
	}
	// Bottom Center
	for (int i = 0; i < number; i++) {
		x = sin(i/half*M_PI);
		y = -1;
		z = cos(i/half*M_PI);
		norm = CylinderNorm(i,half);
		vDef.push_back({{x, y, z}, {norm.x, norm.y, norm.z}});
	}
	// Bottom Circle
	for (int i = 0; i < number; i++) {
		vDef.push_back({{sin(i/half*M_PI), -1.0, cos(i/half*M_PI)}, {0.0, -1.0, 0.0}});
	}
	vDef.push_back({{0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}});

	// Top Circle
	for (int i = 0; i < number; i++) {
		vIdx.push_back(0);
		vIdx.push_back(i+1);
		vIdx.push_back((i + 1) % number + 1);
	}
	// Center 
	for(int i = 0; i < number; i++) {
		vIdx.push_back( i + number);
		vIdx.push_back( i + number*2);
		//% as the last one must be back to index 36
		vIdx.push_back((i + 1) % number + number);
		
		vIdx.push_back( i + number*2);
		vIdx.push_back((i + 1) % number + number);
		vIdx.push_back((i + 1) % number + number*2);
	}
	// Lower Circle
	for(int i = 0; i < number; i++) {
		vIdx.push_back(number*4);
		vIdx.push_back(i + number*3);
		vIdx.push_back((i + 1) % number + number*3);
	}
}
