#pragma once

#include "game/Exports.h"

#include "component/sprite.h"

namespace bko
{
	struct Ball
	{
		Sprite sprite;
		glm::vec2 velocity;
		GLfloat radius;
		GLboolean is_stuck;
	};

	GAME_EXPORT Ball
	ball_new(const Texture& texture, const glm::vec2& position, const glm::vec2& velocity, GLfloat radius, GLboolean is_stuck);

	GAME_EXPORT void
	ball_free(Ball& self);

	inline static void
	destruct(Ball& self)
	{
		ball_free(self);
	}

} //namespace end