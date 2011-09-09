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
#include "CoreGraphicsFont.h"

using namespace std;
using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

namespace DromeGui {

static CGGlyph
charToCGGlyph(uint32_t c)
{
	return (CGGlyph)(c - 29);
}

CoreGraphicsFont::CoreGraphicsFont(GfxDriver *driver, const char *filename,
                                   unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	// create data provider
	CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(filename);
	if(!dataProvider)
		throw Exception(string("CoreGraphicsFont::CoreGraphicsFont(): Couldn't open ") + filename);

	// create font and release the data provider
	m_font = CGFontCreateWithDataProvider(dataProvider);
	CGDataProviderRelease(dataProvider);

	// make sure the font was created
	if(!m_font)
		throw Exception(string("CoreGraphicsFont::CoreGraphicsFont(): CGFontCreateWithDataProvider failed"));

	int unitsPerEm = CGFontGetUnitsPerEm(m_font);
	m_glyphToPixelFactor = (float)m_width / (float)unitsPerEm;

	buildTexture(driver);
}

CoreGraphicsFont::~CoreGraphicsFont()
{
	CGFontRelease(m_font);
}

RefPtr <Image>
CoreGraphicsFont::getCharImage(uint32_t c)
{
	CGGlyph glyphs[] = { charToCGGlyph(c) };

	// get glyph bounding box and calculate image size/character offset
	CGRect bbox;
	CGFontGetGlyphBBoxes(m_font, glyphs, 1, &bbox);
	unsigned int width = (unsigned int)(bbox.size.width * m_glyphToPixelFactor + 0.5f);
	unsigned int height = (unsigned int)(bbox.size.height * m_glyphToPixelFactor + 0.5f);
	float offsetX = -bbox.origin.x * m_glyphToPixelFactor;
	float offsetY = -bbox.origin.y * m_glyphToPixelFactor;

	if(width == 0 || height == 0)
		return RefPtr <Image> (NULL);

	// create image
	RefPtr <Image> img = Image::create(width, height, 1);

	// create color space
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
	if(!colorSpace)
		throw Exception("CoreGraphicsFont::getCharImage(): CGColorSpaceCreateDeviceGray failed");

	// create CGBitmapContext to draw to the image and release the color space
	CGContextRef context = CGBitmapContextCreate((void *)img->getData(), width, height, 8, width, colorSpace, 0);
	CGColorSpaceRelease(colorSpace);

	// make sure the context was created
	if(!context)
		throw Exception("CoreGraphicsFont::getCharImage(): CGBitmapContextCreate failed");

	// draw character
	CGContextSetFont(context, m_font);
	CGContextSetFontSize(context, (float)m_width);
	CGContextSetGrayFillColor(context, 1.0f, 1.0f);
	CGContextShowGlyphsAtPoint(context, offsetX, offsetY, glyphs, 1);

	// release the context
	CGContextRelease(context);

	return img;
}

Vector2i
CoreGraphicsFont::getCharOffset(uint32_t c)
{
	CGGlyph glyphs[] = { charToCGGlyph(c) };

	// get glyph bounding box and calculate image height/character offset
	CGRect bbox;
	CGFontGetGlyphBBoxes(m_font, glyphs, 1, &bbox);
	unsigned int height = (unsigned int)(bbox.size.height * m_glyphToPixelFactor + 0.5f);
	float offsetX = bbox.origin.x * m_glyphToPixelFactor;
	float offsetY = bbox.origin.y * m_glyphToPixelFactor;

	return Vector2i((int)offsetX, (int)(m_height - height) - (int)offsetY);
}

unsigned int
CoreGraphicsFont::getCharAdvance(uint32_t c)
{
	CGGlyph glyphs[] = { charToCGGlyph(c) };

	int advance;
	CGFontGetGlyphAdvances(m_font, glyphs, 1, &advance);

	return (unsigned int)((float)advance * m_glyphToPixelFactor + 0.5f);
}

RefPtr <CoreGraphicsFont>
CoreGraphicsFont::create(GfxDriver *driver, const char *filename,
                         unsigned int width, unsigned int height)
{
	return RefPtr <CoreGraphicsFont> (new CoreGraphicsFont(driver, filename, width, height));
}

} // namespace DromeGui
