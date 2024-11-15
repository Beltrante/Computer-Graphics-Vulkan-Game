

void SetupMatrices(Assignment03 *A) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	
	
	// To solve the puzzle: scale 2x proportionally, centered in (2,0,-3)
	S = glm::mat4(1);
	glm::mat4 Sc = scale(S, glm::vec3(2, 2, 2));
	glm::mat4 T = translate(S, glm::vec3(2, 0, -3));
	glm::mat4 TI = inverse(translate(S, glm::vec3(2, 0, -3)));
	S = T * Sc * TI;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// To solve the puzzle: scale 2x on an axis oriented 45 degrees between the x and z axis and passing through point (1,0,0)
	S = glm::mat4(1);
	Sc = scale(S, glm::vec3(3, 1, 1));

	glm::mat4 DT1 = translate(S, glm::vec3(1, 0, 0));
	glm::mat4 IT1 = inverse(translate(S, glm::vec3(1, 0, 0)));

    glm::mat4 DR1 = rotate(S, glm::radians(45.0f), glm::vec3(0, 1, 0));
	glm::mat4 IR1 = inverse(rotate(S, glm::radians(45.0f), glm::vec3(0, 1, 0)));
	
	S=DT1 * DR1 * Sc * IR1 * IT1;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// To solve the puzzle: find it yourself! Hint: compose a rotation around an arbitrary point with a translation
	S = glm::mat4(1);
	DT1 = translate(S, glm::vec3(-2, 0, -3));
	DR1 = rotate(S, glm::radians(45.0f), glm::vec3(0, 1, 0));
	glm::mat4 DT2 = translate(S, glm::vec3(1, 0, 2));

    S=DT2 * DR1 * DT1;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// To solve the puzzle: rotate 60 degree around the y axis, centered in (-1,0,-2)
	S = glm::mat4(1);
	DT1 = translate(S, glm::vec3(-1, 0, -2));
	IT1 = inverse(translate(S, glm::vec3(-1, 0, -2)));
	DR1 = rotate(S, glm::radians(60.0f), glm::vec3(0, 1, 0));
	S=DT1 * DR1 * IT1;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

	// To solve the puzzle: rotate -90 degree around an arbitrary axis passing in (-1,0,0). The x axis can be aligned to this arbitrary direction with a rotation of -45 around the y axis.	
	S = glm::mat4(1);
	DT1 = translate(S, glm::vec3(-1, 0, 0));
	IT1 = inverse(translate(S, glm::vec3(-1, 0, 0)));
	DR1 = rotate(S, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	IR1 = inverse(rotate(S, glm::radians(-45.0f), glm::vec3(0, 1, 0)));
	glm::mat4 DR2 = rotate(S, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	S=DT1 * DR1 *  DR2 * IR1 * IT1;
	A->Matrix(5, S); // sets the matrix corresponding to piece 5

	// To solve the puzzle: find it yourself! Hint: compose a non-proportional scaling that halves the object along one of the main axes, centered in an arbitrary point, with a translation
	S = glm::mat4(1);
	DT1 = translate(S, glm::vec3(3, 0, 0));
	IT1 = inverse(translate(S, glm::vec3(3, 0, 0)));
    DT2 = translate(S, glm::vec3(-2, 0, 0));
	Sc = scale(S, glm::vec3(0.5, 1, 1));
	S=DT1 * Sc * DT2 * IT1;
	A->Matrix(6, S); // sets the matrix corresponding to piece 6

}
