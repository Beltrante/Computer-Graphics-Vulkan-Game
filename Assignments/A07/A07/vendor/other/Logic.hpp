
// i don't use this function anymore i left it since it was used in the simple implementation i commented at the end of the assignment
glm::mat4 MakeWorldMatrixEuler(glm::vec3 pos, float yaw, float pitch, float roll)
{
	glm::mat4 out;
	out =
		glm::translate(glm::mat4(1.0f), pos) *
		glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
	// glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1, 0, 0)) * //character does not rotate up and down
	// glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0, 0, 1)); //character dows not roll;
	return out;
}

void GameLogic(Assignment07 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World)
{
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);

	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);
	const float MOVE_SPEED = 2.0f;

	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
	// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;

	// To be done in the assignment

	// Variables
	static float Yaw = 0.0f;
	static float YawCamera = 0.0f;
	static float Pitch = glm::radians(30.0f);
	static float Roll = 0.0f;
	static glm::vec3 c;//used for damping
	glm::vec3 cameraPos;

	/// Walk model update procedure
	//up vectors update
	glm::vec3 ux = glm::rotate(glm::mat4(1.0f), YawCamera, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::rotate(glm::mat4(1.0f), YawCamera, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
    //angles update
    YawCamera = YawCamera - ROT_SPEED * deltaT * r.y;
	Pitch = Pitch + ROT_SPEED * deltaT * r.x;
	Pitch = Pitch < minPitch ? minPitch : (Pitch > maxPitch ? maxPitch : Pitch);
	Roll = Roll - ROT_SPEED * deltaT * r.z;
	Roll = Roll < glm::radians(-175.0f) ? glm::radians(-175.0f) : (Roll > glm::radians(175.0f) ? glm::radians(175.0f) : Roll);
	if (m.x != 0 || m.z != 0)
	{

		Yaw = atan2(-m.x, m.z);
		Yaw += YawCamera;
	}
    //update position
	Pos += MOVE_SPEED * m.x * ux * deltaT;
	Pos += MOVE_SPEED * m.y * uy * deltaT;
	Pos.y = Pos.y < 0.0f ? 0.0f : Pos.y;
	Pos += MOVE_SPEED * m.z * uz * deltaT;

	// Final world matrix computaiton
	World = glm::translate(glm::mat4(1), Pos) * glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0));

	
	// Projection
	glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	Prj[1][1] *= -1;
	// View
	glm::vec3 target = Pos + glm::vec3(0.0f, camHeight, 0.0f);
	cameraPos = glm::translate(glm::mat4(1), Pos) * glm::vec4(camDist * sin(YawCamera), camHeight + camDist * sin(Pitch), camDist * cos(YawCamera), 1.0);

	// apply damping (c = pNew Pos = pOld)
	c = c * exp(-10 * deltaT) + cameraPos * (1 - exp(-10 * deltaT));

	glm::mat4 View = glm::rotate(glm::mat4(1.0f), -Roll, glm::vec3(0, 0, 1)) *
					 glm::lookAt(c, target, glm::vec3(0, 1, 0));
	ViewPrj = Prj * View;
}

//////This commented is the old implementation with easy walk model and camera damping, the one above is the complete version
// void GameLogic(Assignment07 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World) {
// 	// The procedure must implement the game logic  to move the character in third person.
// 	// Input:
// 	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
// 	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
// 	// Output:
// 	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
// 	// <glm::mat4 &World> the world matrix of the object
// 	// Parameters
// 	// Camera FOV-y, Near Plane and Far Plane
// 	const float FOVy = glm::radians(45.0f);
// 	const float nearPlane = 0.1f;
// 	const float farPlane = 100.f;

// 	// Player starting point
// 	const glm::vec3 StartingPosition = glm::vec3(-41.50f, 0.0f, -19.5f);

// 	// Camera target height and distance
// 	const float camHeight = 0.25;
// 	const float camDist = 1.5;
// 	// Camera Pitch limits
// 	const float minPitch = glm::radians(-8.75f);
// 	const float maxPitch = glm::radians(60.0f);
// 	// Rotation and motion speed
// 	const float ROT_SPEED = glm::radians(120.0f);
// 	const float MOVE_SPEED = 2.0f;

// 	// Integration with the timers and the controllers
// 	// returns:
// 	// <float deltaT> the time passed since the last frame
// 	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
// 	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
// 	// <bool fire> if the user has pressed a fire button (not required in this assginment)
// 	float deltaT;
// 	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
// 	bool fire = false;
// 	A->getSixAxis(deltaT, m, r, fire);

// 	// Game Logic implementation
// 	// Current Player Position - statc variables make sure that their value remain unchanged in subsequent calls to the procedure
// 	static glm::vec3 Pos = StartingPosition;

// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	static float yaw = 0.0f;
// 	static float pitch = glm::radians(30.0f);
// 	static float roll = 0.0f;

// 	//walk model update procedure object
// 	glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1),yaw,glm::vec3(0,1,0))*glm::vec4(1,0,0,1));
// 	glm::vec3 uy = glm::vec3(0,1,0);
// 	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1),yaw,glm::vec3(0,1,0))*glm::vec4(0,0,-1,-1));
// 	pitch -= ROT_SPEED * r.x * deltaT;
// 	yaw += ROT_SPEED * r.y * deltaT;
// 	roll += ROT_SPEED * r.z * deltaT;
// 	pitch = glm::clamp(pitch, minPitch, maxPitch); //limit the pitch angle
// 	Pos -= ux * MOVE_SPEED * m.x * deltaT;
// 	Pos += uy * MOVE_SPEED * m.y * deltaT;
// 	Pos.y = glm::max(Pos.y, 0.0f); //no going trough ground allowed
// 	Pos += uz * MOVE_SPEED * m.z * deltaT;
// 	World = MakeWorldMatrixEuler(Pos,yaw,pitch,roll);

// 	//calculate cameraPosition c and targetPosition a
// 	static glm::vec3 a,c;
//     glm::vec3 pNew,pOld;
//     pOld = c;
// 	pNew = World * glm::vec4(0,camHeight+camDist*sin(pitch),camDist*cos(pitch),1);
// 	a = Pos + glm::vec3(0,camHeight,0);
// 	//apply damping (c = pNew Pos = pOld)
// 	c = pOld * exp(-10*deltaT) + pNew *(1-exp(-10*deltaT));

// 	// Calculate view matrix using LookAt technique
//     glm::mat4 View = glm::lookAt(c, a, glm::vec3(0.0f, -1.0f, 0.0f));
// 	//Calculate projection matrix
//     glm::mat4 Projection = glm::perspective(FOVy, Ar, nearPlane, farPlane);

// 	// To be done in the assignment
// 	ViewPrj = Projection * View;
// 	World = World;
// }