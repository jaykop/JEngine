#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <renderer.hpp>
#include <vec2.hpp>
#include <vec4.hpp>

jeBegin

struct Character {
	unsigned texture = 0; // ID handle of the glyph texture
	unsigned advance = 0; // Horizontal offset to advance to next glyph
	vec2 size = vec2::zero;	// Size of glyph
	vec2 bearing = vec2::zero; // Offset from baseline to left/top of glyph
};

struct Font {

	using FontData = std::unordered_map<unsigned long, Character>;

	FontData data;
	FT_Face	face = 0;
	FT_Library lib = 0;
	unsigned size = 0;
	float newline = 0.f;
};

class Text : public Renderer
{
	jeBaseFriends(Text);
	friend class GraphicSystem;

public:

	Text(Object* owner);
	virtual ~Text();

	void set_text(const wchar_t* pText, ...);
	const std::wstring& get_text() const;

	Font* get_font() const;
	void set_font(Font* font); 

	vec4 color;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

	void draw(float dt) override;

private:

	void render_character(unsigned long key, float& newX, float intervalY);

	unsigned vao_, vbo_, ebo_;
	Font* font_ = nullptr;
	wchar_t* buffer_ = nullptr;
	std::wstring text_;
	size_t size_ = 0;
	
};

jeDeclareComponentBuilder(Text);

jeEnd