void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/
//Parameters
float n = -50.0; // near plane
float f = 50.0; // far plane
float w = 10.0; // half width
float l = -w; // left x
float r = w; // right x
float b = -w/Ar; // bottom y
float t = w/Ar; // top y

	glm::mat4 S;
	glm::mat4 rot45;
	glm::mat4 rot35;
	glm::mat4 rotX;
	glm::mat4 rotY;
	
	
	// Isometric projection
	S = glm::mat4(1);
	glm::mat4 ort = glm::scale(S,glm::vec3(1,-1,1)) * glm::ortho(l, r, b, t, n, f);//parallel projection
	rot45 = rotate(S, glm::radians(-45.0f), glm::vec3(0, 1, 0));//rotate 45 around y axis
	rot35 = rotate(S, glm::radians(35.26f), glm::vec3(1, 0, 0));//rotate 35.26 around x axis
	S = ort * rot35 * rot45;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
	S = glm::mat4(1);
	ort = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);//parallel projection
	rot45 = rotate(S, glm::radians(-45.0f), glm::vec3(0, 1, 0));//rotate 45 around y axis
	rotX = rotate(S, glm::radians(23.0f), glm::vec3(1, 0, 0));//rotate RAND around x axis
	S = ort * rotX * rot45;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
	S = glm::mat4(1);
	ort = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);//parallel projection
	rotY = rotate(S, glm::radians(-30.0f), glm::vec3(0, 1, 0));//rotate RAND around y axis
	rotX = rotate(S, glm::radians(60.0f), glm::vec3(1, 0, 0));//rotate RAND around x axis
	S = ort * rotX * rotY;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
	S = glm::mat4(1);
    float p = 0.5;
	float alfa = 45.0f;
	ort = glm::scale(S, glm::vec3(1, -1, 1)) * glm::ortho(l, r, b, t, n, f);
	glm::mat4 shear = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, -p * cos(glm::radians(alfa)), -p * sin(glm::radians(alfa)), 1, 0, 0, 0, 0, 1);
	S = ort * shear;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
