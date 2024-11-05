#include <iostream>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "EclipseEngine/Shader.h"
#include "EclipseEngine/VAO.h"
#include "EclipseEngine/VBO.h"
#include "EclipseEngine/EBO.h"
#include "EclipseEngine/Core.h"
#include "EclipseEngine/Logger.h"
#include "EclipseEngine/Component.h"
#include "EclipseEngine/Texture.h"
#include "EclipseEngine/Mesh.h"
#include "EclipseEngine/Material.h"
#include "EclipseEngine/Transform.h"
#include "EclipseEngine/GameObject.h"
#include "EclipseEngine/Grid.h"
#include "PanelHandler.h"
#include "AssetsPanel.h"
#include "FPSpanel.h"
#include "ConsolePanel.h"

using namespace std;

using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1500, 844);

std::vector<Vertex> vertices =
{ //               COORDINATES           /            NORMALS                TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  // 0
	Vertex{glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  // 1
	Vertex{glm::vec3( 1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 2
	Vertex{glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 3
	Vertex{glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},	 // 4
	Vertex{glm::vec3( 1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 5
	Vertex{glm::vec3(-1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 6
	Vertex{glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}	 // 7
};

std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3,
	1, 4, 5,
	1, 5, 2,
	4, 7, 6,
	4, 6, 5,
	7, 0, 3,
	7, 3, 6,
	0, 1, 4,
	0, 4, 7,
	3, 2, 5,
	3, 5, 6
};

int main(int argc, char** argv) {
	Logger::Init();

	Core core(WINDOW_SIZE.x, WINDOW_SIZE.y, "Eclipse Engine");

	if (!core.Initialize()) {
		std::cerr << "Engine initialization failed." << std::endl;
		return -1;
	}

	PanelHandler panelHandler(core.GetWindow());
	ilInit();  // Initialize the DevIL library

	std::vector<Texture> catTexture
	{
		Texture("Assets/cat.jpg","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	std::vector<Texture> houseTexture
	{
		Texture("Assets/Baker_house.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	std::vector<Texture> shipTexture
	{
		Texture("Assets/SF_Fighter-Albedo_dds.dds","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	std::vector<Texture> F1Texture
	{
		Texture("Assets/F1_texture.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
	Shader gridShader("Shaders/grid.vert", "Shaders/grid.frag");

	GameObject cube;
	cube.AddComponent<Mesh>(vertices, indices, catTexture);
	cube.AddComponent<Material>(shaderProgram, catTexture);
	cube.transform.SetPosition(glm::vec3(0.0f, 7.0f, 0.0f));

	GameObject house;
	std::string meshFilePath = "Assets/BakerHouse.FBX";
	house.AddComponent<Mesh>(meshFilePath);
	//auto& meshTexture = house.GetComponent<Mesh>()->GetTextures();    // It is supposed to get the defined texture from the fbx
	house.AddComponent<Material>(shaderProgram, houseTexture);

	GameObject ship;
	std::string shipModel = "Assets/SF_Fighter.FBX";
	ship.AddComponent<Mesh>(shipModel);
	ship.AddComponent<Material>(shaderProgram, shipTexture);
	ship.transform.SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));
	ship.transform.SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

	GameObject F1;
	std::string F1Mesh = "Assets/formula1.fbx";
	F1.AddComponent<Mesh>(F1Mesh);
	F1.AddComponent<Material>(shaderProgram, F1Texture);
	F1.transform.SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));

	Camera camera(WINDOW_SIZE.x, WINDOW_SIZE.y, glm::vec3(7.0f, 4.0f, -7.0f));

	Grid grid(gridShader, camera);

	// Register the scroll callback for zoom control
	glfwSetScrollCallback(core.GetWindow(), Camera::scroll_callback);

	// Variables to track FPS and milliseconds per frame
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	std::unique_ptr<Panel>* panelPtr = &panelHandler.GetPanel("FPS Panel");

	// console panel
	auto consolePanel = dynamic_cast<ConsolePanel*>(panelHandler.GetPanel("Console Panel").get());

	while (!core.ShouldClose()) {
		// fps
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float fps = 1.0f / deltaTime;
		float ms = deltaTime * 1000.0f;

		core.BeginFrame();
		panelHandler.NewFrame();

		camera.Inputs(core.GetWindow());
		camera.UpdateMatrix(0.1f, 100.0f);

		cube.Draw(shaderProgram, camera);
		house.Draw(shaderProgram, camera);
		F1.Draw(shaderProgram, camera);
		ship.Draw(shaderProgram, camera);

		grid.Draw();

		// fps
		if (panelPtr) {
			if (auto* fpsPanel = dynamic_cast<FPSPanel*>(panelPtr->get())) {
				fpsPanel->Update(fps, ms);
			}
		}
		// Rendering ImGui
		panelHandler.Render();

		panelHandler.EndFrame();
		core.EndFrame();
	}

	shaderProgram.Delete();
	gridShader.Delete();
	Logger::Close();

	return EXIT_SUCCESS;
}