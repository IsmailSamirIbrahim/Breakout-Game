#include "resource_manager/resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>


using namespace std;
using namespace bko;

namespace bko
{
	// Helper functions
	inline static Program
	_load_program_from_file(std::string vshader_path, std::string fshader_path)
	{
		// retrieve the vertex/fragment source code from filePath
		std::string vertex_source;
		std::string fragment_source;

		// open files
		std::ifstream vertex_shader_file(vshader_path);
		std::ifstream fragment_shader_file(fshader_path);
		std::stringstream vshader_stream, fshader_stream;

		// Read file's buffer contents into streams
		vshader_stream << vertex_shader_file.rdbuf();
		fshader_stream << fragment_shader_file.rdbuf();
		
		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();
		
		// Convert stream into string
		vertex_source = vshader_stream.str();
		fragment_source = fshader_stream.str();

		// now create program object from shaders source code
		Program program = program_new(vertex_source.c_str(), fragment_source.c_str());

		return program;
	}

	inline static Texture
	_load_texture_from_file(std::string image_path)
	{
		// Load image
		int width, height, channels;
		unsigned char* image = stbi_load(image_path.c_str(), &width, &height, &channels, 0);
		
		int internal_format;
		int image_format;

		Texture texture{};

		if (channels == 4)
			texture = texture_new(width, height, image, PIXEL_FORMAT::RGB, PIXEL_FORMAT::RGBA);
		else
			texture = texture_new(width, height, image, PIXEL_FORMAT::RGB, PIXEL_FORMAT::RGB);

		stbi_image_free(image);

		return texture;
	}


	// API
	void
	resource_manager_free(Resource_Manager self)
	{
		// delete all programs
		for (auto program : self->programs)
			program_free(program.second);

		// delete all textures
		for (auto texture : self->textures)
			texture_free(texture.second);
	}

	void
	resource_manager_load_program(Resource_Manager self, std::string vshader_path, std::string fshader_path, std::string name)
	{
		self->programs[name] = _load_program_from_file(vshader_path, fshader_path);
	}

	Program
	resource_manager_program(Resource_Manager self, std::string name)
	{
		return self->programs[name];
	}

	void
	resource_manager_load_texture(Resource_Manager self, std::string image_path, std::string name)
	{
		self->textures[name] = _load_texture_from_file(image_path);
	}

	Texture
	resource_manager_texture(Resource_Manager self, std::string name)
	{
		return self->textures[name];
	}
}