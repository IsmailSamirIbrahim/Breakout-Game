#include "resource_manager/resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>
#include <string>


using namespace std;
using namespace bko;

namespace bko
{
	// Helper functions
	inline static Program
	_load_program_from_file(const char* vshader_path, const char* fshader_path)
	{
		// open files
		ifstream vertex_shader_file(vshader_path);
		ifstream fragment_shader_file(fshader_path);
		stringstream vshader_stream, fshader_stream;

		// Read file's buffer contents into streams
		vshader_stream << vertex_shader_file.rdbuf();
		fshader_stream << fragment_shader_file.rdbuf();
		
		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();
		
		// Convert stream into string
		string vertex_source = vshader_stream.str();
		string fragment_source = fshader_stream.str();

		if (vertex_source.empty() || fragment_source.empty())
		{
			printf("ERROR:: Can't load shaders\n");
			return Program{};
		}

		// now create program object from shaders source code
		Program program = program_new(vertex_source.c_str(), fragment_source.c_str());

		return program;
	}

	inline static Texture
	_load_texture_from_file(const char* image_path)
	{
		// Load image
		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;
		unsigned char* image = stbi_load(image_path, &width, &height, &channels, 0);

		if (image == NULL)
		{
			printf("ERROR:: Can't load texture\n");
			return Texture{};
		}
		
		int internal_format;
		int image_format;

		Texture texture{};

		if (channels == 4)
			texture = texture_new(width, height, image, Texture::RGBA, Texture::RGBA);
		else
			texture = texture_new(width, height, image, Texture::RGB, Texture::RGB);

		stbi_image_free(image);

		return texture;
	}


	// API
	void
	resource_manager_free(Resource_Manager self)
	{
		// delete all programs
		for (auto program : self->programs)
			destruct(program.second);

		// delete all textures
		for (auto texture : self->textures)
			destruct(texture.second);
	}

	void
	resource_manager_load_program(Resource_Manager self, const char* vshader_path, const char* fshader_path, const char* name)
	{
		self->programs[name] = _load_program_from_file(vshader_path, fshader_path);
	}

	Program
	resource_manager_program(Resource_Manager self, const char* name)
	{
		for (auto pair : self->programs)
			if (strcmp(pair.first, name) == 0)
				return pair.second;
	}

	void
	resource_manager_load_texture(Resource_Manager self, const char* image_path, const char* name)
	{
		self->textures[name] = _load_texture_from_file(image_path);
	}

	Texture
	resource_manager_texture(Resource_Manager self, const char* name)
	{
		for (auto pair : self->textures)
			if (strcmp(pair.first, name) == 0)
				return pair.second;
	}
}