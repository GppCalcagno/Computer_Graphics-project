#include "MyProject.hpp"
#include <cstdlib>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

struct GlobalUniformBufferObject {
	//slide 21
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	
	alignas(16) glm::vec3 cameraPos;
	alignas(16) glm::vec4 selector;
	alignas(4) float lightPow;
	alignas(4) float BoatLight;
	//alignas(4) float par;
};

struct GlobalUniformBufferText {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct GlobalUniformBufferBackground {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(4) float lightPow; //to increase/decrease lighting of the background
};


struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(4) float sigma; // sigma is needed for the roughness of the materials (Oren-Navay)
};

struct UniformBufferObjSecondary {
	alignas(16) glm::mat4 model;
};

float randomFloat(float min, float max) {
	return  min + ((float)rand() / (float)(RAND_MAX / (max - min)));
}

void seaWave(glm::mat4& WorldMat, glm::vec3 shift,float go) {
	static glm::vec3 x = glm::vec3(0);
	static int dir = -1; //1 -> UP -1-> DOWN

	//add the wave motion (up down)
	if (x.y > 0.07 || x.y < 0.0001)
		dir = -dir;

	//add the transation motion 
	x -= shift*go;

	// reset the position of the sea
	if (x.z < -12 || x.x < -12 || x.x>12)
		x = glm::vec3(0, x.y, 0);


	float speed = 0.00065; //depend a lot from the CPU Clock  -> on mine: without batter 0.0005, with battery 0.00013
	x.y = x.y + dir * speed;
	//udate the position of the boat (above movements + initial offset) + scale 
	WorldMat = glm::translate(glm::mat4(1),x+glm::vec3(0, 0.03, 5)) * glm::scale(glm::mat4(1), glm::vec3(3.0, 1.0, 3.0));
}

//collision check
bool canStep(glm::vec3 rock_centre, float rock_ray, glm::vec3 boat_centre, float boat_ray) {
	return glm::distance(rock_centre, boat_centre) > (rock_ray + boat_ray);
}


// MAIN ! 
class MyProject : public BaseProject {
	protected:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL_Global;
	DescriptorSetLayout DSL_Obj;

	DescriptorSetLayout DSL_GlobalText;
	DescriptorSetLayout DSL_ObjText;
	DescriptorSetLayout DSL_GlobalWallPaper;
	DescriptorSetLayout DSL_ObjWallPaper;

	// Pipelines [Shader couples]
	Pipeline P1;
	Pipeline P_Text;
	Pipeline P_WallPaper;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M_Boat;
	Texture T_Boat;
	DescriptorSet DS_Boat; //instange of DSL_Obj

	Model M_Rock1;
	Texture T_Rock1;
	DescriptorSet DS_Rock1; //instange of DSL_Obj
	DescriptorSet DS_Rock2; //instange of DSL_Obj
	DescriptorSet DS_Rock5; //instange of DSL_Obj
	DescriptorSet DS_Rock6; //instange of DSL_Obj

	Model M_Rock2;
	Texture T_Rock2;
	DescriptorSet DS_Rock3; //instange of DSL_Obj
	DescriptorSet DS_Rock4; //instange of DSL_Obj
	DescriptorSet DS_Rock7; //instange of DSL_Obj
	DescriptorSet DS_Rock8; //instange of DSL_Obj


	Model M_Water;
	Texture T_Water;
	DescriptorSet DS_Water; //instange of DSL_Obj

	Model M_Sky;
	Texture T_Sky;
	DescriptorSet DS_Sky; //instange of DSL_Obj_Secondary
	DescriptorSet DS_SkyLeft; //instange of DSL_Obj_Secondary
	DescriptorSet DS_SkyRight; //instange of DSL_Obj_Secondary
	DescriptorSet DS_SkyBack; //instange of DSL_Obj_Secondary

	Model M_SkyUp;
	Texture T_SkyUp;
	DescriptorSet DS_SkyUp; //instange of DSL_Obj_Secondary

	Model M_InitialScreen;
	Texture T_InitialScreen;
	DescriptorSet DS_InitialScreen; //instange of DSL_Obj_Secondary

	Model M_FinalScreen;
	Texture T_FinalScreen;
	DescriptorSet DS_FinalScreen; //instange of DSL_Obj_Secondary

	DescriptorSet DS_Global; //instange of DSL_Global
	DescriptorSet DS_GlobalText; //instange of DSL_GlobalText
	DescriptorSet DS_GlobalWallPaper; //instange of DSL_GlobalBack

	float rateo;
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 1700;
		windowHeight = 1000;
		rateo = (float)windowWidth / (float)windowHeight;
		windowTitle = "Boat Simulator";
		initialBackgroundColor = {0.0f, 0.0f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		//Global + Boat + 4*Rock1+ 4*Rock1 + Water + Global2 + Initial Screen + Final Screen + Global3 + 5*Sky
		uniformBlocksInPool = 20;
		//Boat + 4*Rock1+ 4*Rock2 + Water + Initial + Final + 5sky
		texturesInPool = 17;
		//Global + Boat + 4*Rock1++ 4*Rock1 + Water + Global2 + Initial Screen + Final Screen + Global3+ 5*Sky 
		setsInPool = 20;
	}
	
	// Here you load and setup all your Vulkan objects
	void localInit() {

		// Descriptor Layouts [what will be passed to the shaders] setting layout
		DSL_Obj.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
				{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSL_Global.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		DSL_ObjText.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
				{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSL_GlobalText.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}
			});

		DSL_ObjWallPaper.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
				{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSL_GlobalWallPaper.init(this, {
				{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});


		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		
		
		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSL_Global,&DSL_Obj});
		P_Text.init(this, "shaders/vertText.spv", "shaders/fragText.spv", { &DSL_GlobalText,&DSL_ObjText });
		P_WallPaper.init(this, "shaders/vertWall.spv", "shaders/fragWall.spv", { &DSL_GlobalWallPaper,&DSL_ObjWallPaper });


		// Models, textures and Descriptors (values assigned to the uniforms)
		M_Boat.init(this, "models/boat.obj");
		T_Boat.init(this, "textures/boat_diffuse.bmp");
		DS_Boat.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Boat}
				});
		//ROCK 1
		M_Rock1.init(this, "models/rock1.obj");
		T_Rock1.init(this, "textures/rock1_low_Base_Color.png");

		DS_Rock1.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Rock1}
			});
		DS_Rock2.init(this, &DSL_Obj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Rock1}
			});

		DS_Rock5.init(this, &DSL_Obj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Rock1}
			});
		DS_Rock6.init(this, &DSL_Obj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Rock1}
			});


		//ROCK 2
		M_Rock2.init(this, "models/Rock_2.obj");
		T_Rock2.init(this, "textures/Rock_2_Base_Color.jpg");

		DS_Rock3.init(this, &DSL_Obj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Rock2}
			});
		DS_Rock4.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Rock2}
			});

		DS_Rock7.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Rock2}
			});
		DS_Rock8.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Rock2}
			});
		//WATER
		M_Water.init(this, "models/LargePlane.obj");
		T_Water.init(this, "textures/WaterTexture.png");

		DS_Water.init(this, &DSL_Obj, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Water}
			});
		//SPASH SCREEN
		M_InitialScreen.init(this, "models/Square.obj");
		T_InitialScreen.init(this, "textures/InitialScreen1.png");

		DS_InitialScreen.init(this, &DSL_ObjText, {
					{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
					{1, TEXTURE, 0, &T_InitialScreen}
			});

		M_FinalScreen.init(this, "models/Square.obj");
		T_FinalScreen.init(this, "textures/FinalScreen1.png");
		DS_FinalScreen.init(this, &DSL_ObjText, {
					{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
					{1, TEXTURE, 0, &T_FinalScreen}
			});

		//SKY
		M_Sky.init(this, "models/Square.obj");
		T_Sky.init(this, "textures/WallPaper.png");
		DS_Sky.init(this, &DSL_ObjWallPaper, {
					{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
					{1, TEXTURE, 0, &T_Sky}
			});

		DS_SkyLeft.init(this, &DSL_ObjWallPaper, {
				{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
				{1, TEXTURE, 0, &T_Sky}
			});

		DS_SkyRight.init(this, &DSL_ObjWallPaper, {
				{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
				{1, TEXTURE, 0, &T_Sky}
			});

		DS_SkyBack.init(this, &DSL_ObjWallPaper, {
				{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
				{1, TEXTURE, 0, &T_Sky}
			});

		M_SkyUp.init(this, "models/Square.obj");
		T_SkyUp.init(this, "textures/WallPaperUp.png");

		DS_SkyUp.init(this, &DSL_ObjWallPaper, {
				{0, UNIFORM, sizeof(UniformBufferObjSecondary), nullptr},
				{1, TEXTURE, 0, &T_SkyUp}
			});

		//GLOBAL DS
		DS_Global.init(this, &DSL_Global, {
				{0, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});

		DS_GlobalText.init(this, &DSL_GlobalText, {
				{0, UNIFORM, sizeof(GlobalUniformBufferText), nullptr}
			});

		DS_GlobalWallPaper.init(this, &DSL_GlobalWallPaper, {
				{0, UNIFORM, sizeof(GlobalUniformBufferBackground), nullptr}
			});
	}

	// Here you destroy all the objects you created!		
	void localCleanup() {
		DS_Boat.cleanup();
		DS_Rock1.cleanup();
		DS_Rock2.cleanup();
		DS_Rock3.cleanup();
		DS_Rock4.cleanup();
		DS_Rock5.cleanup();
		DS_Rock6.cleanup();
		DS_Rock7.cleanup();
		DS_Rock8.cleanup();
		DS_Sky.cleanup();
		DS_SkyUp.cleanup();
		DS_SkyLeft.cleanup();
		DS_SkyRight.cleanup();
		DS_SkyBack.cleanup();
		DS_Water.cleanup();
		DS_InitialScreen.cleanup();
		DS_FinalScreen.cleanup();


		DS_Global.cleanup();
		DS_GlobalText.cleanup();
		DS_GlobalWallPaper.cleanup();

		T_Rock1.cleanup();
		T_Rock2.cleanup();
		T_Boat.cleanup();
		T_Water.cleanup();
		T_InitialScreen.cleanup();
		T_FinalScreen.cleanup();
		T_Sky.cleanup();
		T_SkyUp.cleanup();

		M_Boat.cleanup();
		M_Rock1.cleanup();
		M_Rock2.cleanup();
		M_Water.cleanup();
		M_InitialScreen.cleanup();
		M_FinalScreen.cleanup();
		M_Sky.cleanup();
		M_SkyUp.cleanup();

		P1.cleanup();
		P_Text.cleanup();
		P_WallPaper.cleanup();

		DSL_Global.cleanup();
		DSL_Obj.cleanup();

		DSL_GlobalText.cleanup();
		DSL_ObjText.cleanup();	

		DSL_GlobalWallPaper.cleanup();
		DSL_ObjWallPaper.cleanup();
	
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		
		//set P1
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,P1.graphicsPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 0, 1, &DS_Global.descriptorSets[currentImage], 0, nullptr);		

		//BOAT
		// property .vertexBuffer of models, contains the VkBuffer handle to its vertex buffer
		// property .indexBuffer of models, contains the VkBuffer handle to its index buffer
		// property .pipelineLayout of a pipeline contains its layout.
		// property .descriptorSets of a descriptor set contains its elements. Numbers are (set, Block).
		// property .indices.size() of models, contains the number of triangles * 3 of the mesh.
		VkBuffer vertexBuffers[] = {M_Boat.vertexBuffer};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, M_Boat.indexBuffer, 0,VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,P1.pipelineLayout, 1, 1, &DS_Boat.descriptorSets[currentImage],0, nullptr);
		vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(M_Boat.indices.size()), 1, 0, 0, 0);
		
		//ROCK1
		VkBuffer vertexBuffers1[] = { M_Rock1.vertexBuffer };
		VkDeviceSize offsets1[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers1, offsets1);
		vkCmdBindIndexBuffer(commandBuffer, M_Rock1.indexBuffer, 0,VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,P1.pipelineLayout, 1, 1, &DS_Rock1.descriptorSets[currentImage],0, nullptr);
		vkCmdDrawIndexed(commandBuffer,	static_cast<uint32_t>(M_Rock1.indices.size()), 1, 0, 0, 0);

		//ROCK2
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Rock2.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock1.indices.size()), 1, 0, 0, 0);

		//ROCK5
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Rock5.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock1.indices.size()), 1, 0, 0, 0);

		//ROCK6
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Rock6.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock1.indices.size()), 1, 0, 0, 0);

		//ROCK3
		VkBuffer vertexBuffers7[] = { M_Rock2.vertexBuffer };
		VkDeviceSize offsets7[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers7, offsets7);
		vkCmdBindIndexBuffer(commandBuffer, M_Rock2.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1,&DS_Rock3.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock2.indices.size()), 1, 0, 0, 0);

		//ROCK4
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1,&DS_Rock4.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock2.indices.size()), 1, 0, 0, 0);

		//ROCK7
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Rock7.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock2.indices.size()), 1, 0, 0, 0);

		//ROCK8
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Rock8.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rock2.indices.size()), 1, 0, 0, 0);

		//Water
		VkBuffer vertexBuffers2[] = { M_Water.vertexBuffer };
		VkDeviceSize offsets2[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
		vkCmdBindIndexBuffer(commandBuffer, M_Water.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P1.pipelineLayout, 1, 1, &DS_Water.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Water.indices.size()), 1, 0, 0, 0);

		//OTHER PIPELINE: WALLPAPER PIPELINE
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.graphicsPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 0, 1, &DS_GlobalWallPaper.descriptorSets[currentImage], 0, nullptr);

		//sky
		VkBuffer vertexBuffers5[] = { M_Sky.vertexBuffer };
		VkDeviceSize offsets5[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers5, offsets5);
		vkCmdBindIndexBuffer(commandBuffer, M_Sky.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 1, 1, &DS_Sky.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Sky.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 1, 1, &DS_SkyLeft.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Sky.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 1, 1, &DS_SkyRight.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Sky.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 1, 1, &DS_SkyBack.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Sky.indices.size()), 1, 0, 0, 0);

		//skyUp
		VkBuffer vertexBuffers6[] = { M_SkyUp.vertexBuffer };
		VkDeviceSize offsets6[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers6, offsets6);
		vkCmdBindIndexBuffer(commandBuffer, M_SkyUp.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_WallPaper.pipelineLayout, 1, 1, &DS_SkyUp.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_SkyUp.indices.size()), 1, 0, 0, 0);



		//OTHER PIPELINE: TEXT PIPELINE
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_Text.graphicsPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_Text.pipelineLayout, 0, 1, &DS_GlobalText.descriptorSets[currentImage], 0, nullptr);

		//Initial Screen
		VkBuffer vertexBuffers3[] = { M_InitialScreen.vertexBuffer };
		VkDeviceSize offsets3[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
		vkCmdBindIndexBuffer(commandBuffer, M_InitialScreen.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_Text.pipelineLayout, 1, 1, &DS_InitialScreen.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_InitialScreen.indices.size()), 1, 0, 0, 0);

		//finalScreen
		VkBuffer vertexBuffers4[] = { M_FinalScreen.vertexBuffer };
		VkDeviceSize offsets4[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets4);
		vkCmdBindIndexBuffer(commandBuffer, M_FinalScreen.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, P_Text.pipelineLayout, 1, 1, &DS_FinalScreen.descriptorSets[currentImage], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_FinalScreen.indices.size()), 1, 0, 0, 0);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		static float lastTime = 0.0f;
		float deltaT = time - lastTime;
		lastTime = time;
		static float debounce = time;

		static float SPEED = 5.0;
		const float ROT_SPEED = glm::radians(60.0f);
		const float ROT_SPEED_ROLL = glm::radians(20.0f);

		static glm::vec3 pos = glm::vec3(0, 0, 0);

		static float rotation = 0.0f;

		static float look_pitch = 1.0f;
		static float look_roll_boat = 0.0f;
		static float look_roll = 0.0f;
		static float look_yaw = 0.0f;


		static std::array <glm::vec3, 8> rock_pos_array = { glm::vec3(1.0f, 0.0f, 12.0f), glm::vec3(3.0f, 0.0f, 16.0f), glm::vec3(-1.0f, 0.0f, 4.0f), glm::vec3(-4.0f, 0.0f, 21.0f),
			 glm::vec3(6.0f, 0.0f, 8.0f), glm::vec3(9.0f, 0.0f, 13.0f), glm::vec3(-5.0f, 0.0f, 14.0f), glm::vec3(-10.0f, 0.0f, 18.0f) };
		std::array <DescriptorSet, 8> rock_ds_array = { DS_Rock1, DS_Rock2, DS_Rock5, DS_Rock6, DS_Rock3, DS_Rock4, DS_Rock7, DS_Rock8 };
		std::array <float, 8> rock_rot_array = { 0.0f, 1.3f, 0.6f, 1.4f,  0.7f, 1.2f, 1.6f, 0.1f }; // rotations
		std::array <float, 8> rock_scale_array = { 1.0f, 1.0f, 0.6f, 1.2f, 2.0f, 3.0f, 1.5f, 2.5f }; //scaling factors
		std::array <float, 8> rock_ray_array = { 0.32f, 0.32f, 0.192f, 0.384f, 0.3f, 0.45f, 0.225f, 0.375f }; // rays

		static int CameraState = 0;
		static int view_state = 0; // 0->look_at, 1->look_in, 2->top_view
		static int go = 1; // to stop/start movement with W
		static int game_state = 0; // 0: not started; 1: started; 2: collision; 3: ended

		/*LIGHT*/
		static int LightMode = 0; //0=Morning-direct 1=morning-point 2=Evening 3=MidNight
		static float boatLight = 1; //set in function of boat Movements
		static glm::vec3 camPos = glm::vec3(0); //set in function of the Camera Movements

		static float BackgroundLight = 0.8f;
		static float AmbientLight = 0.2f;
		static glm::vec4 selector=glm::vec4(0,0,1,0); //x = boat Light, y=PointLight, z=Direct light
		//static float par = 0;




		if (glfwGetKey(window, GLFW_KEY_ENTER) && (game_state == 0 || game_state == 3)) {
			CameraState = 1;
			game_state = 1;

			//reset variables
			startTime = std::chrono::high_resolution_clock::now();
			currentTime = std::chrono::high_resolution_clock::now();
			lastTime = 0.0f;
			debounce = 0;
			SPEED = 2.0f;
			go = 1;

			BackgroundLight = 0.8;
			AmbientLight = 0.3;
			selector.x = 0; //boatlight OFF;
			selector.y = 0; //PointLight ON;
			selector.z = 1;
			std::cout << "lightMode:" << LightMode << " - Day - direct light\n";

			view_state = 0;
			look_pitch = 1.0f;
			rotation = 0.0f;
			look_roll_boat = 0.0f;
			look_roll = 0.0f;
			look_yaw = 0.0f;

			rock_pos_array = { glm::vec3(1.0f, 0.0f, 12.0f), glm::vec3(3.0f, 0.0f, 16.0f), glm::vec3(-1.0f, 0.0f, 4.0f), glm::vec3(-4.0f, 0.0f, 21.0f),
				 glm::vec3(6.0f, 0.0f, 8.0f), glm::vec3(9.0f, 0.0f, 13.0f), glm::vec3(-5.0f, 0.0f, 14.0f), glm::vec3(-10.0f, 0.0f, 18.0f) };
			rock_rot_array = { 0.0f, 1.3f, 0.6f, 1.4f,  0.7f, 1.2f, 1.6f, 0.1f };
			rock_scale_array = { 1.0f, 1.0f, 0.6f, 1.2f, 2.0f, 3.0f, 1.5f, 2.5f };
			rock_ray_array = { 0.32f, 0.32f, 0.192f, 0.384f, 0.3f, 0.45f, 0.225f, 0.375f };
		}

		/*
		if (game_state == 3) { //after collision
			view_state = 0;
			look_pitch = 1.0f;
			rotation = 0.0f;
			look_roll_boat = 0.0f;
			look_roll = 0.0f;
			look_yaw = 0.0f;
			rock_pos_array = { glm::vec3(1.0f, 0.0f, 12.0f), glm::vec3(3.0f, 0.0f, 16.0f), glm::vec3(-1.0f, 0.0f, 4.0f), glm::vec3(-4.0f, 0.0f, 21.0f),
				 glm::vec3(6.0f, 0.0f, 8.0f), glm::vec3(9.0f, 0.0f, 13.0f), glm::vec3(-5.0f, 0.0f, 14.0f), glm::vec3(-10.0f, 0.0f, 18.0f) };
			rock_rot_array = { 0.0f, 1.3f, 0.6f, 1.4f,  0.7f, 1.2f, 1.6f, 0.1f };
			rock_scale_array = { 1.0f, 1.0f, 0.6f, 1.2f, 2.0f, 3.0f, 1.5f, 2.5f };
			rock_ray_array = { 0.32f, 0.32f, 0.192f, 0.384f, 0.3f, 0.45f, 0.225f, 0.375f };
		}*/
		if (game_state == 1) {
			if (glfwGetKey(window, GLFW_KEY_SPACE)) {
				if (time - debounce > 0.2) {
					debounce = time;
					view_state++;
					if (view_state == 3) {
						view_state = 0;
					}

					if (view_state == 0) {
						look_pitch = 1.0f;
					}
					else if (view_state == 1) {
						look_pitch = 0.6f;
					}
				}
			}

			// start/stop movement of the boat with W
			if (glfwGetKey(window, GLFW_KEY_W)) {
				if (time - debounce > 0.2) {
					debounce = time;
					go = 1 - go;
				}
			}
			// if the boat is moving increase speed and update position of rocks
			if (go) {
				SPEED = SPEED + 0.001;
				for (int i = 0; i < std::size(rock_pos_array); i++) {
					rock_pos_array[i] -= SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), look_yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::vec4(0, 0, 1, 1)) * deltaT;
					// check collision with each rock
					if (!canStep(rock_pos_array[i], rock_ray_array[i], glm::vec3(0.4 * sin(look_yaw), 0, 0.4 * cos(look_yaw)), 0.1f) ||
						!canStep(rock_pos_array[i], rock_ray_array[i], glm::vec3(0.2 * sin(look_yaw), 0, 0.2 * cos(look_yaw)), 0.12f)) {
						game_state = 2;
						view_state = 0;
						look_pitch = 1.0f;
						rotation = 0.0f;
					}
				}
			}
			// update rotation angles
			if (glfwGetKey(window, GLFW_KEY_E)) {
				if (look_roll < 1.2) {
					look_roll += deltaT * ROT_SPEED;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_Q)) {
				if (look_roll > -1.2) {
					look_roll -= deltaT * ROT_SPEED;
				}
			}
			//modify camera position up
			if (glfwGetKey(window, GLFW_KEY_UP)) {
				if (look_pitch >= -1.2f) {
					look_pitch -= deltaT * ROT_SPEED;
				}
			}
			//modify camera position down
			if (glfwGetKey(window, GLFW_KEY_DOWN)) {
				if (look_pitch <= 1.2f) {
					look_pitch += deltaT * ROT_SPEED;
				}
			}
			//modify camera pisition right
			if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
				if (rotation < 1.5f) {
					rotation += deltaT * ROT_SPEED;
				}
			}
			//modify camera position left
			if (glfwGetKey(window, GLFW_KEY_LEFT)) {
				if (rotation > -1.5f) {
					rotation -= deltaT * ROT_SPEED;
				}
			}
			//turn the boat right
			if (glfwGetKey(window, GLFW_KEY_D)) {
				if (look_yaw > -1.2) {
					look_yaw -= deltaT * ROT_SPEED;
					look_roll_boat += deltaT * ROT_SPEED_ROLL;
				}
			}
			//turn the boat right
			if (glfwGetKey(window, GLFW_KEY_A)) {
				if (look_yaw < 1.2) {
					look_yaw += deltaT * ROT_SPEED;
					look_roll_boat -= deltaT * ROT_SPEED_ROLL;
				}
			}
		}
		// after collision
		if (game_state == 2) { 
			selector.x = 0;			
			if(BackgroundLight == 0.0){
				BackgroundLight = 0.1;
				AmbientLight = 0.1;
			}
			go = 0;
			// animation for the boat 
			if (look_roll_boat >= 0) {
				if (look_roll_boat < 2) {
					look_roll_boat += deltaT * ROT_SPEED_ROLL;
				}
				else {
					game_state = 3;
					CameraState = 2;
				}
			}
			else {
				if (look_roll_boat > -2) {
					look_roll_boat -= deltaT * ROT_SPEED_ROLL;
				}
				else {
					game_state = 3;
					CameraState = 2;
				}
			}
		}

		// modify backgroud Light 
		if (glfwGetKey(window, GLFW_KEY_O)) {
			if (BackgroundLight > 0.0)
				BackgroundLight -= 0.02;

		}
		// modify backgroud Light 
		if (glfwGetKey(window, GLFW_KEY_L)) {
			if (BackgroundLight < 1)
				BackgroundLight += 0.02;
		}

		//modify ambient light (useless)
		if (glfwGetKey(window, GLFW_KEY_9)) {
			if (AmbientLight > 0.0)
				AmbientLight -= 0.02;

		}
		//modify ambient light (useless)
		if (glfwGetKey(window, GLFW_KEY_0)) {
			if (AmbientLight < 1)
				AmbientLight += 0.02;
		}

		//BoatLight (faro) Selector
		if (glfwGetKey(window, GLFW_KEY_I)) {
			if (time - debounce > 0.2) {
				debounce = time;
				selector.x = 1 - selector.x;
			}
		}
		//Day-Night Selection
		if (glfwGetKey(window, GLFW_KEY_K)) {
			if (time - debounce > 0.2) {
				debounce = time;

				LightMode++;
				LightMode = LightMode > 3 ? 0 : LightMode;


				switch (LightMode) {
				case 0: //morning - direct light;
					BackgroundLight = 0.8;
					AmbientLight = 0.3;
					selector.x = 0; //boatlight OFF;
					selector.y = 0; //PointLight OFF;
					selector.z = 1; //DirectLight ON;
					std::cout << "lightMode:" << LightMode << " - Day - direct light\n";
					break;
				case 1: //morning
					BackgroundLight = 0.8;
					AmbientLight = 0.3;
					selector.x = 0; //boatlight OFF;
					selector.y = 1; //PointLight ON;
					selector.z = 0; //DirectLight OFF;
					std::cout << "lightMode:" << LightMode << " - Day\n";
					break;
				case 2: //Evening 
					BackgroundLight = 0.2;
					AmbientLight = 0.1;
					selector.x = 1; //boatlight ON;
					selector.y = 0; //PointLight OFF;
					selector.z = 0; //DirectLight OFF;
					std::cout << "lightMode:" << LightMode << " - Evening\n";
					break;
				case 3: //night;
					BackgroundLight = 0.0;
					AmbientLight = 0.0;
					selector.x = 1; //boatlight ON;
					selector.y = 0; //PointLight OFF;
					selector.z = 0; //DirectLight OFF;
					std::cout << "lightMode:" << LightMode << " - Night\n";
					break;
				}
			}
		}

		GlobalUniformBufferObject gubo{};
		UniformBufferObject ubo{};

		GlobalUniformBufferText guboText{};
		GlobalUniformBufferBackground guboBack{};
		UniformBufferObjSecondary uboSecondary{};

		gubo.lightPow = AmbientLight;
		gubo.BoatLight = boatLight;
		gubo.cameraPos = camPos;
		gubo.selector = selector;

		guboBack.lightPow = BackgroundLight;


		switch (CameraState) {
		case 0: //initial Screen
			gubo.view = glm::mat4(0);
			gubo.proj = glm::mat4(0);			

			guboBack.view = glm::mat4(0);
			guboBack.proj = glm::mat4(0);	

			guboText.view = glm::lookAt(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
			guboText.proj = glm::perspective(glm::radians(90.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
			guboText.proj[1][1] *= -1;
			break;
		case 1: //game Phase
			if (view_state == 0) {
				//glm::vec3 lookat_eye = pos + glm::vec3(-sin(look_yaw), 0.5, -cos(look_yaw));
				camPos = glm::vec3(-sin(look_pitch) * sin(look_yaw + rotation),cos(look_pitch), -sin(look_pitch) * cos(look_yaw + rotation));
				gubo.view = glm::rotate(glm::mat4(1.0), -look_roll, glm::vec3(0, 0, 1)) *glm::lookAt(camPos, pos, glm::vec3(0.0f, 1.0f, 0.0f));				
			}
			if (view_state == 1) {
				float dir = 3.14f;
				glm::mat4 rot = glm::mat4(glm::quat(glm::vec3(-look_pitch, dir + look_yaw - rotation, look_roll)));
				glm::mat4 rot_roll = glm::rotate(glm::mat4(1.0), look_roll_boat + look_roll, glm::vec3(0, 0, 1));
				glm::mat4 rot_elev = glm::rotate(glm::mat4(1.0), -look_pitch, glm::vec3(1, 0, 0));
				glm::mat4 rot_dir = glm::rotate(glm::mat4(1.0), dir + look_yaw - rotation, glm::vec3(0, 1, 0));
				glm::mat4 transl = glm::translate(glm::mat4(1.0), glm::vec3(pos.x, pos.y + 0.5, pos.z));
				gubo.view = glm::inverse(transl * rot_dir * rot_elev * rot_roll);
				camPos = glm::vec3(0.0,0.5,0.0);
			}
			if (view_state == 2) {
				camPos = glm::vec3(0.0, 7.0, -0.01);
				gubo.view = glm::lookAt(camPos, pos, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			gubo.proj = glm::perspective(glm::radians(85.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 40.0f);
			gubo.proj[1][1] *= -1;
			
			guboText.view = glm::mat4(0);
			guboText.proj = glm::mat4(0);

			guboBack.view = gubo.view;
			guboBack.proj = gubo.proj;			
			break;
		case 2: //endgame
			gubo.view = glm::mat4(0);
			gubo.proj = glm::mat4(0);
			
			guboText.view = glm::lookAt(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(3, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
			guboText.proj = glm::perspective(glm::radians(90.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
			guboText.proj[1][1] *= -1;

			guboBack.view = gubo.view;
			guboBack.proj = gubo.proj;
			
		}


		// Here is where you actually update your uniforms
		void* data;
		//looad the Global Data
		vkMapMemory(device, DS_Global.uniformBuffersMemory[0][currentImage], 0, sizeof(gubo), 0, &data);
		memcpy(data, &gubo, sizeof(gubo));
		vkUnmapMemory(device, DS_Global.uniformBuffersMemory[0][currentImage]);

		//looad the GlobalText Data
		vkMapMemory(device, DS_GlobalText.uniformBuffersMemory[0][currentImage], 0, sizeof(guboText), 0, &data);
		memcpy(data, &guboText, sizeof(guboText));
		vkUnmapMemory(device, DS_GlobalText.uniformBuffersMemory[0][currentImage]);

		//looad the GlobalBackGroud Data
		vkMapMemory(device, DS_GlobalWallPaper.uniformBuffersMemory[0][currentImage], 0, sizeof(guboBack), 0, &data);
		memcpy(data, &guboBack, sizeof(guboBack));
		vkUnmapMemory(device, DS_GlobalWallPaper.uniformBuffersMemory[0][currentImage]);


		//Boat
		boatLight = look_yaw;
		ubo.model = glm::rotate(glm::mat4(1), look_yaw, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1), look_roll_boat, glm::vec3(0, 0, 1)) *
			glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0, 1, 0)) *
			glm::scale(glm::mat4(1.0), glm::vec3(0.001, 0.001, 0.001));
		ubo.sigma = 0.1;
		vkMapMemory(device, DS_Boat.uniformBuffersMemory[0][currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Boat.uniformBuffersMemory[0][currentImage]);

		//rocks
		for (int i = 0; i < std::size(rock_pos_array); i++) {
			if (pos.z > rock_pos_array[i].z + 5) {
				float min_x = -10.0f;
				float max_x = 10.0f;
				float min_z = 15.0f;
				float max_z = 20.0f;
				float x_rand = randomFloat(min_x, max_x);
				float z_rand = randomFloat(min_z, max_z);
				float rot = randomFloat(0.0f, 3.14f);
				float scale;
				if (i <= 3) {
					scale = randomFloat(2.0f, 4.0f);
				}
				else {
					scale = randomFloat(3.0f, 5.0f);
				}
				rock_pos_array[i] = glm::vec3(x_rand, 0, z_rand);
				rock_rot_array[i] = rot;
				rock_scale_array[i] = scale;
				if (i <= 3) {
					rock_ray_array[i] = 0.32 * scale;
				}
				else {
					rock_ray_array[i] = 0.15 * scale;
				}
			}
			ubo.model = glm::translate(glm::mat4(1), rock_pos_array[i]) * glm::rotate(glm::mat4(1.0), rock_rot_array[i], glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1), rock_scale_array[i] * glm::vec3(0.1f, 0.1f, 0.1f));
			ubo.sigma = 0.5;
			vkMapMemory(device, rock_ds_array[i].uniformBuffersMemory[0][currentImage], 0, sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, rock_ds_array[i].uniformBuffersMemory[0][currentImage]);
		}

		//Sea
		seaWave(ubo.model, glm::vec3(glm::rotate(glm::mat4(1.0f), look_yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT * SPEED,go);
		ubo.sigma = 0.0;
		vkMapMemory(device, DS_Water.uniformBuffersMemory[0][currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Water.uniformBuffersMemory[0][currentImage]);

		// Sky
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(0, 8, 25)) * glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.0, 20.0, 10.0));
		vkMapMemory(device, DS_Sky.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_Sky.uniformBuffersMemory[0][currentImage]);

		// SkyUp
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(0, 18, 5)) * glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1), glm::vec3(1.0, 20.0, 20.0));
		vkMapMemory(device, DS_SkyUp.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_SkyUp.uniformBuffersMemory[0][currentImage]);

		//SkyBack
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(0, 8, -15)) * glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.0, 20.0, 10.0));
		vkMapMemory(device, DS_SkyBack.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_SkyBack.uniformBuffersMemory[0][currentImage]);


		// SkyLeft
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(20, 8, 5)) * glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.0, 20.0, 10.0));
		vkMapMemory(device, DS_SkyLeft.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_SkyLeft.uniformBuffersMemory[0][currentImage]);

		// SkyRight
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(-20, 8, 5)) * glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.0, 20.0, 10.0));
		vkMapMemory(device, DS_SkyRight.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_SkyRight.uniformBuffersMemory[0][currentImage]);

		// Initial Text
		uboSecondary.model = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(0.0, 3.0 * rateo, 3.0));
		vkMapMemory(device, DS_InitialScreen.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_InitialScreen.uniformBuffersMemory[0][currentImage]);

		// Final Text
		uboSecondary.model = glm::translate(glm::mat4(1), glm::vec3(3, 0, 0)) * glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(0.0, 3.0 * rateo, 3.0));
		vkMapMemory(device, DS_FinalScreen.uniformBuffersMemory[0][currentImage], 0, sizeof(uboSecondary), 0, &data);
		memcpy(data, &uboSecondary, sizeof(uboSecondary));
		vkUnmapMemory(device, DS_FinalScreen.uniformBuffersMemory[0][currentImage]);
	}
};


int main() {
    MyProject app;
	
    try {
		app.run();
		system("pause");
    } catch (const std::exception& e) {
		system("pause");
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
