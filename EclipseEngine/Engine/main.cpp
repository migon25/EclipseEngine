#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

using vec3 = glm::dvec3;
using mat4 = glm::dmat4x4;
using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(640, 480);

struct Camera {
	vec3 pos = vec3( 0, 2, 10);
	vec3 to = vec3(0, 2, 0);
	vec3 up = vec3( 0, 1, 0);
	double fov = glm::radians(60.0);
	double aspect = static_cast<double>(WINDOW_SIZE.x) / WINDOW_SIZE.y;
	double zNear = 0.1;
	double zFar = 100.0;
};

struct Triangle {
	array<vec3, 3> verts = { vec3(-1, -1, 0), vec3(1, -1, 0), vec3(0, 1, 0) };
	vec3& a = verts[0];
	vec3& b = verts[1];
	vec3& c = verts[2];
};

struct CTriangle {
	vec3 color;
	Triangle geometry;
};

static void initOpenGL() {
	glewInit();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
	//glfw init and enable context
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW Simple Example", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	initOpenGL();

	Camera camera;

	array<CTriangle, 3> triangles = { CTriangle{vec3(1, 0, 0)}, CTriangle{vec3(0, 1, 0)}, CTriangle{vec3(0, 0, 1)} };
	for (auto& v : triangles[1].geometry.verts) v += vec3(1, 0, -1);
	for (auto& v : triangles[2].geometry.verts) v += vec3(2, 0, -2);

	//Set projection matrix
	auto proj_mat = glm::perspective(camera.fov, camera.aspect, camera.zNear, camera.zFar);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(&proj_mat[0][0]);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set view matrix
		auto view_mat = glm::lookAt(camera.pos, camera.to, camera.up);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(&view_mat[0][0]);

		//paint triangles
		for (const auto& triangle : triangles) {
			glColor3d(triangle.color.r, triangle.color.g, triangle.color.b);
			glBegin(GL_TRIANGLES);
			for (const auto& vert : triangle.geometry.verts) glVertex3d(vert.x, vert.y, vert.z);
			glEnd();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}