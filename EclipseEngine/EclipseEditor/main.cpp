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
#include "EclipseEngine/Mesh.h"
#include "PanelHandler.h"
#include "FPSpanel.h"
#include "ConsolePanel.h"

using namespace std;

using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1100, 619);

Vertex vertices[] =
{ //               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // 0
	Vertex{glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},  // 1
	Vertex{glm::vec3( 1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},	 // 2
	Vertex{glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},	 // 3
	Vertex{glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},	 // 4
	Vertex{glm::vec3( 1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},	 // 5
	Vertex{glm::vec3(-1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},	 // 6
	Vertex{glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}	 // 7
};

GLuint indices[] =
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

static void initOpenGL() 
{
	glewInit();
	glViewport(0,0,WINDOW_SIZE.x, WINDOW_SIZE.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
	
	glfwInit();

	// defining what version of opengl we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating a window
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "ECLIPSE ENGINE", NULL, NULL);
	if (window == NULL)
	{
		cout << "failed to create GLFW WINDOW" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	initOpenGL();

	PanelHandler panelHandler(window);
	ilInit();  // Initialize the DevIL library

	Texture textures[]
	{
		Texture("Assets/Baker_house.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};

	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh cube(verts, ind, tex);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Camera camera(WINDOW_SIZE.x, WINDOW_SIZE.y, glm::vec3(0.0f, 0.0f, 2.0f));

	// Register the scroll callback for zoom control
	glfwSetScrollCallback(window, Camera::scroll_callback);

	// Variables to track FPS and milliseconds per frame
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	std::unique_ptr<Panel>* panelPtr = &panelHandler.GetPanel("FPS Panel");

	// console panel
	auto consolePanel = dynamic_cast<ConsolePanel*>(panelHandler.GetPanel("Console Panel").get());
  
	while (!glfwWindowShouldClose(window))
	{
		// fps
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float fps = 1.0f / deltaTime;
		float ms = deltaTime * 1000.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		panelHandler.NewFrame();

		// fps
		if (panelPtr) {
			if (auto* fpsPanel = dynamic_cast<FPSPanel*>(panelPtr->get())) {
				fpsPanel->Update(fps, ms);
			}
		}

		if (consolePanel) {
			if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
				// PLACEHOLDER OF THE CONSOLE FOR LOADING GEOMETRY THERE
				// SHOULD BE SMTHING IN THE ENGINE TO ACTIVATE THIS CODE
				consolePanel->Log("Loading geometry from ASSIMP...");
				// Load your geometry here
				consolePanel->Log("Geometry loaded successfully.");
			}
		}

		// Render all added panels
		panelHandler.RenderPanels();

		shaderProgram.Activate();
		glUniform1f(uniID, -0.5f);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		camera.Inputs(window);
		camera.UpdateMatrix(0.1f, 100.0f);

		cube.Draw(shaderProgram, camera);

		// Rendering ImGui
		panelHandler.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();

	// destroy the window
	glfwDestroyWindow(window);
	// terminate glfw
	glfwTerminate();
	return EXIT_SUCCESS;
}