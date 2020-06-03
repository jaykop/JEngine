#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <vec2.hpp>
#include <macros.hpp>

jeBegin

struct Character {
	unsigned texture;	// ID handle of the glyph texture
	unsigned advance;	// Horizontal offset to advance to next glyph
	vec2 size;		// Size of glyph
	vec2 bearing;	// Offset from baseline to left/top of glyph
};

struct Font {

	using FontData = std::map<unsigned long, Character>;

	FontData	data;
	FT_Face		face;
	FT_Library	lib;
	unsigned	size;
	float		newline;
};

jeEnd