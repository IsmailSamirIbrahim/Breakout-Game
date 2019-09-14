#include <texture/texture.h>

namespace bko
{

	Texture
	texture_new(GLuint width, GLuint height, unsigned char* data, GLuint internal_format, GLuint image_format)
	{
		Texture self{};

		self.width = width;
		self.height = height;
		self.internal_format = internal_format;
		self.image_format = image_format;

		//create texture
		glGenTextures(1, &self.id);

		//create texture image
		glBindTexture(GL_TEXTURE_2D, self.id);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			internal_format,
			width,
			height,
			0,
			image_format,
			GL_UNSIGNED_BYTE,
			data);

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