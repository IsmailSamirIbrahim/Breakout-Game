#include "component/ball.h"

using namespace glm;

namespace bko
{
	Ball
	ball_new(const Texture& texture, const vec2& position, const vec2& velocity, GLfloat radius, GLboolean is_stuck)
	{
		Ball self{};

		self.sprite.texture = texture;
		self.sprite.color = vec3{ 1.0f, 1.0f , 1.0f };
		self.sprite.position = position;
		self.sprite.size = vec2{ radius * 2, radius * 2 };
		self.velocity = velocity;
		self.radius = radius;
		self.is_stuck = is_stuck;

		return self;
	}
	
	void
	ball_free(Ball& self)
	{
		destruct(self.sprite.texture);
	}
}