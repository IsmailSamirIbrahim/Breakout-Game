#pragma once

#include <texture/Exports.h>

#include <GL/glew.h>

namespace bko
{
	struct Texture
	{
		enum FORMAT 
		{ 
			FORMAT_RGB,
			FORMAT_RGBA
		};

		GLuint id;
		GLuint width;
		GLuint height;
		GLuint internal_format;
		GLuint image_format;
	};

	TEXTURE_EXPORT Texture
	texture_new(GLuint width, GLuint height, unsigned char* image, Texture::FORMAT internal_format, Texture::FORMAT image_format);

	TEXTURE_EXPORT void
	texture_free(Texture self);

	TEXTURE_EXPORT void
	texture_bind(Texture self, GLuint texture_unit);

	TEXTURE_EXPORT void
	texture_unbind(Texture self);

} //namespace end