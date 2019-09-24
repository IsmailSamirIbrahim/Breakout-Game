#include "component/player_paddle.h"

using namespace glm;

namespace bko
{
	Player_Paddle
	player_paddle_new(const Texture& texture, const vec2& position, const vec2& size, const vec2& velocity)
	{
		Player_Paddle self{};

		self.sprite.texture = texture;
		self.sprite.color = vec3{ 1.0f, 1.0f , 1.0f };
		self.sprite.position = position;
		self.sprite.size = size;
		self.velocity = velocity;

		return self;
	}

	void
	player_paddle_free(Player_Paddle& self)
	{
		destruct(self.sprite.texture);
	}
}