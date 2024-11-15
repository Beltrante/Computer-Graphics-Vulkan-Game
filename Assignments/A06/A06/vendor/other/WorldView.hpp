

glm::mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Rho, glm::vec3 Pos) {
	// Creates a view projection matrix, with near plane at 0.1, and far plane at 50.0, and
	// aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
	// vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
	// defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
	// elevation (Pitch), and <Rho> the roll.
	glm::mat4 M = glm::mat4(1);
	glm::mat4 Prj = glm::mat4(1);
	glm::mat4 View = glm::mat4(1);

	//Set up projection matrix  
	float n = 0.1; // near plane
	float f = 50.0; // far plane
	// Fov-y = 90 deg 
	Prj = glm::perspective(glm::radians(45.0f), Ar, n, f);
	Prj[1][1] *= -1; // To be inverted because of y-up convention in vulkan different from opengl
	//Set up look in View matrix
	View =
	glm::rotate(glm::mat4(1.0), -Rho, glm::vec3(0,0,1)) * 
	glm::rotate(glm::mat4(1.0), -Beta, glm::vec3(1,0,0)) *
	glm::rotate(glm::mat4(1.0), -Alpha, glm::vec3(0,1,0)) *
	glm::translate(glm::mat4(1.0), -Pos);
	//Combine into view-projection matrix
    M = Prj * View;
	return M;
}

glm::mat4 MakeWorldMatrix(glm::vec3 pos, glm::quat rQ, glm::vec3 size) {
	// creates and returns a World Matrix that positions the object at <pos>,
	// orients it according to <rQ>, and scales it according to the sizes
	// given in vector <size>
	glm::mat4 M = glm::mat4(1);
	// Convert quaternion to rotation matrix
	glm::mat4 RM = glm::mat4(rQ);
	// Apply the transformation
	M = glm::translate(M, pos) * RM * glm::scale(M, size);
	return M;
}
