#include "game/game_object.h"

using namespace glm;

namespace bko
{
	Game_Object
	game_object_new(Texture texture, vec3 color, vec2 position, vec2 size, vec2 velocity, GLfloat rotation_angle, GLboolean is_solid, GLboolean is_destroyed)
	{
		Game_Object self{};

		self.texture = texture;
		self.color = color;
		self.position = position;
		self.size = size;
		self.velocity = velocity;
		self.rotation_angle = rotation_angle;
		self.is_solid = is_solid;
		self.is_destroyed = is_destroyed;

		return self;
	}

	void
	game_object_free(Game_Object self)
	{
		destruct(self.texture);
	}
}
