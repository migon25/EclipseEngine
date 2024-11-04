#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentShader);

		void Activate();
		void Delete();

		void SetMat4(const std::string& name, const glm::mat4& matrix) const {
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		}
};

#endif //SHADER_H
