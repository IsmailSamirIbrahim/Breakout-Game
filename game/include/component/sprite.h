#pragma once

#include "game/Exports.h"

#include <texture/texture.h>

#include <glm/glm.hpp>

namespace bko
{
	struct Sprite
	{
		Texture texture;
		glm::vec3 color;
		glm::vec2 position;
		glm::vec2 size;
	};

} //namespace end