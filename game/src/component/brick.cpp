#include "component/brick.h"

using namespace glm;

namespace bko
{
	Brick
	brick_new(const Texture& texture, const vec3& color, const vec2& position, const vec2& size, GLboolean is_solid, GLboolean is_destroyed)
	{
		Brick self{};

		self.sprite.texture = texture;
		self.sprite.color = color;
		self.sprite.position = position;
		self.sprite.size = size;
		self.is_solid = is_solid;
		self.is_destroyed = is_destroyed;

		return self;
	}

	void
	brick_free(Brick& self)
	{
		destruct(self.sprite.texture);
	}
}