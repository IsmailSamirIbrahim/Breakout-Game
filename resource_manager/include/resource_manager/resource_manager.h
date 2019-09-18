#pragma once

#include "resource_manager/Exports.h"

#include <unordered_map>

#include <program/program.h>
#include <texture/texture.h>

namespace bko
{
	typedef struct IResource_Manager *Resource_Manager;

	struct IResource_Manager
	{
		std::unordered_map<const char*, bko::Program> programs;
		std::unordered_map<const char*, bko::Texture> textures;

		static Resource_Manager
		get_instance()
		{
			static IResource_Manager instance;
			return &instance;
		}		
	};

	RESOURCE_MANAGER_EXPORT void
	resource_manager_free(Resource_Manager self);

	inline static void
	destruct(Resource_Manager self)
	{
		resource_manager_free(self);
	}

	RESOURCE_MANAGER_EXPORT void
	resource_manager_load_program(Resource_Manager self, const char* vshader_path, const char* fshader_path, const char* name);

	RESOURCE_MANAGER_EXPORT Program
	resource_manager_program(Resource_Manager self, const char* name);

	RESOURCE_MANAGER_EXPORT void
	resource_manager_load_texture(Resource_Manager self, const char* image_path, const char* name);

	RESOURCE_MANAGER_EXPORT Texture
	resource_manager_texture(Resource_Manager self, const char* name);

} //namespace end