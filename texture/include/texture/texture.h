#pragma once

#include <texture/Exports.h>

#include <GL/glew.h>

namespace bko
{
	enum class PIXEL_FORMAT :int
	{
		RGBA,
		RGB
	};

	struct Texture
	{
		GLuint id;
		GLuint width;
		GLuint height;
		GLuint internal_format;
		GLuint image_format;
	};

	TEXTURE_EXPORT Texture
	texture_new(GLuint width, GLuint height, unsigned char* data, PIXEL_FORMAT internal_format, PIXEL_FORMAT image_format);

	TEXTURE_EXPORT void
	texture_free(Texture self);

	TEXTURE_EXPORT void
	texture_bind(Texture self, GLuint texture_unit);

	TEXTURE_EXPORT void
	texture_unbind(Texture self);

} //namespace end