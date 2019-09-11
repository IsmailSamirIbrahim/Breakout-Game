#pragma once

#include "program/Exports.h"

#include <GL/glew.h>

namespace bko
{
	struct Program
	{		
		GLuint id;

		operator bool()
		{
			return id != 0;
		}
	};

	PROGRAM_EXPORT Program
	program_new(const char* vertex_shader_source, const char* fragment_shader_cource);

	PROGRAM_EXPORT void
	program_free(Program self);

	PROGRAM_EXPORT void
	program_use(Program self);

} //namespace end