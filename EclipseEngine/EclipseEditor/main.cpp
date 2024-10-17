#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include "EclipseEngine/Shader.h"
#include "EclipseEngine/VAO.h"
#include "EclipseEngine/VBO.h"
#include "EclipseEngine/EBO.h"

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
	
	// glfw init and enable context
	glfwInit();

	// defining what version of opengl we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating a window
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "ECLIPSE ENGINE", NULL, NULL);
	// checking if there's an error in window creation
	if (window == NULL)
	{
		cout << "failed to create GLFW WINDOW" << endl;
		glfwTerminate();
		return -1;
	}

	// tell opengl we want to use this window
	glfwMakeContextCurrent(window);

	initOpenGL();

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

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		glUniform1f(uniID, -0.5f);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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