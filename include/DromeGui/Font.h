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

#ifndef __DROMEGUI_FONT_H__
#define __DROMEGUI_FONT_H__

#include <DromeGfx/Driver.h>

namespace DromeGui {

class Font : public DromeCore::RefClass
{
	protected:
		struct CharProperties
		{
			public:
				DromeMath::Vector2i position;
				DromeMath::Vector2i offset;
				int advance;
				unsigned int width;
		};

		unsigned int m_width, m_height;

		DromeCore::RefPtr <DromeGfx::Texture> m_texture;
		CharProperties *m_charProperties;

		Font();
		virtual ~Font();

		const CharProperties *getCharProperties(uint32_t c) const;
		void buildTexture(DromeGfx::GfxDriver *driver);

		virtual DromeCore::RefPtr <DromeGfx::Image> getCharImage(uint32_t c) = 0;
		virtual DromeMath::Vector2i getCharOffset(uint32_t c) = 0;
		virtual unsigned int getCharAdvance(uint32_t c) = 0;

	public:
		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }
		DromeCore::RefPtr <DromeGfx::Texture> getTexture() const { return m_texture; }

		DromeMath::Vector2i drawChar(DromeGfx::GfxDriver *driver, uint32_t c, const DromeMath::Vector2i &position, const DromeGfx::Color &color);
		DromeMath::Vector2i getStringDimensions(const char *s, const DromeMath::Vector2i &maxDimensions = DromeMath::Vector2i()) const;
		DromeMath::Rect2i drawString(DromeGfx::GfxDriver *driver, const char *s, const DromeMath::Rect2i &bounds, const DromeGfx::Color &color);
		DromeMath::Rect2i drawString(DromeGfx::GfxDriver *driver, const char *s, const DromeMath::Vector2i &position, const DromeGfx::Color &color);

		static DromeCore::RefPtr <Font> create(DromeGfx::GfxDriver *driver, const char *filename, unsigned int width, unsigned int height);
		static DromeCore::RefPtr <Font> create(DromeGfx::GfxDriver *driver, const char *filename, float size);
};

} // namespace DromeGui

#endif /* __DROMEGUI_FONT_H__ */
