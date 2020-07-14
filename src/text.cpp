#include <text.hpp>
#include <transform.hpp>
#include <graphic_system.hpp>
#include <shader.hpp>
#include <gl_manager.hpp>
#include <camera.hpp>
#include <asset_manager.hpp>

jeBegin

jeDefineComponentBuilder(Text);

const float intervalOffset = 50.f;
const int characterShift = 6;
const std::vector<unsigned> textIndices = { 0, 2, 3, 1, 0, 2 };

Text::Text(Object* owner)
	: Renderer(owner), color(vec4::one)
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals of vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * textIndices.size(),
		static_cast<const void*>(&textIndices[0]), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);

	set_font(AssetManager::get_font("Default"));
}

Text::~Text()
{
	if (buffer_) {
		delete[] buffer_;
		buffer_ = nullptr;
	}

	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
}
void Text::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void Text::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Text::load(const rapidjson::Value& /*data*/) {

}

void Text::start_draw(Camera* camera, const mat4& perspective, const vec3& resScalar)
{
	Shader* shader = GLManager::shader_[GLManager::Pipeline::TEXT];
	shader->use();

	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec4("v4_color", color);

	mat4 viewport;

	if (prjType_ == ProjectType::PERSPECTIVE) {

		viewport = mat4::look_at(camera->position_, camera->target_, camera->up_);
		shader->set_matrix("m4_projection", perspective);
	}

	else {

		viewport = mat4::scale(resScalar);

		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);
}

void Text::draw()
{
	if (!text_.empty()) {

		const vec3 scale = transform_->scale;
		const vec3 pos = transform_->position;
		const float nextLineInverval = font_->newline * font_->size * scale.y / intervalOffset;

		float initX = float(pos.x), newX = initX, intervalY = 0.f;
		int num_newline = 1;

		// Iterate all character
		std::wstring::const_iterator letter;
		for (auto letter = text_.begin(); letter != text_.end(); ++letter)
		{
			const wchar_t newline = *letter;
			if (newline == L'\n') {
				newX = initX;
				intervalY = nextLineInverval * num_newline;
				++num_newline;
			}

			else 
				render_character(*letter, newX, intervalY);
		}
	}
}

void Text::render_character(unsigned long key, float& newX, float intervalY)
{
	const vec3 scale = transform_->scale;
	vec3 position = transform_->position;
	Character character = font_->data[key];

	position.x = newX + character.bearing.x * scale.x;
	position.y = position.y - (character.size.y - character.bearing.y) * scale.y - intervalY;
	position.z = position.z;

	newX += (character.advance >> characterShift) * scale.x;

	Shader* shader = GLManager::shader_[GLManager::Pipeline::TEXT];
	shader->set_matrix("m4_translate", mat4::translate(position));

	GLfloat width = character.size.x;
	GLfloat height = character.size.y;

	//Update vbo
	float vertices[4][8] = {
		{ 0.f, height, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f },
		{ width, height, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f},
		{ 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f,},
		{ width, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f }
	};

	glBindTexture(GL_TEXTURE_2D, character.texture);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLE_STRIP, GLsizei(textIndices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}

void Text::end_draw()
{
}

void Text::set_text(const wchar_t* text, ...)
{
	if (text)
	{
		size_t newSize = 0;
		text_.assign(text);
		va_list argumens;

		va_start(argumens, text);
		// Get new size of the new text
		newSize = _vscwprintf(text, argumens) + 1;

		// If new size is greater than one,
		// reallocate the new heap memories
		if (size_ < newSize) {
			size_ = newSize; // Initialize new size
			delete[] buffer_; // Deallocate heap memories
			buffer_ = nullptr;
			buffer_ = new wchar_t[size_]; // Reallocate new memories
		}

		vswprintf_s(buffer_, size_, text, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		text = buffer_;
	}
}

const std::wstring& Text::get_text() const { return text_; }

Font* Text::get_font() const { return font_; }

void Text::set_font(Font* font) { font_ = font; }

jeEnd