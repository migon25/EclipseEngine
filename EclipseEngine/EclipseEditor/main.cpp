#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "EclipseEngine/Shader.h"
#include "EclipseEngine/VAO.h"
#include "EclipseEngine/VBO.h"
#include "EclipseEngine/EBO.h"
#include "PanelHandler.h"
#include "FPSpanel.h"
#include "ConsolePanel.h"

using namespace std;

using vec3 = glm::dvec3;
using mat4 = glm::dmat4x4;
using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(711, 400);

GLfloat vertices[] =
{
	-0.5,-0.5 * float(sqrt(3)) / 3,     0.0f,    0.2f, 0.2f, 0.2f,  0.0f, 0.0f,
	 0.5,-0.5 * float(sqrt(3)) / 3, 	0.0f,    0.2f, 0.2f, 0.5f,  0.0f, 1.0f,
	 0.0, 0.5 * float(sqrt(3)) * 2 / 3, 0.0f,    0.2f, 0.2f, 0.9f,  1.0f, 1.0f
};

GLuint indices[] =
{
	0,1,2,
};

static void initOpenGL() {
	glewInit();
	glViewport(0,0,WINDOW_SIZE.x, WINDOW_SIZE.y);
	glClearColor(0.35f, 0.34f, 0.30f, 1.0);
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

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

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

		// Rendering ImGui
		panelHandler.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// destroy the window
	glfwDestroyWindow(window);
	// terminate glfw
	glfwTerminate();
	return EXIT_SUCCESS;
}