// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Tri {
	float x1, y1;
	float x2, y2;
	float x3, y3;
	float r, g, b;
	
	void print() {
		std::cout << x1 << " " << y1 << "\t";
		std::cout << x2 << " " << y2 << "\t";
		std::cout << x3 << " " << y3 << "\t";
		std::cout << "[" << r << "," << g << "," << b << "]\n";
	};
};

class Assignment01;
void SetupTriangles(Assignment01 *A);

// MAIN ! 
class Assignment01 : public BaseProject {
	std::vector<Tri> triangles;
	public:
	void Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
		Tri t;
		t.x1 = x1;
		t.y1 = y1;
		t.x2 = x2;
		t.y2 = y2;
		t.x3 = x3;
		t.y3 = y3;
		t.r = r;
		t.g = g;
		t.b = b;
		
		triangles.push_back(t);
	}
	
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M1[2];
	Texture T1;
	DescriptorSet DS1[2];
	
	glm::mat3 CamDir = glm::mat3(1.0f);
	glm::vec3 CamPos = glm::vec3(0.0f, 0.5f, 2.5f);

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 01";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 4;
		texturesInPool = 2;
		setsInPool = 2;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				  });

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL1});
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);

		// Models, textures and Descriptors (values assigned to the uniforms)
		SetupTriangles(this);
		M1[0].BP = this;
		{
			int j = M1[0].vertices.size();
			for(auto& t : triangles) {
				t.print();
				Vertex vertex{};
				
				vertex.pos = {t.x1, t.y1, 0.2};				
				vertex.texCoord = {-1, -1};
				vertex.norm = {t.r, t.g, t.b};				
				M1[0].vertices.push_back(vertex);
				M1[0].indices.push_back(M1[0].vertices.size()-1);
				
				vertex.pos = {t.x2, t.y2, 0.2};				
				vertex.texCoord = {-1, -1};
				vertex.norm = {t.r, t.g, t.b};
				M1[0].vertices.push_back(vertex);
				M1[0].indices.push_back(M1[0].vertices.size()-1);
				
				vertex.pos = {t.x3, t.y3, 0.2};				
				vertex.texCoord = {-1, -1};
				vertex.norm = {t.r, t.g, t.b};
				M1[0].vertices.push_back(vertex);
				M1[0].indices.push_back(M1[0].vertices.size()-1);
			}
		}
		M1[0].createVertexBuffer();
		M1[0].createIndexBuffer();
		
		M1[1].init(this, "models/Plane.obj");
		T1.init(this, "textures/Colors.png");		
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		for(int i = 0; i < 2; i++) {
			DS1[i].init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		}
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		for(int i = 0; i < 2; i++) {
			DS1[i].cleanup();
		}
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		for(int i = 0; i < 2; i++) {
			M1[i].cleanup();
		}
		DSL1.cleanup();
		
		
		P1.destroy();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		P1.bind(commandBuffer);
		for(int i = 0; i < 2; i++) {
			M1[i].bind(commandBuffer);
			DS1[i].bind(commandBuffer, P1, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M1[i].indices.size()), 1, 0, 0, 0);
		}
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();
		static float lastTime = 0.0f;
		
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>
					(currentTime - startTime).count();
		float deltaT = time - lastTime;
		lastTime = time;
					
		const float ROT_SPEED = glm::radians(60.0f);
		const float MOVE_SPEED = 0.75f;
		
		static int Mode = 0;
		static float debounce = time;
		
		if(glfwGetKey(window, GLFW_KEY_SPACE)) {
			if(time - debounce > 0.33) {
				Mode = (Mode + 1) % 4;
				debounce = time;
			}
		}			

		glm::mat4 CamMat = glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos);
					
		glm::mat4 Prj = glm::perspective(glm::radians(45.0f),
						swapChainExtent.width / (float) swapChainExtent.height,
						0.1f, 50.0f);
		Prj[1][1] *= -1;

		// Updates unifoms for the objects
		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)), sin(glm::radians(135.0f)), 0.0f);
		gubo.lightColor = (Mode == 0 ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) :
						  (Mode == 1 ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : 
						  (Mode == 2 ? glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) : glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)) ));
		gubo.eyePos = CamPos;

		for(int j = 0; j < 2; j++) {
			UniformBufferObject ubo{};
			
			ubo.mMat = glm::mat4(1.0f);

			ubo.mvpMat = Prj * CamMat * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			ubo.nMat = glm::inverse(glm::transpose(glm::mat3(ubo.mMat)));

			DS1[j].map(currentImage, &ubo, sizeof(ubo), 0);
			DS1[j].map(currentImage, &gubo, sizeof(gubo), 2);
		}		
	}
};

#include "triangles.hpp"


// This is the main: probably you do not need to touch this!
int main() {
    Assignment01 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}