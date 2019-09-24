#pragma once

#include "game/Exports.h"

#include "component/sprite.h"

namespace bko
{
	struct Player_Paddle
	{
		Sprite sprite;
		glm::vec2 velocity;
	};

	GAME_EXPORT Player_Paddle
	player_paddle_new(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& velocity);

	GAME_EXPORT void
	player_paddle_free(Player_Paddle& self);

	inline static void
	destruct(Player_Paddle& self)
	{
		player_paddle_free(self);
	}

} //namespace end