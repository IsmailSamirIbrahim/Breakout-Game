#include <texture/texture.h>

#include <assert.h>

namespace bko
{
	// Helper Functions
	inline static GLint
	_map_format(Texture::FORMAT format)
	{
		switch (format)
		{
		case Texture::FORMAT_RGB:
			return GL_RGB;

		case Texture::FORMAT_RGBA:
			return GL_RGBA;

		default:
			assert("invalid pixel format");
			return GL_RGBA;
		}
	}


	// API
	Texture
	texture_new(GLuint width, GLuint height, unsigned char* image, Texture::FORMAT internal_format, Texture::FORMAT image_format)
	{
		Texture self{};

		self.width = width;
		self.height = height;
		self.internal_format = _map_format(internal_format);
		self.image_format = _map_format(image_format);

		//create texture
		glGenTextures(1, &self.id);

		//create texture image
		glBindTexture(GL_TEXTURE_2D, self.id);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			self.internal_format,
			width,
			height,
			0,
			self.image_format,
			GL_UNSIGNED_BYTE,
			image);

		// generate mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return self;
	}

	void
	texture_free(Texture self)
	{
		glDeleteTextures(1, &self.id);
	}

	void
	texture_bind(Texture self, GLuint texture_unit)
	{
		glActiveTexture(texture_unit);
		glBindTexture(GL_TEXTURE_2D, self.id);
	}

	void
	texture_unbind(Texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}