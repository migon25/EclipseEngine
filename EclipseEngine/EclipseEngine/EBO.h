#ifndef EBO_H
#define EBO_h

#include <vector>
#include <GL/glew.h>

class EBO
{
public:
	GLuint ID;
	EBO(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif //EBO_H
