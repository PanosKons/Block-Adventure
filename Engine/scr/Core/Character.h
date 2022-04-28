#pragma once
#include "../Math/Vector.h"
struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	Vector2<unsigned int>   Size;       // Size of glyph
	Vector2<unsigned int>   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};