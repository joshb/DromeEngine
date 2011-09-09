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

#ifndef __DROMEGUI_TRUETYPEFONT_H__
#define __DROMEGUI_TRUETYPEFONT_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include <DromeGui/Font.h>

namespace DromeGui {

class TrueTypeFont : public Font
{
	protected:
		FT_Library library;
		FT_Face face;

		TrueTypeFont(DromeGfx::GfxDriver *driver, const char *filename, unsigned int width, unsigned int height);
		virtual ~TrueTypeFont();

		DromeCore::RefPtr <DromeGfx::Image> getCharImage(uint32_t c);
		DromeMath::Vector2i getCharOffset(uint32_t c);
		unsigned int getCharAdvance(uint32_t c);

	public:
		static DromeCore::RefPtr <TrueTypeFont> create(DromeGfx::GfxDriver *driver, const char *filename, unsigned int width, unsigned int height);
};

} // namespace DromeGui

#endif /* __DROMEGUI_TRUETYPEFONT_H__ */
