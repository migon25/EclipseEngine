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
#include "PanelHandler.h"
#include "AssetsPanel.h"
#include "FPSpanel.h"
#include "ConsolePanel.h"

using namespace std;

using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1100, 619);

std::vector<Vertex> vertices =
{ //               COORDINATES           /            NORMALS                TEXTURE COORDINATES    //
	Vertex{glm::vec3(-0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  // 0
	Vertex{glm::vec3( 0.0f, -0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  // 1
	Vertex{glm::vec3( 0.0f,  0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 2
	Vertex{glm::vec3(-0.0f,  0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 3
	Vertex{glm::vec3( 0.0f, -0.0f,-0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},	 // 4
	Vertex{glm::vec3( 0.0f,  0.0f,-0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 5
	Vertex{glm::vec3(-0.0f,  0.0f,-0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 6
	Vertex{glm::vec3(-0.0f, -0.0f,-0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}	 // 7
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

void DrawGrid(Shader& shader, Camera& camera) {
	// Step 1: Define grid vertices
	const float gridSize = 10.0f; // Size of the grid
	const int gridLines = 20;      // Number of lines in each direction
	const float lineSpacing = gridSize / gridLines;

	std::vector<Vertex> gridVertices;

	for (int i = -gridLines / 2; i <= gridLines / 2; ++i) {
		float offset = i * lineSpacing;

		// Horizontal line
		gridVertices.push_back({ glm::vec3(-gridSize / 2, 0.0f, offset), glm::vec3(0.0f), glm::vec2(0.0f) });
		gridVertices.push_back({ glm::vec3(gridSize / 2, 0.0f, offset), glm::vec3(0.0f), glm::vec2(1.0f) });

		// Vertical line
		gridVertices.push_back({ glm::vec3(offset, 0.0f, -gridSize / 2), glm::vec3(0.0f), glm::vec2(0.0f) });
		gridVertices.push_back({ glm::vec3(offset, 0.0f, gridSize / 2), glm::vec3(0.0f), glm::vec2(1.0f) });
	}

	// Step 2: Create VAO and VBO for the grid if they don't exist
	static VAO gridVAO;
	static VBO gridVBO(gridVertices); // Initialize VBO with grid vertices

	gridVAO.Bind(); // Bind the VAO
	gridVAO.LinkAttrib(gridVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position)); // Link position
	gridVAO.Unbind(); // Unbind the VAO

	// Step 3: Set the shader and draw the grid
	shader.Activate(); // Activate the shader
	glm::mat4 model = glm::mat4(1.0f); // Identity matrix for the grid
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Pass camera matrices
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Draw the grid
	gridVAO.Bind();
	glLineWidth(1.0f); // Set line width
	glDrawArrays(GL_LINES, 0, gridVertices.size());
	gridVAO.Unbind();
}


int main(int argc, char** argv) {
	Logger::Init();

	Core core(WINDOW_SIZE.x, WINDOW_SIZE.y, "Eclipse Engine");

	if (!core.Initialize()) {
		std::cerr << "Engine initialization failed." << std::endl;
		return -1;
	}

	PanelHandler panelHandler(core.GetWindow());
	ilInit();  // Initialize the DevIL library

	std::vector<Texture> textures
	{
		Texture("Assets/Baker_house.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
	Shader gridShader("Shaders/grid.vert", "Shaders/grid.frag");

	GameObject cube;
	cube.AddComponent<Mesh>(vertices, indices, textures);

	GameObject house;
	std::string meshFilePath = "Assets/BakerHouse.fbx";
	house.AddComponent<Mesh>(meshFilePath);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	Camera camera(WINDOW_SIZE.x, WINDOW_SIZE.y, glm::vec3(7.0f, 4.0f, -7.0f));

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	gridShader.Activate();
	gridShader.SetMat4("view", camera.GetViewMatrix());
	gridShader.SetMat4("projection", camera.GetProjectionMatrix());
	glm::vec3 whiteColor(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(gridShader.ID, "gridColor"), 1, glm::value_ptr(whiteColor));

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

		panelHandler.NewFrame();

		core.BeginFrame();

		camera.Inputs(core.GetWindow());
		camera.UpdateMatrix(0.1f, 100.0f);

		DrawGrid(gridShader, camera);

		cube.Draw(shaderProgram, camera);
		house.Draw(shaderProgram, camera);

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
	Logger::Close();

	return EXIT_SUCCESS;
}