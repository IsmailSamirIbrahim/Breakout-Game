#include "program/program.h"

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
}
