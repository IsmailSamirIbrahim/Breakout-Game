#pragma once

#include "game/Exports.h"

#include "component/sprite.h"

namespace bko
{
	struct Background
	{
		Sprite sprite;
	};

	GAME_EXPORT Background
	background_new(const Texture& texture, const glm::vec2& position, const glm::vec2& size);

	GAME_EXPORT void
	background_free(Background& self);

	inline static void
	destruct(Background& self)
	{
		background_free(self);
	}

} //namespace end