#include "program/program.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

namespace bko
{
	// Helper Functions
	void
	_check_compile_shader_error(GLuint shader)
	{
		// check for shader compile errors
		int success;
		char info_log[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << endl;
		}
	}

	void
	_check_compile_program_error(GLuint program)
	{
		int success;
		char info_log[512];
		// check for linking errors
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
		}
	}

	// API
	Program
	program_new(const char* vertex_shader_source, const char* fragment_shader_source)
	{
		Program self{};

		// vertex shader
		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		glCompileShader(vertex_shader);
		_check_compile_shader_error(vertex_shader);

		//fragmnet shader
		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);
		_check_compile_shader_error(fragment_shader);

		// link shaders
		self.id = glCreateProgram();
		glAttachShader(self.id, vertex_shader);
		glAttachShader(self.id, fragment_shader);
		glLinkProgram(self.id);
		_check_compile_program_error(self.id);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return self;
	}

	void
	program_free(Program self)
	{
		glDeleteProgram(self.id);
	}

	void
	program_use(Program self)
	{
		glUseProgram(self.id);
	}

	void
	program_float_set(Program self, const char * name, GLfloat val)
	{
		glUniform1f(glGetUniformLocation(self.id, name), val);
	}

	void
	program_int_set(Program self, const char * name, GLint val)
	{
		glUniform1i(glGetUniformLocation(self.id, name), val);
	}

	void
	program_vec2f_set(Program self, const char * name, GLfloat x, GLfloat y)
	{
		 glUniform2f(glGetUniformLocation(self.id, name), x, y);
	}

	void
	program_vec2f_set(Program self, const char * name, const glm::vec2 & val)
	{
		 glUniform2f(glGetUniformLocation(self.id, name), val.x, val.y);
	}

	void
	program_vec3f_set(Program self, const char * name, GLfloat x, GLfloat y, GLfloat z)
	{
		 glUniform3f(glGetUniformLocation(self.id, name), x, y, z);
	}

	void
	program_vec3f_set(Program self, const char * name, const glm::vec3 & val)
	{
		 glUniform3f(glGetUniformLocation(self.id, name), val.x, val.y, val.z);
	}

	void
	program_vec4f_set(Program self, const char * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		 glUniform4f(glGetUniformLocation(self.id, name), x, y, z, w);
	}

	void
	program_vec4f_set(Program self, const char * name, const glm::vec4 & val)
	{
		 glUniform4f(glGetUniformLocation(self.id, name), val.x, val.y, val.z, val.w);
	}

	void
	program_mat4_set(Program self, const char * name, const glm::mat4 & matrix)
	{
		 glUniformMatrix4fv(glGetUniformLocation(self.id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
