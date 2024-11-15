

void SetupProjectionMatrices(Assignment05 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/
	//Parameters
	float n = 0.1; // near plane
	float f = 50.0; // far plane

	glm::mat4 S;
	
	
	// Fov-y = 60 deg
	S = glm::mat4(1);
	glm::mat4 Port = glm::perspective(glm::radians(60.0f), Ar, n, f);
	Port[1][1] *= -1; // To be inverted because of y-up convention in vulkan different from opengl
	S = Port * S;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	S = glm::mat4(1);
	Port = glm::perspective(glm::radians(105.0f), Ar, n, f);
	Port[1][1] *= -1; // To be inverted because of y-up convention in vulkan different from opengl
	S = Port * S;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	S = glm::mat4(1);
	Port = glm::perspective(glm::radians(25.0f), Ar, n, f);
	Port[1][1] *= -1; // To be inverted because of y-up convention in vulkan different from opengl
	S = Port * S;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-x = 45 deg
	S = glm::mat4(1);
	float FovY = glm::radians(45.0f);
	//As seen in slides
	float t = n * tan(FovY/2); 
	//As seen in slides
	float b = -n * tan(FovY/2);
	// left sife of the frustum (right view is requested)
	float l = 0;
	// right side of the frustum
	float r = Ar * n * tan(FovY/2) * 2; //relation r-l = a*(t-b) must be satisfied
	S = glm::scale(S, glm::vec3(1, -1, 1)) * glm::frustum(l, r, b, t, n, f);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
