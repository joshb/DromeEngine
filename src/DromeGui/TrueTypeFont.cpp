/*
 * Copyright (C) 2010 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <DromeCore/Exception.h>
#include "TrueTypeFont.h"

using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

namespace DromeGui {

TrueTypeFont::TrueTypeFont(GfxDriver *driver, const char *filename,
                           unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	if(FT_Init_FreeType(&library))
		throw Exception("TrueTypeFont::TrueTypeFont(): FT_Init_FreeType failed");

	if(FT_New_Face(library, filename, 0, &face))
		throw Exception("TrueTypeFont::TrueTypeFont(): FT_New_Face failed");

	if(FT_Set_Pixel_Sizes(face, width, height))
		throw Exception("TrueTypeFont::TrueTypeFont(): FT_Set_Pixel_Sizes failed");

	face->style_flags |= FT_STYLE_FLAG_BOLD;

	buildTexture(driver);
}

TrueTypeFont::~TrueTypeFont()
{
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

RefPtr <Image>
TrueTypeFont::getCharImage(uint32_t c)
{
	// load glyph for the given character
	if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		throw Exception("TrueTypeFont::getCharImage(): FT_Load_Char failed");

	uint8_t *glyphData = face->glyph->bitmap.buffer;
	unsigned int width = face->glyph->bitmap.width;
	unsigned int height = face->glyph->bitmap.rows;

	// create image
	RefPtr <Image> img = Image::create(width, height, 1);
	for(unsigned int y = 0; y < height; y++) {
		for(unsigned int x = 0; x < width; x++) {
			Color pixel;

			pixel.r = glyphData[width * y + x];
			pixel.g = pixel.b = pixel.a = pixel.r;

			img->setPixel(x, y, pixel);
		}
	}

	return img;
}

Vector2i
TrueTypeFont::getCharOffset(uint32_t c)
{
	// load glyph for the given character
	if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		throw Exception("TrueTypeFont::getCharOffset(): FT_Load_Char failed");

	return Vector2i(face->glyph->bitmap_left, m_height - face->glyph->bitmap_top);
}

unsigned int
TrueTypeFont::getCharAdvance(uint32_t c)
{
	// load glyph for the given character
	if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		throw Exception("TrueTypeFont::getCharAdvance(): FT_Load_Char failed");

	return face->glyph->advance.x >> 6;
}

RefPtr <TrueTypeFont>
TrueTypeFont::create(GfxDriver *driver, const char *filename,
                     unsigned int width, unsigned int height)
{
	return RefPtr <TrueTypeFont> (new TrueTypeFont(driver, filename, width, height));
}

} // namespace DromeGui
