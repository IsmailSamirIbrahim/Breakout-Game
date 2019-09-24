#pragma once

#include "game/Exports.h"

#include "component/sprite.h"

namespace bko
{
	struct Brick
	{
		Sprite sprite;
		GLboolean is_solid;
		GLboolean is_destroyed;
	};

	GAME_EXPORT Brick
	brick_new(const Texture& texture, const glm::vec3& color, const glm::vec2& position, const glm::vec2& size, GLboolean is_solid, GLboolean is_destroyed);

	GAME_EXPORT void
	brick_free(Brick& self);

	inline static void
	destruct(Brick& self)
	{
		brick_free(self);
	}

} //namespace end