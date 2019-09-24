#include "component/background.h"

using namespace glm;

namespace bko
{
	Background
	background_new(const Texture& texture, const vec2& position, const vec2& size)
	{
		Background self{};

		self.sprite.texture = texture;
		self.sprite.color = vec3{ 1.0f, 1.0f , 1.0f };
		self.sprite.position = position;
		self.sprite.size = size;

		return self;
	}
	
	void
	background_free(Background& self)
	{
		destruct(self.sprite.texture);
	}
}