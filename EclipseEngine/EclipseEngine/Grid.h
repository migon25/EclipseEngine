#ifndef GRID_H
#define GRID_H

#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "Camera.h"

class Grid
{
public:
	std::vector<Vertex> gridVertices;
	Shader& shader;
	Camera& camera;
	VAO vao;

	Grid(Shader& shader, Camera& camera);

	void Draw();

public:
	const float gridSize = 10.0f; // Size of the grid
	const int gridLines = 20;      // Number of lines in each direction
	const float lineSpacing = gridSize / gridLines;
};

#endif // GRID_H