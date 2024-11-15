// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)
// Depens on the data needed by the shader
struct UniformBlock
{
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct UniformBlockSun
{
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
	alignas(8) glm::vec2 isEmitting;
};

struct GlobalUniformBlock
{
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec3 lightPos3;
	alignas(16) glm::vec3 lightPos4;
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec3 lightDir2;
	alignas(16) glm::vec3 lightColor;
	alignas(16) glm::vec3 lightColor3;
	alignas(16) glm::vec3 lightColor4;
	alignas(16) glm::vec3 eyePos;
	alignas(16) glm::vec4 coneInOutDecayExp;
};

struct GlobalUniformBlockToon
{
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec3 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct GlobalUniformBlockSun
{
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec3 lightPos2;
	alignas(16) glm::vec3 lightColor;
	alignas(16) glm::vec3 lightColor2;
	alignas(16) glm::vec3 eyePos;
	alignas(8) glm::vec2 pointDecayExp;
	alignas(16) glm::vec4 coneInOutDecayExp;
	alignas(16) glm::vec3 lightDir;
};

// The vertices data structures
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

struct GateStruct
{
	glm::mat4 gateWorld;
	glm::mat4 gemWorld;
	glm::mat4 startGemWorld; // used to animate gem
	float gemRotation = 0;	 // radians
	bool gemCollected = false;
};

struct SunStruct
{
	glm::vec3 pos = glm::vec3(-8, 1, -10);
	bool moveRight = false;
	float sunSpeed = 0.1;
};

struct Segment
{
	glm::vec3 start;
	glm::vec3 end;
};

struct Obstacle
{
	std::vector<Segment> segments;
};

enum GameState
{
	room1,
	room2,
	room3,
	room4,
};

// MAIN !
class DungeonCrawler : public BaseProject
{

protected:
	// Current aspect ratio (used by the callback that resized the window
	float Ar;

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSL, DSLGubo;

	// Vertex formats
	VertexDescriptor VD;

	// Pipelines [Shader couples]
	Pipeline P, PSun, PToon, PBlinn;

	// Models, textures and Descriptors (values assigned to the uniforms)
	// Please note that Model objects depends on the corresponding vertex structure
	// Models
	Model<Vertex> MRoom1, MTunnel1, MRoom2, MTunnel2, MRoom3, MTunnel3, MRoom4;
	Model<Vertex> MGem, MGate;
	Model<Vertex> MSun;
	// Descriptor sets
	DescriptorSet DSGubo, DSGuboSun, DSGuboToon;
	DescriptorSet DSRoom1, DSTunnel1, DSRoom2, DSTunnel2, DSRoom3, DSTunnel3, DSRoom4;
	DescriptorSet DSGem1, DSGate1, DSGem2, DSGate2, DSGem3, DSGate3, DSGem4, DSGate4;
	DescriptorSet DSSun;
	// Textures
	Texture T, TGem, TGem2, TGem3, TGem4, TSun;

	// C++ storage for uniform variables
	UniformBlock uboTunnel1, uboRoom2, uboTunnel2, uboRoom3, uboTunnel3, uboRoom4;
	UniformBlock uboGate1, uboGem2, uboGate2, uboGem3, uboGate3, uboGem4, uboGate4;
	UniformBlockSun uboSun, uboRoom1, uboGem1;
	GlobalUniformBlock gubo;
	GlobalUniformBlockSun guboSun;
	GlobalUniformBlockToon guboToon;

	// Other application parameters
	glm::mat4 ViewPrj;
	glm::vec3 Pos = glm::vec3(0, 0, 0);
	glm::vec3 cameraPos;	  // Player but heigher
	glm::vec3 playerLightPos; // In front of player
	float Yaw = glm::radians(0.0f);
	float Pitch = glm::radians(0.0f);
	float Roll = glm::radians(0.0f);
	float jumpingVel = 0.0; // velocity of the jump (on the y axis)
	bool jumping = false;	// is the character in the air(no jump allowed)
	bool adminMode = false; // is the admin mode on
	glm::vec3 gem1SpotColor = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 gem3SpotColor = glm::vec3(1, 0.5, 0.5);
	glm::vec3 gem4SpotColor = glm::vec3(0.5, 1, 0.5);
	glm::vec4 playerSpotColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	std::chrono::steady_clock::time_point lastClickTime = std::chrono::steady_clock::now();
	;

	GameState gameState = room1;
	glm::mat4 transform2 =
		glm::translate(glm::mat4(1), glm::vec3(0, 0, -44));
	glm::mat4 transform3 =
		transform2 *
		glm::translate(glm::mat4(1), glm::vec3(-24, 0, -20));
	glm::mat4 transform4 =
		transform3 *
		glm::translate(glm::mat4(1), glm::vec3(-22, 0, 2)) *
		glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));

	glm::mat4 worldGem1 =
		glm::translate(glm::mat4(1), glm::vec3(8, 0.5, -4)) *
		glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 worldGate1 =
		glm::rotate(glm::translate(glm::mat4(1), glm::vec3(-10, 0, -20)), glm::radians(90.0f), glm::vec3(0, 1, 0));
	GateStruct gate1 = {worldGate1, worldGem1, worldGem1};

	glm::mat4 worldGem2 =
		glm::translate(transform2, glm::vec3(0, 0.5, -18.5)) *
		glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 worldGate2 =
		glm::rotate(glm::translate(transform2, glm::vec3(-10, 0, -12)), glm::radians(90.0f), glm::vec3(0, 1, 0));
	GateStruct gate2 = {worldGate2, worldGem2, worldGem2};

	glm::mat4 worldGem3 =
		glm::translate(transform3, glm::vec3(6, 0.5, -15)) *
		glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 worldGate3 =
		glm::rotate(glm::translate(transform3, glm::vec3(-4, 0, -18)), glm::radians(90.0f), glm::vec3(0, 1, 0));
	GateStruct gate3 = {worldGate3, worldGem3, worldGem3};

	glm::mat4 worldGem4 =
		glm::translate(transform4, glm::vec3(-12, 0.5, -36)) *
		glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 worldGate4 =
		glm::translate(transform4, glm::vec3(-12, 0, -46));
	GateStruct gate4 = {worldGate4, worldGem4, worldGem4};

	// set obstacles
	Obstacle obstacleRoom1 = {
		{
			{glm::vec3(-10, 0, -2), glm::vec3(-1, 0, -2)},	  // left of door
			{glm::vec3(1, 0, -2), glm::vec3(10, 0, -2)},	  // right of door
			{glm::vec3(-10, 0, -2), glm::vec3(-10, 0, -19)},  // left wall first part
			{glm::vec3(-10, 0, -21), glm::vec3(-10, 0, -22)}, // left wall second part
			{glm::vec3(10, 0, -2), glm::vec3(10, 0, -22)},	  // right wall
			{glm::vec3(-10, 0, -22), glm::vec3(10, 0, -22)},  // front wall
		}};

	Obstacle obstacleTunnel1 = {
		{
			{glm::vec3(-15, 0, -21), glm::vec3(-10, 0, -21)}, // A
			{glm::vec3(-17, 0, -19), glm::vec3(-10, 0, -19)}, // B
			{glm::vec3(-15, 0, -39), glm::vec3(1, 0, -39)},	  // C
			{glm::vec3(-17, 0, -41), glm::vec3(-1, 0, -41)},  // D
			{glm::vec3(-15, 0, -21), glm::vec3(-15, 0, -39)}, // E
			{glm::vec3(-17, 0, -19), glm::vec3(-17, 0, -41)}, // F
			{glm::vec3(1, 0, -39), glm::vec3(1, 0, -46)},	  // G
			{glm::vec3(-1, 0, -41), glm::vec3(-1, 0, -46)},	  // H
		}};

	Obstacle obstacleRoom2 = {
		{
			{transform2 * glm::vec4(glm::vec3(-2, 0, -2), 1), transform2 *glm::vec4(glm::vec3(-2, 0, -4), 1)},	   // A
			{transform2 * glm::vec4(glm::vec3(2, 0, -2), 1), transform2 *glm::vec4(glm::vec3(2, 0, -20), 1)},	   // B
			{transform2 * glm::vec4(glm::vec3(-2, 0, -4), 1), transform2 *glm::vec4(glm::vec3(-2, 0, -11), 1)},	   // C
			{transform2 * glm::vec4(glm::vec3(-2, 0, -13), 1), transform2 *glm::vec4(glm::vec3(-2, 0, -20), 1)},   // D
			{transform2 * glm::vec4(glm::vec3(-10, 0, -4), 1), transform2 *glm::vec4(glm::vec3(-10, 0, -11), 1)},  // E
			{transform2 * glm::vec4(glm::vec3(-10, 0, -13), 1), transform2 *glm::vec4(glm::vec3(-10, 0, -20), 1)}, // F
			{transform2 * glm::vec4(glm::vec3(-10, 0, -4), 1), transform2 *glm::vec4(glm::vec3(-2, 0, -4), 1)},	   // G
			{transform2 * glm::vec4(glm::vec3(-10, 0, -20), 1), transform2 *glm::vec4(glm::vec3(2, 0, -20), 1)},   // H
		}};

	Obstacle obstacleTunnel2 = {
		{
			{transform2 * glm::vec4(glm::vec3(-15, 0, -11), 1), transform2 *glm::vec4(glm::vec3(-15, 0, 1), 1)},   // A
			{transform2 * glm::vec4(glm::vec3(-17, 0, -13), 1), transform2 *glm::vec4(glm::vec3(-17, 0, -1), 1)},  // B
			{transform2 * glm::vec4(glm::vec3(-23, 0, -22), 1), transform2 *glm::vec4(glm::vec3(-23, 0, -1), 1)},  // C
			{transform2 * glm::vec4(glm::vec3(-25, 0, -22), 1), transform2 *glm::vec4(glm::vec3(-25, 0, 1), 1)},   // D
			{transform2 * glm::vec4(glm::vec3(-10, 0, -13), 1), transform2 *glm::vec4(glm::vec3(-17, 0, -13), 1)}, // E
			{transform2 * glm::vec4(glm::vec3(-23, 0, -1), 1), transform2 *glm::vec4(glm::vec3(-17, 0, -1), 1)},   // F
			{transform2 * glm::vec4(glm::vec3(-25, 0, 1), 1), transform2 *glm::vec4(glm::vec3(-15, 0, 1), 1)},	   // G
			{transform2 * glm::vec4(glm::vec3(-15, 0, -11), 1), transform2 *glm::vec4(glm::vec3(-10, 0, -11), 1)}, // H

		}};

	Obstacle obstacleRoom3 = {
		{
			{transform3 * glm::vec4(glm::vec3(-4, 0, -2), 1), transform3 *glm::vec4(glm::vec3(-1, 0, -2), 1)},	 // A
			{transform3 * glm::vec4(glm::vec3(1, 0, -2), 1), transform3 *glm::vec4(glm::vec3(8, 0, -2), 1)},	 // B
			{transform3 * glm::vec4(glm::vec3(-4, 0, -22), 1), transform3 *glm::vec4(glm::vec3(8, 0, -22), 1)},	 // C
			{transform3 * glm::vec4(glm::vec3(-4, 0, -2), 1), transform3 *glm::vec4(glm::vec3(-4, 0, -17), 1)},	 // D
			{transform3 * glm::vec4(glm::vec3(-4, 0, -19), 1), transform3 *glm::vec4(glm::vec3(-4, 0, -22), 1)}, // E
			{transform3 * glm::vec4(glm::vec3(8, 0, -2), 1), transform3 *glm::vec4(glm::vec3(8, 0, -22), 1)},	 // F
			{transform3 * glm::vec4(glm::vec3(0, 0, -6), 1), transform3 *glm::vec4(glm::vec3(0, 0, -10), 1)},	 // G
			{transform3 * glm::vec4(glm::vec3(0, 0, -13), 1), transform3 *glm::vec4(glm::vec3(0, 0, -17), 1)},	 // H
			{transform3 * glm::vec4(glm::vec3(4, 0, -6), 1), transform3 *glm::vec4(glm::vec3(4, 0, -10), 1)},	 // I
			{transform3 * glm::vec4(glm::vec3(4, 0, -13), 1), transform3 *glm::vec4(glm::vec3(4, 0, -17), 1)},	 // L
		}};

	Obstacle obstacleTunnel3 = {
		{
			{transform3 * glm::vec4(glm::vec3(-19, 0, -19), 1), transform3 *glm::vec4(glm::vec3(-4, 0, -19), 1)}, // A
			{transform3 * glm::vec4(glm::vec3(-17, 0, -17), 1), transform3 *glm::vec4(glm::vec3(-4, 0, -17), 1)}, // B
			{transform3 * glm::vec4(glm::vec3(-17, 0, -7), 1), transform3 *glm::vec4(glm::vec3(-9, 0, -7), 1)},	  // C
			{transform3 * glm::vec4(glm::vec3(-19, 0, -5), 1), transform3 *glm::vec4(glm::vec3(-11, 0, -5), 1)},  // D
			{transform3 * glm::vec4(glm::vec3(-24, 0, 1), 1), transform3 *glm::vec4(glm::vec3(-11, 0, 1), 1)},	  // E
			{transform3 * glm::vec4(glm::vec3(-24, 0, 3), 1), transform3 *glm::vec4(glm::vec3(-9, 0, 3), 1)},	  // F
			{transform3 * glm::vec4(glm::vec3(-19, 0, -19), 1), transform3 *glm::vec4(glm::vec3(-19, 0, -5), 1)}, // G
			{transform3 * glm::vec4(glm::vec3(-17, 0, -17), 1), transform3 *glm::vec4(glm::vec3(-17, 0, -7), 1)}, // H
			{transform3 * glm::vec4(glm::vec3(-9, 0, -7), 1), transform3 *glm::vec4(glm::vec3(-9, 0, 3), 1)},	  // I
			{transform3 * glm::vec4(glm::vec3(-11, 0, -5), 1), transform3 *glm::vec4(glm::vec3(-11, 0, 1), 1)},	  // L

		}};

	Obstacle obstacleRoom4 = {
		{
			{transform4 * glm::vec4(glm::vec3(-21, 0, -3), 1), transform4 *glm::vec4(glm::vec3(-1, 0, -3), 1)},	   // A
			{transform4 * glm::vec4(glm::vec3(1, 0, -3), 1), transform4 *glm::vec4(glm::vec3(9, 0, -3), 1)},	   // B
			{transform4 * glm::vec4(glm::vec3(-21, 0, -45), 1), transform4 *glm::vec4(glm::vec3(-14, 0, -45), 1)}, // C
			{transform4 * glm::vec4(glm::vec3(-10, 0, -45), 1), transform4 *glm::vec4(glm::vec3(9, 0, -45), 1)},   // D
			{transform4 * glm::vec4(glm::vec3(9, 0, -3), 1), transform4 *glm::vec4(glm::vec3(9, 0, -45), 1)},	   // E
			{transform4 * glm::vec4(glm::vec3(-21, 0, -3), 1), transform4 *glm::vec4(glm::vec3(-21, 0, -45), 1)},  // F
		}};

	std::vector<Obstacle> obstacles = {obstacleRoom1, obstacleTunnel1, obstacleRoom2, obstacleTunnel2, obstacleRoom3, obstacleTunnel3};
	Obstacle obstacleGate1 = {{{glm::vec3(-10, 0, -21), glm::vec3(-10, 0, -19)}}};
	Obstacle obstacleGate2 = {{{transform2 * glm::vec4(glm::vec3(-10, 0, -13), 1), transform2 *glm::vec4(glm::vec3(-10, 0, -11), 1)}}};
	Obstacle obstacleGate3 = {{{transform3 * glm::vec4(glm::vec3(-4, 0, -19), 1), transform3 *glm::vec4(glm::vec3(-4, 0, -17), 1)}}};
	Obstacle obstacleGate4 = {{{transform4 * glm::vec4(glm::vec3(-13, 0, -46), 1), transform4 *glm::vec4(glm::vec3(-11, 0, -46), 1)}}};
	std::vector<Obstacle> gatesObstacles = {obstacleGate1, obstacleGate2, obstacleGate3, obstacleGate4};

	SunStruct sun;
	// Here you set the main application parameters
	void setWindowParameters()
	{
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Dungeon Crawler";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.005f, 0.01f, 1.0f};

		// Descriptor pool sizes (Remember if test i have 1 more for each defined in TextMaker.hpp)
		uniformBlocksInPool = 19;
		texturesInPool = 16;
		setsInPool = 19;

		Ar = (float)windowWidth / (float)windowHeight;
	}

	// What to do when the window changes size
	void onWindowResize(int w, int h)
	{
		Ar = (float)w / (float)h;
	}

	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit()
	{
		// Descriptor Layouts [what will be passed to the shaders]
		DSL.init(this, {// this array contains the bindings:
						// first  element : the binding number
						// second element : the type of element (buffer or texture)
						//                  using the corresponding Vulkan constant
						// third  element : the pipeline stage where it will be used
						//                  using the corresponding Vulkan constant
						{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
						{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});
		DSLGubo.init(this, {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

		// Vertex descriptors
		VD.init(this, {// this array contains the bindings
					   // first  element : the binding number
					   // second element : the stride of this binging
					   // third  element : whether this parameter change per vertex or per instance
					   //                  using the corresponding Vulkan constant
					   {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}},
				{// this array contains the location
				 // first  element : the binding number
				 // second element : the location number
				 // third  element : the offset of this element in the memory record
				 // fourth element : the data type of the element
				 //                  using the corresponding Vulkan constant
				 // fifth  elmenet : the size in byte of the element
				 // sixth  element : a constant defining the element usage
				 //                   POSITION - a vec3 with the position
				 //                   NORMAL   - a vec3 with the normal vector
				 //                   UV       - a vec2 with a UV coordinate
				 //                   COLOR    - a vec4 with a RGBA color
				 //                   TANGENT  - a vec4 with the tangent vector
				 //                   OTHER    - anything else
				 //
				 // ***************** DOUBLE CHECK ********************
				 //    That the Vertex data structure you use in the "offsetoff" and
				 //	in the "sizeof" in the previous array, refers to the correct one,
				 //	if you have more than one vertex format!
				 // ***************************************************
				 {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos), sizeof(glm::vec3), POSITION},
				 {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm), sizeof(glm::vec3), NORMAL},
				 {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV), sizeof(glm::vec2), UV}});

		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P.init(this, &VD, "shaders/MeshVert.spv", "shaders/OrenNayarFrag.spv", {&DSLGubo, &DSL});
		PSun.init(this, &VD, "shaders/MeshSunVert.spv", "shaders/SunFrag.spv", {&DSLGubo, &DSL});
		PToon.init(this, &VD, "shaders/MeshVert.spv", "shaders/ToonFrag.spv", {&DSLGubo, &DSL});
		PBlinn.init(this, &VD, "shaders/MeshVert.spv", "shaders/BlinnFrag.spv", {&DSLGubo, &DSL});

		// Roof o corridors is transparent if not set
		P.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
							  VK_CULL_MODE_NONE, false);
		PBlinn.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
								   VK_CULL_MODE_NONE, false);
		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF
		MRoom1.init(this, &VD, "Models/room1.obj", OBJ);
		MTunnel1.init(this, &VD, "Models/Tunnel1.obj", OBJ);
		MRoom2.init(this, &VD, "Models/room2.obj", OBJ);
		MTunnel2.init(this, &VD, "Models/Tunnel2.obj", OBJ);
		MRoom3.init(this, &VD, "Models/room3.obj", OBJ);
		MTunnel3.init(this, &VD, "Models/Tunnel3.obj", OBJ);
		MRoom4.init(this, &VD, "Models/room4.obj", OBJ);
		MGem.init(this, &VD, "Models/Gem.obj", OBJ);
		MGate.init(this, &VD, "Models/Gate.obj", OBJ);
		MSun.init(this, &VD, "Models/Sun.obj", OBJ);

		// Create the textures
		// The second parameter is the file name
		T.init(this, "textures/Textures_Dungeon.png");
		TGem.init(this, "textures/Gems/Material_1_baseColor.png");
		TGem2.init(this, "textures/Gems/Material_2_baseColor.png");
		TGem3.init(this, "textures/Gems/Material_3_baseColor.png");
		TGem4.init(this, "textures/Gems/Material_4_baseColor.png");
		TSun.init(this, "textures/Sun.jpg");

		std::vector<int> startValue = {-19, -11, +1};
		std::vector<int> endValue = {-13, -1, +7};
		std::vector<int> translationValue = {-5, -11, -13, -23, -25, -35, -37, -43};
		for (int i = 0; i < startValue.size(); i++)
		{
			int a = startValue[i];
			int b = endValue[i];
			for (int j = 0; j < translationValue.size(); j++)
			{
				int z = translationValue[j];
				obstacleRoom4.segments.push_back({transform4 * glm::vec4(glm::vec3(a, 0, z), 1), transform4 * glm::vec4(glm::vec3(b, 0, z), 1)});
			}
		}
		startValue = {-5, -13, -25, -37};
		endValue = {-11, -23, -35, -43};
		translationValue = {-19, -13, -11, -1, +1, +7};
		for (int i = 0; i < startValue.size(); i++)
		{
			int a = startValue[i];
			int b = endValue[i];
			for (int j = 0; j < translationValue.size(); j++)
			{
				int x = translationValue[j];
				obstacleRoom4.segments.push_back({transform4 * glm::vec4(glm::vec3(x, 0, a), 1), transform4 * glm::vec4(glm::vec3(x, 0, b), 1)});
			}
		}
		obstacles.push_back(obstacleRoom4);
	}

	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit()
	{
		// This creates a new pipeline (with the current surface), using its shaders
		/////////////OrenNayar Pipeline////////////////
		P.create();
		// the second parameter, is a pointer to the Uniform Set Layout of this set
		// the last parameter is an array, with one element per binding of the set.
		// first  elmenet : the binding number
		// second element : UNIFORM or TEXTURE (an enum) depending on the type
		// third  element : only for UNIFORMs, the size of the corresponding C++ object. For texture, just put 0
		// fourth element : only for TEXTUREs, the pointer to the corresponding texture object. For uniforms, use nullptr
		DSTunnel1.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSTunnel2.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSTunnel3.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});

		DSGate1.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGate2.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGate3.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGate4.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});

		DSRoom4.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGem4.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &TGem4}});

		DSGubo.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}}); // used by blinn too

		/////////////Sun Pipeline////////////////
		PSun.create();
		DSRoom1.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlockSun), nullptr}, {1, TEXTURE, 0, &T}});
		DSGem1.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlockSun), nullptr}, {1, TEXTURE, 0, &TGem}});
		DSSun.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlockSun), nullptr}, {1, TEXTURE, 0, &TSun}});
		DSGuboSun.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlockSun), nullptr}});
		/////////////Toon Pipeline////////////////
		PToon.create();
		DSRoom2.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGem2.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &TGem2}});
		DSGuboToon.init(this, &DSLGubo, {{0, UNIFORM, sizeof(GlobalUniformBlockToon), nullptr}});
		/////////////Blinn Pipeline////////////////
		PBlinn.create();
		DSRoom3.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &T}});
		DSGem3.init(this, &DSL, {{0, UNIFORM, sizeof(UniformBlock), nullptr}, {1, TEXTURE, 0, &TGem3}});
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup()
	{
		// Cleanup pipelines
		P.cleanup();
		PSun.cleanup();
		PBlinn.cleanup();
		PToon.cleanup();

		// Cleanup datasets
		DSGubo.cleanup();
		DSGuboSun.cleanup();
		DSGuboToon.cleanup();

		DSRoom1.cleanup();
		DSRoom2.cleanup();
		DSRoom3.cleanup();
		DSRoom4.cleanup();

		DSTunnel1.cleanup();
		DSTunnel2.cleanup();
		DSTunnel3.cleanup();

		DSGem1.cleanup();
		DSGem2.cleanup();
		DSGem3.cleanup();
		DSGem4.cleanup();

		DSGate1.cleanup();
		DSGate2.cleanup();
		DSGate3.cleanup();
		DSGate4.cleanup();

		DSSun.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup()
	{
		// Cleanup textures
		T.cleanup();
		TGem.cleanup();
		TGem2.cleanup();
		TGem3.cleanup();
		TGem4.cleanup();
		TSun.cleanup();

		// Cleanup models
		MRoom1.cleanup();
		MTunnel1.cleanup();
		MRoom2.cleanup();
		MTunnel2.cleanup();
		MRoom3.cleanup();
		MTunnel3.cleanup();
		MRoom4.cleanup();
		MGem.cleanup();
		MGate.cleanup();

		MSun.cleanup();

		// Cleanup descriptor set layouts
		DSL.cleanup();
		DSLGubo.cleanup();

		// Destroies the pipelines
		P.destroy();
		PSun.destroy();
		PToon.destroy();
		PBlinn.destroy();
	}

	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage)
	{
		// For a Dataset object, this command binds the corresponing dataset
		// to the command buffer and pipeline passed in its first and second parameters.
		// The third parameter is the number of the set being bound
		// As described in the Vulkan tutorial, a different dataset is required for each image in the swap chain.
		// This is done automatically in file Starter.hpp, however the command here needs also the index
		// of the current image in the swap chain, passed in its last parameter
		DSGubo.bind(commandBuffer, P, 0, currentImage); // Always necessary for corridors and gates!
		P.bind(commandBuffer);
		MGate.bind(commandBuffer); // One gate is always present and they all use this model

		if (gameState == room1) // Tunnel1+Gate1 in P + Room1+gem1+Sun in PSun
		{
			DSGate1.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGate.indices.size()), 1, 0, 0, 0);

			MTunnel1.bind(commandBuffer);
			DSTunnel1.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel1.indices.size()), 1, 0, 0, 0);

			DSGuboSun.bind(commandBuffer, PSun, 0, currentImage);
			PSun.bind(commandBuffer);
			MGem.bind(commandBuffer);
			DSGem1.bind(commandBuffer, PSun, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGem.indices.size()), 1, 0, 0, 0);
			MRoom1.bind(commandBuffer);
			DSRoom1.bind(commandBuffer, PSun, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MRoom1.indices.size()), 1, 0, 0, 0);
			MSun.bind(commandBuffer);
			DSSun.bind(commandBuffer, PSun, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MSun.indices.size()), 1, 0, 0, 0);
		}
		if (gameState == room2) // tunnel1+tunnel2+Gate2 in P + room2+gem 2 in PToon
		{
			DSGate2.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGate.indices.size()), 1, 0, 0, 0);

			MTunnel1.bind(commandBuffer);
			DSTunnel1.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel1.indices.size()), 1, 0, 0, 0);

			MTunnel2.bind(commandBuffer);
			DSTunnel2.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel2.indices.size()), 1, 0, 0, 0);

			DSGuboToon.bind(commandBuffer, PToon, 0, currentImage);
			PToon.bind(commandBuffer);
			MRoom2.bind(commandBuffer);
			DSRoom2.bind(commandBuffer, PToon, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MRoom2.indices.size()), 1, 0, 0, 0);

			MGem.bind(commandBuffer);
			DSGem2.bind(commandBuffer, PToon, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGem.indices.size()), 1, 0, 0, 0);
		}
		if (gameState == room3) // tunnel2+tunnel3+Gate3 in P + room3+gem3 in PBlinn
		{
			DSGate3.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGate.indices.size()), 1, 0, 0, 0);

			MTunnel2.bind(commandBuffer);
			DSTunnel2.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel2.indices.size()), 1, 0, 0, 0);

			MTunnel3.bind(commandBuffer);
			DSTunnel3.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel3.indices.size()), 1, 0, 0, 0);

			DSGubo.bind(commandBuffer, PBlinn, 0, currentImage);
			PBlinn.bind(commandBuffer);
			MRoom3.bind(commandBuffer);
			DSRoom3.bind(commandBuffer, PBlinn, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MRoom3.indices.size()), 1, 0, 0, 0);
			MGem.bind(commandBuffer);
			DSGem3.bind(commandBuffer, PBlinn, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGem.indices.size()), 1, 0, 0, 0);
		}
		if (gameState == room4) // tunnel3 Gate4 in P + room4 gem4 in P again
		{
			DSGate4.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGate.indices.size()), 1, 0, 0, 0);

			MTunnel3.bind(commandBuffer);
			DSTunnel3.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MTunnel3.indices.size()), 1, 0, 0, 0);

			MGem.bind(commandBuffer);
			DSGem4.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MGem.indices.size()), 1, 0, 0, 0);

			MRoom4.bind(commandBuffer);
			DSRoom4.bind(commandBuffer, P, 1, currentImage);
			vkCmdDrawIndexed(commandBuffer,
							 static_cast<uint32_t>(MRoom4.indices.size()), 1, 0, 0, 0);
		}
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage)
	{
		// Standard procedure to quit when the ESC key is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		/////////GAME LOGIC/////////
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.f;
		// Camera target height
		const float camHeight = 1.25; // Player height
		const float lightDist = 0.5;  // Light distance from player
		// Camera Pitch limits
		const float minPitch = glm::radians(-60.0f);
		const float maxPitch = glm::radians(60.0f);
		// Rotation and motion speed
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 8.0f;
		const int g = 9.81; // Gravity
		const int jump = 4; // Jump power
		static glm::vec3 c; // used for damping

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		bool click = false;
		getSixAxis(deltaT, m, r, fire, click);
		// Light on/off click (enter on left click)
		if (click)
		{
			std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
			long long timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastClickTime).count() / 1000;
			if (timeSinceLastClick > 0.5)
			{
				std::cout << "light on/off click" << std::endl;
				playerSpotColor = playerSpotColor.x == 0 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 0);
				lastClickTime = currentTime;
			}
		}

		// Jumping vel logic
		if (fire && !jumping)
		{
			jumpingVel = jump;
			jumping = true;
		}
		if (jumping)
		{
			jumpingVel -= g * deltaT;
		}

		ViewPrj = glm::mat4(1);
		glm::mat4 World = glm::mat4(1);

		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
		glm::vec3 uy = glm::vec3(0, 1, 0);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
		// Save old position in case movement is not allowed
		glm::vec3 oldPos = Pos;
		Pos = Pos + MOVE_SPEED * m.x * ux * deltaT;
		Pos = Pos + MOVE_SPEED * m.z * uz * deltaT;
		float yMove = adminMode ? MOVE_SPEED * m.y : jumpingVel; // If in admin allow flight else jump
		Pos = Pos + yMove * uy * deltaT;
		if (Pos.y < 0.0f)
		{
			Pos.y = 0.0f;
			jumping = false;
			jumpingVel = 0;
		}
		// Rotation
		Yaw = Yaw - ROT_SPEED * deltaT * r.y;
		Pitch = Pitch + ROT_SPEED * deltaT * r.x;
		Pitch = Pitch < minPitch ? minPitch : (Pitch > maxPitch ? maxPitch : Pitch);
		Roll = Roll - ROT_SPEED * deltaT * r.z;
		Roll = Roll < glm::radians(-175.0f) ? glm::radians(-175.0f) : (Roll > glm::radians(175.0f) ? glm::radians(175.0f) : Roll);

		// check on collisions
		if (controlAllCollisions(obstacles, Pos) || controlAllCollisions(gatesObstacles, Pos))
		{
			Pos = oldPos;
		}

		// Final world matrix computaiton
		World = glm::translate(glm::mat4(1), Pos) * glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0, 1, 0));

		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// View
		cameraPos = Pos + glm::vec3(0.0f, camHeight, 0.0f); // i want the player not to be on floor level (target is camera pos if it was in 3rd)
		// Calculated as camera pos if it was third person but in front and non in the back(opposite eff on y and z)
		playerLightPos = World * glm::vec4(0.0f, camHeight - lightDist * sin(Pitch), -(lightDist * cos(Pitch)), 1.0);
		// apply damping (c = pNew Pos = pOld)
		c = c * exp(-10 * deltaT) + cameraPos * (1 - exp(-10 * deltaT));
		glm::mat4 View = glm::rotate(glm::mat4(1.0f), -Roll, glm::vec3(0, 0, 1)) *
						 glm::rotate(glm::mat4(1.0f), Pitch, glm::vec3(1, 0, 0)) *
						 glm::rotate(glm::mat4(1.0f), -Yaw, glm::vec3(0, 1, 0)) *
						 glm::translate(glm::mat4(1), -c); // used cameraPos for no dumping

		ViewPrj = Prj * View;
		///////////////////////////

		HandleVisibleRooms();

		switch (gameState)
		{
		case room1:
			HandleSunMechanic(sun, uboSun,deltaT, currentImage);
			guboSun.lightPos = sun.pos;
			guboSun.lightPos2 = gate1.startGemWorld[3];
			guboSun.lightPos2.y += 1;
			guboSun.lightColor = glm::vec3(0.6f, 0.6f, 0.6f);
			guboSun.lightColor2 = gem1SpotColor;
			guboSun.pointDecayExp = glm::vec2(3.0f, 2.0f);
			guboSun.coneInOutDecayExp = glm::vec4(0.95f, 0.85f, 1.5f, 2.0f);
			guboSun.lightDir = glm::vec3(0, 1, 0);
			guboSun.eyePos = cameraPos;
			DSGuboSun.map(currentImage, &guboSun, sizeof(guboSun), 0);
			DSSun.map(currentImage, &uboSun, sizeof(uboSun), 0);
			DSRoom1.map(currentImage, &uboRoom1, sizeof(uboRoom1), 0);
			DSTunnel1.map(currentImage, &uboTunnel1, sizeof(uboTunnel1), 0);
			if (HandleGatesMechanic(gate1, uboGate1, deltaT, currentImage))
			{
				HideGem(uboGem1);
			}
			else
			{
				AnimateGem(gate1, deltaT);
				SetUboValues(uboGem1, gate1.gemWorld);
			}
			DSGate1.map(currentImage, &uboGate1, sizeof(uboGate1), 0);
			DSGem1.map(currentImage, &uboGem1, sizeof(uboGem1), 0);

		case room2:
			// pitch corrected for the light direction (15 degrees)
			guboToon.lightDir = glm::vec3(cos(glm::radians(135.0f)) * cos(glm::radians(210.0f)), sin(glm::radians(135.0f)), cos(glm::radians(135.0f)) * sin(glm::radians(210.0f)));
			guboToon.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			guboToon.eyePos = cameraPos;
			DSGuboToon.map(currentImage, &guboToon, sizeof(guboToon), 0);
			DSTunnel1.map(currentImage, &uboTunnel1, sizeof(uboTunnel1), 0);
			DSRoom2.map(currentImage, &uboRoom2, sizeof(uboRoom2), 0);
			DSTunnel2.map(currentImage, &uboTunnel2, sizeof(uboTunnel2), 0);
			if (HandleGatesMechanic(gate2, uboGate2, deltaT, currentImage))
			{
				HideGem(uboGem2);
			}
			else
			{
				AnimateGem(gate2, deltaT);
				SetUboValues(uboGem2, gate2.gemWorld);
			}
			DSGate2.map(currentImage, &uboGate2, sizeof(uboGate2), 0);
			DSGem2.map(currentImage, &uboGem2, sizeof(uboGem2), 0);

		case room3:
			DSTunnel2.map(currentImage, &uboTunnel2, sizeof(uboTunnel2), 0);
			DSRoom3.map(currentImage, &uboRoom3, sizeof(uboRoom3), 0);
			DSTunnel3.map(currentImage, &uboTunnel3, sizeof(uboTunnel3), 0);
			if (HandleGatesMechanic(gate3, uboGate3, deltaT, currentImage))
			{
				HideGem(uboGem3);
			}
			else
			{
				AnimateGem(gate3, deltaT);
				SetUboValues(uboGem3, gate3.gemWorld);
			}
			DSGate3.map(currentImage, &uboGate3, sizeof(uboGate3), 0);
			DSGem3.map(currentImage, &uboGem3, sizeof(uboGem3), 0);
		case room4:
			DSTunnel3.map(currentImage, &uboTunnel3, sizeof(uboTunnel3), 0);
			DSRoom4.map(currentImage, &uboRoom4, sizeof(uboRoom4), 0);
			if (HandleGatesMechanic(gate4, uboGate4, deltaT, currentImage))
			{
				HideGem(uboGem4);
			}
			else
			{
				AnimateGem(gate4, deltaT);
				SetUboValues(uboGem4, gate4.gemWorld);
			}
			DSGate4.map(currentImage, &uboGate4, sizeof(uboGate4), 0);
			DSGem4.map(currentImage, &uboGem4, sizeof(uboGem4), 0);
		default:
			// dang used to make the light point downwards (15 degrees)
			float dang = Pitch + glm::radians(15.0f);
			gubo.lightPos = playerLightPos;
			gubo.lightPos3 = gate3.startGemWorld[3] + glm::vec4(0, 1, 0, 0); // Gem3 Spot light position
			gubo.lightPos4 = gate4.startGemWorld[3] + glm::vec4(0, 1, 0, 0); // Gem4 Spot light position
			gubo.lightDir = calculateDirection(Yaw, dang, Roll);			 // Player Spot light direction
			gubo.lightColor = playerSpotColor;								 // Player Spot light
			gubo.lightColor3 = gem3SpotColor;								 // Gem3 Spot light
			gubo.lightColor4 = gem4SpotColor;								 // Gem4 Spot light
			gubo.eyePos = cameraPos;
			gubo.coneInOutDecayExp = glm::vec4(0.95f, 0.85f, 1.5f, 2.0f); // Gems Spot light parameters
			gubo.lightDir2 = glm::vec3(0, 1, 0);						  // Gem3 and Gem4 Spot light direction
			DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);
		}
	}

	void HandleSunMechanic(SunStruct &sun, UniformBlockSun &sunUbo, float deltaT, uint32_t currentImage)
	{
		float sunVel = deltaT;
		if (sun.moveRight)
		{
			sun.pos += glm::vec3(sunVel, 0, 0);
			if (sun.pos.x > 8)
			{
				sun.pos.x = 8;
				sun.moveRight = false;
			}
		}
		else
		{
			sun.pos -= glm::vec3(sunVel, 0, 0);
			if (sun.pos.x < -8)
			{
				sun.pos.x = -8;
				sun.moveRight = true;
			}
		}
		glm::mat4 sunWorld = glm::translate(glm::mat4(1), sun.pos) * glm::scale(glm::mat4(1), glm::vec3(0.3, 0.3, 0.3));
		SetUboValues(sunUbo, sunWorld, true);
	}

	bool HandleGatesMechanic(GateStruct &gate, UniformBlock &gateUbo, float deltaT, uint32_t currentImage)
	{
		bool gemPicked = GemIsPicked(gate);
		AnimateGate(gate, deltaT);
		SetUboValues(gateUbo, gate.gateWorld);
		return gemPicked;
	}

	/// @brief Check if gem has been picked
	/// @param gate pointer to gate struct
	/// @return true if gem has been picked
	bool GemIsPicked(GateStruct &gate)
	{
		if (gate.gemCollected)
			return true;
		if (Pos.x > gate.gemWorld[3][0] - 0.5 && Pos.x < gate.gemWorld[3][0] + 0.5 &&
			Pos.z > gate.gemWorld[3][2] - 0.5 && Pos.z < gate.gemWorld[3][2] + 0.5)
		{
			gate.gemCollected = true;
			gatesObstacles.erase(gatesObstacles.begin()); // remove gate obstacle
			return true;
		}
		return false;
	}

	/// @brief Animate gem by setting gem world
	/// @param gate pointer to gate struct
	void AnimateGem(GateStruct &gate, float deltaT)
	{
		// Set gem rotation and bounce
		if (gate.gemRotation > 6.28) // if full rotation restart
			gate.gemRotation -= 6.28;
		else
			gate.gemRotation += glm::radians(120.0f) * deltaT;
		float gemY = sin(gate.gemRotation);
		// Set gem world matrix starting from fixed startGemWorld matrix
		gate.gemWorld = glm::translate(gate.startGemWorld, glm::vec3(0, gemY, 0)) * 
		glm::rotate(glm::mat4(1), gate.gemRotation, glm::vec3(0, 1, 0));
	}

	/// @brief Animate gate by setting gate world
	/// @param gate pointer to gate struct
	void AnimateGate(GateStruct &gate, float deltaT)
	{
		if (gate.gemCollected && gate.gateWorld[3][1] < 1.7) // if associated gem is collected and gate is not fully opened
		{
			gate.gateWorld = glm::translate(gate.gateWorld, glm::vec3(0, 0.1 * deltaT, 0));
		}
	}

	/// @brief Handle visible rooms, setting ubo values for visible models
	void HandleVisibleRooms()
	{
		bool rebuild = false;
		switch (gameState)
		{
		case room1:
			if (Pos.z < -30) // Exit in room2 territory
			{
				SetUboValues(uboTunnel1, glm::mat4(1));
				SetUboValues(uboRoom2, transform2);
				SetUboValues(uboTunnel2, transform2);
				gameState = room2;
				rebuild = true;
				break;
			}
			// normal room1 visible models update
			SetUboValues(uboRoom1, glm::mat4(1), false);
			SetUboValues(uboTunnel1, glm::mat4(1));
			break;
		case room2:
			if (Pos.x < -20) // Exit in room3 territory
			{
				SetUboValues(uboTunnel2, transform2);
				SetUboValues(uboRoom3, transform3);
				SetUboValues(uboTunnel3, transform3);
				gameState = room3;
				rebuild = true;
				break;
			}
			if (Pos.z > -30) // Exit in room1 territory
			{
				SetUboValues(uboRoom1, glm::mat4(1), false);
				SetUboValues(uboTunnel1, glm::mat4(1));
				gameState = room1;
				rebuild = true;
				break;
			}
			// normal room2 visible models update
			SetUboValues(uboTunnel1, glm::mat4(1));
			SetUboValues(uboRoom2, transform2);
			SetUboValues(uboTunnel2, transform2);
			break;
		case room3:
			if (Pos.z > -76 && Pos.x < -33) // Exit in room4 territory
			{
				SetUboValues(uboTunnel3, transform3);
				SetUboValues(uboRoom4, transform4);
				gameState = room4;
				rebuild = true;
				break;
			}
			if (Pos.z > -53 && Pos.x > -20) // Exit in room2 territory
			{
				SetUboValues(uboTunnel1, glm::mat4(1));
				SetUboValues(uboRoom2, transform2);
				SetUboValues(uboTunnel2, transform2);
				gameState = room2;
				rebuild = true;
				break;
			}
			// normal room3 visible models update
			SetUboValues(uboTunnel2, transform2);
			SetUboValues(uboRoom3, transform3);
			SetUboValues(uboTunnel3, transform3);
			break;
		case room4:
			if (Pos.z < -76) // Exit in room3 territory
			{
				SetUboValues(uboTunnel3, transform3);
				SetUboValues(uboRoom3, transform3);
				SetUboValues(uboTunnel2, transform2);
				gameState = room3;
				rebuild = true;
				break;
			}
			// normal room4 visible models update
			SetUboValues(uboTunnel3, transform3);
			SetUboValues(uboRoom4, transform4);
			break;
		}
		if (rebuild)
		{
			RebuildPipeline();
		}
	}

	bool controlAllCollisions(std::vector<Obstacle> obstacles, glm::vec3 pos)
	{
		for (int i = 0; i < obstacles.size(); i++)
		{
			for (int j = 0; j < obstacles[i].segments.size(); j++)
			{
				// check if the player is in collision with the segment
				if (isCollision(obstacles[i].segments[j], pos))
				{
					return true;
				}
			}
		}
		return false;
	}

	// Coordinates are translated on a normal cartesian plane of xy for the sake of simplicity
	bool isCollision(Segment segment, glm::vec3 point)
	{
		// for all vec3 remove the y value and invert the z value saving in a vec2
		glm::vec2 start2D = glm::vec2(segment.start.x, -segment.start.z);
		glm::vec2 end2D = glm::vec2(segment.end.x, -segment.end.z);
		glm::vec2 point2D = glm::vec2(point.x, -point.z);
		// make start the leftmost and top point and end the rightmost and bottom point
		//  use cushion of 0.5 to avoid collision around the segment and not exactly on it
		float cushion = 0.5;

		if (start2D.x > end2D.x && start2D.x - end2D.x > 0.5) // rotate change segment by a tiny bit so i need the 0.5 part
		{
			glm::vec2 temp = start2D;
			start2D = end2D;
			end2D = temp;
		}
		if (start2D.y < end2D.y && end2D.y - start2D.y > 0.5)
		{
			glm::vec2 temp = start2D;
			start2D = end2D;
			end2D = temp;
		}
		if (point2D.x > start2D.x - cushion && point2D.x < end2D.x + cushion &&
			point2D.y < start2D.y + cushion && point2D.y > end2D.y - cushion)
		{
			return true;
		}
		return false;
	}

	template <typename T>
	/// @brief Set ubo values and mvp based on transform
	/// @param ubo ubo to set values for
	/// @param transform transform to set ubo values and mvp
	/// @param isEmitting if true set isEmitting to 1, else to 0 (if present in the structure)
	void SetUboValues(T &ubo, glm::mat4 transform, bool isEmitting = false)
	{
		ubo.mMat = transform;
		ubo.mvpMat = ViewPrj * transform;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));

		// Check if the structure has the member variable isEmitting
		if constexpr (std::is_same<T, UniformBlockSun>::value)
		{
			static_assert(std::is_same<decltype(ubo.isEmitting), glm::vec2>::value,
						  "UniformBlockSun must have a member variable isEmitting of type glm::vec2");

			isEmitting ? ubo.isEmitting = glm::vec2(1.0, 0.0) : ubo.isEmitting = glm::vec2(0.0, 0.0);
		}
	}

	template <typename T>
	/// @brief Hide object by setting ubo values and mvp to hide object
	/// @param ubo ubo to set values for
	void HideGem(T &ubo)
	{
		// uboGem1 is different
		glm::mat4 hideMat = glm::mat4(1) * glm::translate(glm::mat4(1), glm::vec3(0, 100, 0));
		ubo.mMat = hideMat;
		ubo.mvpMat = ViewPrj * hideMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
	}

	/// @brief Given yaw, pitch and roll calculate direction vector
	/// @param yaw Player yaw
	/// @param pitch Player pitch
	/// @param roll Player roll
	/// @return normalized direction vector
	glm::vec3 calculateDirection(float yaw, float pitch, float roll)
	{
		// Calculate the direction vector
		glm::vec3 direction;
		direction.x = cos(pitch) * sin(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * cos(yaw);

		return glm::normalize(direction); // Normalize the vector to get a unit vector
	}
};

// This is the main: probably you do not need to touch this!
int main()
{
	DungeonCrawler app;

	try
	{
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}