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

#ifndef __DROMECORE_FONT_H__
#define __DROMECORE_FONT_H__

#include <DromeMath/DromeMath>
#include <DromeCore/Image.h>

namespace DromeCore {

class Font : public RefClass
{
	protected:
		unsigned int m_width, m_height;

		Font();
		virtual ~Font();

	public:
		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }

		virtual RefPtr <Image> getCharImage(uint32_t c, const DromeMath::Color4 &color) const = 0;
		virtual DromeMath::Vector2i getCharOffset(uint32_t c) const = 0;
		virtual unsigned int getCharAdvance(uint32_t c) const = 0;

		DromeMath::Vector2i drawChar(RefPtr <Image> image, uint32_t c, const DromeMath::Vector2i &position, const DromeMath::Color4 &color);
		DromeMath::Vector2i getStringDimensions(const char *s, const DromeMath::Vector2i &maxDimensions = DromeMath::Vector2i()) const;
		DromeMath::Rect2i drawString(RefPtr <Image> image, const char *s, const DromeMath::Rect2i &bounds, const DromeMath::Color4 &color);
		DromeMath::Rect2i drawString(RefPtr <Image> image, const char *s, const DromeMath::Vector2i &position, const DromeMath::Color4 &color);

		static DromeCore::RefPtr <Font> create(const char *filename, unsigned int width, unsigned int height);
		static DromeCore::RefPtr <Font> create(const char *filename, float size);
};

} // namespace DromeCore

#endif /* __DROMECORE_FONT_H__ */
