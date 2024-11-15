#define M_PI 3.141595f

float onU (float i) {
	return i/1024;
}

float onV (float i) {
	return i/512;
}

typedef struct Vect {
	float x;
	float y;
	float z;
}Vect;

void Assignment15::createBoxMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// // The primitive built here is a box centered in the origin, with proportions respecting the texture.

	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// vDef.push_back({{0,0,0}, {0,0,1}, {0,0}});	// vertex 0 - Position, Normal and uv
	// vDef.push_back({{1,0,0}, {0,0,1}, {1,0}});	// vertex 1 - Position and Normal
	// vDef.push_back({{0,1,0}, {0,0,1}, {0,1}});	// vertex 2 - Position and Normal
	// vDef.push_back({{1,1,0}, {0,0,1}, {1,1}});// vertex 3 - Position and Normal

	// // The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	// vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle


	// // Fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	// vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle

	///////////////////////////////////////////////////
	//face back
	vDef.push_back({{-1.0f, -1.0f, -0.25f}, {0.0f, 0.0f, -1.0f}, {onU(888),onV(418)}});	
	vDef.push_back({{1.0f, -1.0f, -0.25f}, {0.0f, 0.0f, -1.0f},{onU(540),onV(418)}});	
	vDef.push_back({{-1.0f, 1.0f, -0.25f}, {0.0f, 0.0f, -1.0f},{onU(888),onV(93)}});	
	vDef.push_back({{1.0f, 1.0f, -0.25f}, {0.0f, 0.0f, -1.0f},{onU(540),onV(93)}});
	//face front
	vDef.push_back({{-1.0f, -1.0f, 0.25f}, {0.0f, 0.0f, 1.0f},{onU(93.0),onV(418.0)}});	
	vDef.push_back({{1.0f, -1.0f, 0.25f}, {0.0f, 0.0f, 1.0f},{onU(444.0),onV(418.0)}});	
	vDef.push_back({{-1.0f, 1.0f, 0.25f}, {0.0f, 0.0f, 1.0f},{onU(93.0),onV(93.0)}});	
	vDef.push_back({{1.0f, 1.0f, 0.25f}, {0.0f, 0.0f, 1.0f},{onU(444.0),onV(93.0)}});
	//face right
	vDef.push_back({{1.0f, -1.0f, -0.25f}, {1.0f, 0.0f, 0.0f},{onU(538),onV(418)}});	
	vDef.push_back({{1.0f, 1.0f, -0.25f}, {1.0f, 0.0f, 0.0f},{onU(538),onV(93)}});	
	vDef.push_back({{1.0f, -1.0f, 0.25f}, {1.0f, 0.0f, 0.0f},{onU(445),onV(418)}});	
	vDef.push_back({{1.0f, 1.0f, 0.25f}, {1.0f, 0.0f, 0.0f},{onU(445),onV(93)}});
	//face left
	vDef.push_back({{-1.0f, -1.0f, -0.25f}, {-1.0f, 0.0f, 0.0f},{onU(0),onV(418)}});	
	vDef.push_back({{-1.0f, 1.0f, -0.25f}, {-1.0f, 0.0f, 0.0f},{onU(0),onV(93)}});	
	vDef.push_back({{-1.0f, -1.0f, 0.25f}, {-1.0f, 0.0f, 0.0f},{onU(93),onV(418)}});	
	vDef.push_back({{-1.0f, 1.0f,0.25f}, {-1.0f, 0.0f, 0.0f},{onU(93),onV(93)}});
	//face top
	vDef.push_back({{-1.0f, 1.0f, -0.25f}, {0.0f, 1.0f, 0.0f},{onU(93),onV(0)}});	
	vDef.push_back({{1.0f, 1.0f, -0.25f}, {0.0f, 1.0f, 0.0f},{onU(444),onV(0)}});	
	vDef.push_back({{-1.0f, 1.0f, 0.25f}, {0.0f, 1.0f, 0.0f},{onU(93),onV(93)}});	
	vDef.push_back({{1.0f, 1.0f, 0.25f}, {0.0f, 1.0f, 0.0f},{onU(444),onV(93)}});
	//face bottom
	vDef.push_back({{-1.0f, -1.0f, -0.25f}, {0.0f, -1.0f, 0.0f},{onU(93),onV(512)}});	
	vDef.push_back({{1.0f, -1.0f, -0.25f}, {0.0f,-1.0f, 0.0f},{onU(444),onV(512)}});	
	vDef.push_back({{-1.0f, -1.0f, 0.25f}, {0.0f,-1.0f, 0.0f},{onU(93),onV(418)}});	
	vDef.push_back({{1.0f, -1.0f, 0.25f}, {0.0f,-1.0f, 0.0f},{onU(444),onV(418)}});

    ///////////////////////////////////////////////
	//fill back
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	//fill front
	vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);	// First triangle
	vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7);	// Second triangle
	//right
	vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);	// First triangle
	vIdx.push_back(9); vIdx.push_back(10); vIdx.push_back(11);	// Second triangle
	//left
	vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);	// First triangle
	vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(15);	// Second triangle
	//top
	vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);	// First triangle
	vIdx.push_back(17); vIdx.push_back(18); vIdx.push_back(19);	// Second triangle
	//bottom
	vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);	// First triangle
	vIdx.push_back(21); vIdx.push_back(22); vIdx.push_back(23);	// Second triangle
}



void Assignment15::createSphereMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a sphere of radius 1, centered in the origin, on which the Mars texture is applied seamless.
	
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// vDef.push_back({{0,0,0}, {0,0,1}, {0,0}});	// vertex 0 - Position, Normal and uv
	// vDef.push_back({{1,0,0}, {0,0,1}, {1,0}});	// vertex 1 - Position and Normal
	// vDef.push_back({{0,1,0}, {0,0,1}, {0,1}});	// vertex 2 - Position and Normal
	// vDef.push_back({{1,1,0}, {0,0,1}, {1,1}});// vertex 3 - Position and Normal

	// // The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	// vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Define the radius and number of segments and rings for the sphere
	const float radius = 1.0f;
	const int segments = 32; //number of triangles for each ring
	const int rings = 30; //number of horizontal partitions
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
    // place points using spherical coordinates
	for (float i = 0; i <= rings; ++i) {
		float phi = M_PI * i / rings;
		for (float j = 0; j <= segments; ++j) {
			float theta = 2.0f * M_PI * j / segments;
			
			float x = radius * cos(theta) * sin(phi);
			float y = - radius * cos(phi);
			float z = radius * sin(theta) * sin(phi);
			
			position = glm::vec3(x, y, z);
			normal = glm::normalize(position);
			uv = glm::vec2(j / segments, 1.0f - i / rings);

            vDef.push_back({{position.x, position.y, position.z}, {normal.x, normal.y, normal.z}, {uv.x, uv.y}});
		}
	}	

	for (int i = 0; i < rings; ++i) {
		for (int j = 0; j < segments; ++j) {
			int first = i * (segments + 1) + j;
			int second = first + segments + 1;
			vIdx.push_back(first);
			vIdx.push_back(second);
			vIdx.push_back(first + 1);

			vIdx.push_back(second);
			vIdx.push_back(second + 1);
			vIdx.push_back(first + 1);
		}
	}
}

