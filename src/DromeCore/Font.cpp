/*
 * Copyright (C) 2010-2012 Josh A. Beam
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
#include <DromeCore/Font.h>
#ifdef APPLE
	#include "CoreGraphicsFont.h"
#else
	#include "FreeTypeFont.h"
#endif /* APPLE */

using namespace DromeMath;

namespace DromeCore {

Font::Font()
{
	m_width = 0;
	m_height = 0;
}

Font::~Font()
{
}

Vector2i
Font::drawChar(RefPtr <Image> image, uint32_t c, const Vector2i &position,
               const Color4 &color)
{
	RefPtr <Image> charImage = getCharImage(c, color);
	Vector2i p = position + getCharOffset(c);

	// calculate source and destination coordinates
	Rect2i src = Rect2i(Vector2i(0, 0), Vector2i((int)charImage->getWidth(), (int)charImage->getHeight()));
	Rect2i dest = Rect2i(p, p + src.getDimensions());

	// copy character to destination image
	image->copyFrom(charImage, src, dest);

	// return destination position for next character to be drawn
	return position + Vector2i(getCharAdvance(c), 0);
}

Vector2i
Font::getStringDimensions(const char *s, const Vector2i &maxDimensions) const
{
	Vector2i p, max;

	bool xbound = (maxDimensions.x > 0);
	bool ybound = (maxDimensions.y > 0);

	while(*s) {
		int advance = (int)getCharAdvance(*s);

		// start a new line if necessary
		if(*s == '\n') {
			p.y += m_height;
			p.x = 0;
			s++;
			continue;
		} else if(xbound) {
			if(p.x + advance >= maxDimensions.x) {
				p.y += m_height;
				p.x = 0;
				continue;
			}
		}

		// if the maximum y boundary has been reached, end the loop
		if(ybound && p.y >= maxDimensions.y)
			break;

		s++;
		p.x += advance;
		if(p.x > max.x)
			max.x = p.x;
	}

	max.x++;
	max.y = p.y + m_height;
	return max;
}

Rect2i
Font::drawString(RefPtr <Image> image, const char *s, const Rect2i &bounds,
                 const Color4 &color)
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
			if(p.x + (int)getCharAdvance(*s) >= bounds.max.x) {
				p.y += m_height;
				p.x = bounds.min.x;
				continue;
			}
		}

		// if the maximum y boundary has been reached, end the loop
		if(ybound && p.y >= bounds.max.y)
			break;

		p = drawChar(image, *s++, p, color);
		if(p.x > rect.max.x)
			rect.max.x = p.x;
	}

	rect.max.y = p.y + m_height;
	return rect;
}

Rect2i
Font::drawString(RefPtr <Image> image, const char *s, const Vector2i &position,
                 const Color4 &color)
{
	return drawString(image, s, Rect2i(position, position), color);
}

RefPtr <Font>
Font::create(const char *filename,
             unsigned int width, unsigned int height)
{
#ifdef APPLE
	return CoreGraphicsFont::create(File::getPath(filename).c_str(), width, height);
#else
	return FreeTypeFont::create(File::getPath(filename).c_str(), width, height);
#endif /* APPLE */
}

RefPtr <Font>
Font::create(const char *filename, float size)
{
	const float POINTS_MULTIPLIER = 1.0f;
	unsigned int isize = (unsigned int)(size * POINTS_MULTIPLIER);

	return create(filename, isize, isize);
}

} // namespace DromeGui
