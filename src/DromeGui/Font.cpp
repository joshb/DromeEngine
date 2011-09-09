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

#include <DromeCore/File.h>
#include <DromeGui/Font.h>
#ifdef APPLE
	#include "CoreGraphicsFont.h"
#else
	#include "TrueTypeFont.h"
#endif /* APPLE */

using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

namespace DromeGui {

const unsigned int MIN_CHAR = 32;
const unsigned int MAX_CHAR = 126;
const unsigned int NUM_CHARS = MAX_CHAR - MIN_CHAR + 1;

Font::Font()
{
	m_width = 0;
	m_height = 0;

	// allocate memory for storing properties of all ASCII characters
	m_charProperties = new CharProperties [NUM_CHARS];
}

Font::~Font()
{
	delete [] m_charProperties;
}

const Font::CharProperties *
Font::getCharProperties(uint32_t c) const
{
	// return null if this is a non-ASCII character
	if(c < MIN_CHAR || c > MAX_CHAR)
		return NULL;

	return m_charProperties + (c - MIN_CHAR);
}

void
Font::buildTexture(GfxDriver * /*driver*/)
{
	unsigned int size = 256;
	while((m_width * m_height * NUM_CHARS) > (size * size))
		size *= 2;

	// create image for texture
	RefPtr <Image> img = Image::create(size, size, 2);

	unsigned int x = 0, y = 0;

	// loop through each ASCII character
	for(uint8_t i = MIN_CHAR; i <= MAX_CHAR; i++) {
		if(y + m_height >= size)
			break;

		// get image for character
		RefPtr <Image> charImg = getCharImage(i);
		unsigned int imgWidth = 0;
		if(charImg.isSet()) {
			imgWidth = charImg->getWidth();

			// add character image to texture image
			for(unsigned int j = 0; j < charImg->getHeight(); j++) {
				for(unsigned int k = 0; k < charImg->getWidth(); k++) {
					Color pixel = charImg->getPixel(k, j);
					pixel.a = pixel.r;
					img->setPixel(x + k, y + j, pixel);
				}
			}
		}

		// store character information
		m_charProperties[i-MIN_CHAR].position = Vector2i((int)x, (int)y);
		m_charProperties[i-MIN_CHAR].offset = getCharOffset(i);
		m_charProperties[i-MIN_CHAR].advance = getCharAdvance(i);
		m_charProperties[i-MIN_CHAR].width = imgWidth;

		// increment x position in image; if it reaches
		// the image width, wrap to the next line
		x += imgWidth;
		if(x + m_width >= size) {
			x = 0;
			y += m_height;
		}
	}

	// create texture out of font image
	m_texture = Texture::create(img);
}

Vector2i
Font::drawChar(GfxDriver *driver, uint32_t c, const Vector2i &position,
               const Color &color)
{
	// just return if this is a non-ASCII character
	if(c < MIN_CHAR || c > MAX_CHAR)
		return position;

	Vector2i p = position + m_charProperties[c-MIN_CHAR].offset;

	// calculate texture source and screen destination coordinates
	Rect2i src = Rect2i(m_charProperties[c-MIN_CHAR].position, m_charProperties[c-MIN_CHAR].position + Vector2i(m_charProperties[c-MIN_CHAR].width, m_height));
	Rect2i dest = Rect2i(p, p + src.getDimensions());

	// draw character
	driver->drawPic(m_texture, color, src, dest);

	// return destination position for next character to be drawn
	return position + Vector2i(m_charProperties[c-MIN_CHAR].advance, 0);
}

Vector2i
Font::getStringDimensions(const char *s, const Vector2i &maxDimensions) const
{
	Vector2i p, max;

	bool xbound = (maxDimensions.x > 0);
	bool ybound = (maxDimensions.y > 0);

	while(*s) {
		const CharProperties *props = getCharProperties(*s);

		// start a new line if necessary
		if(*s == '\n') {
			p.y += m_height;
			p.x = 0;
			s++;
			continue;
		} else if(xbound) {
			if(!props) {
				continue;
			} else if(p.x + props->advance >= maxDimensions.x) {
				p.y += m_height;
				p.x = 0;
				continue;
			}
		}

		// if the maximum y boundary has been reached, end the loop
		if(ybound && p.y >= maxDimensions.y)
			break;

		s++;
		p.x += props->advance;
		if(p.x > max.x)
			max.x = p.x;
	}

	max.x++;
	max.y = p.y + m_height;
	return max;
}

Rect2i
Font::drawString(GfxDriver *driver, const char *s, const Rect2i &bounds,
                 const Color &color)
{
	Rect2i rect = bounds;
	Vector2i p = bounds.min;

	bool xbound = (bounds.getWidth() > 0);
	bool ybound = (bounds.getHeight() > 0);

	// loop through and draw each character in the given string
	while(*s) {
		// start a new line if necessary
		if(*s == '\n') {
			p.y += m_height;
			p.x = bounds.min.x;
			s++;
			continue;
		} else if(xbound) {
			const CharProperties *props = getCharProperties(*s);
			if(!props) {
				continue;
			} else if(p.x + props->advance >= bounds.max.x) {
				p.y += m_height;
				p.x = bounds.min.x;
				continue;
			}
		}

		// if the maximum y boundary has been reached, end the loop
		if(ybound && p.y >= bounds.max.y)
			break;

		p = drawChar(driver, *s++, p, color);
		if(p.x > rect.max.x)
			rect.max.x = p.x;
	}

	rect.max.y = p.y + m_height;
	return rect;
}

Rect2i
Font::drawString(GfxDriver *driver, const char *s, const Vector2i &position,
                 const Color &color)
{
	return drawString(driver, s, Rect2i(position, position), color);
}

RefPtr <Font>
Font::create(GfxDriver *driver, const char *filename,
             unsigned int width, unsigned int height)
{
#ifdef APPLE
	return CoreGraphicsFont::create(driver, File::getPath(filename).c_str(), width, height);
#else
	return TrueTypeFont::create(driver, File::getPath(filename).c_str(), width, height);
#endif /* APPLE */
}

RefPtr <Font>
Font::create(GfxDriver *driver, const char *filename, float size)
{
	const float POINTS_MULTIPLIER = 1.0f;
	unsigned int isize = (unsigned int)(size * POINTS_MULTIPLIER);

	return create(driver, filename, isize, isize);
}

} // namespace DromeGui
