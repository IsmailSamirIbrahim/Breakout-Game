#include <game/sprite_renderer.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace bko
{
	inline static void
	_init_render_data(Sprite_Renderer& self)
	{
		// Configure VAO/VBO
		GLuint vbo;
		GLfloat vertices[] = 
		{
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &self.quad_vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(self.quad_vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);

		glDeleteBuffers(1, &vbo);
	}

	Sprite_Renderer
	sprite_renderer_new(const Program& program, const Texture& texture, const vec2& size, const vec2& position, GLfloat rotate_angle, const vec3& color)
	{
		Sprite_Renderer self{};

		self.program = program;
		self.texture = texture;
		self.size = size;
		self.position = position;
		self.rotate_angle = rotate_angle;
		self.color = color;

		_init_render_data(self);

		return self;
	}
	
	void
	sprite_renderer_free(Sprite_Renderer self)
	{
		glDeleteVertexArrays(1, &self.quad_vao);
	}
	
	void
	sprite_renderer_render(Sprite_Renderer self)
	{
		program_use(self.program);

		mat4 model = mat4(1.0);

		model = translate(model, vec3{ self.position, 0.0f });

		model = translate(model, vec3{ 0.5f * self.size.x, 0.5f * self.size.y, 0.0f });
		model = rotate(model, self.rotate_angle, vec3{ 0.0f, 0.0f, 1.0f });
		model = translate(model, vec3(-0.5f * self.size.x, -0.5f * self.size.y, 0.0f));

		model = scale(model, vec3{ self.size, 1.0f });

		program_mat4f_set(self.program, "model", model);
		program_vec3f_set(self.program, "spriteColor", self.color);

		texture_bind(self.texture, GL_TEXTURE0);

		//draw
		glBindVertexArray(self.quad_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}
