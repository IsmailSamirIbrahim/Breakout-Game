#include <game/sprite_renderer.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace bko
{
	struct ISprite_Renderer
	{
		Program program;
		GLuint quad_vao;
	};

	// Helper Functions
	inline static GLuint
	_init_render_data()
	{
		// Configure VAO/VBO
		GLuint vbo, vao;
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

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);

		glDeleteBuffers(1, &vbo);

		return vao;
	}


	// API
	Sprite_Renderer
	sprite_renderer_new(const Program& program)
	{
		Sprite_Renderer self = (Sprite_Renderer)::malloc(sizeof(ISprite_Renderer));

		self->program  = program;
		self->quad_vao = _init_render_data();

		return self;
	}
	
	void
	sprite_renderer_free(Sprite_Renderer self)
	{
		destruct(self->program);
		glDeleteVertexArrays(1, &self->quad_vao);
		::free(self);
	}
	
	void
	sprite_renderer_render(Sprite_Renderer self, Game_Object object)
	{
		// use program
		program_use(self->program);

		// create model matrix
		mat4 model = mat4(1.0);

		// translate
		model = translate(model, vec3{ object.position, 0.0f });

		// rotate
		model = translate(model, vec3{ 0.5f * object.size.x, 0.5f * object.size.y, 0.0f });
		model = rotate(model, object.rotation_angle, vec3{ 0.0f, 0.0f, 1.0f });
		model = translate(model, vec3(-0.5f * object.size.x, -0.5f * object.size.y, 0.0f));

		// scale
		model = scale(model, vec3{ object.size, 1.0f });

		program_mat4f_set(self->program, "model", model);
		program_vec3f_set(self->program, "spriteColor", object.color);

		texture_bind(object.texture, GL_TEXTURE0);

		//draw
		glBindVertexArray(self->quad_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}
