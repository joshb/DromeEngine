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

#ifndef __DROMEGUI_LABEL_H__
#define __DROMEGUI_LABEL_H__

#include <vector>
#include <DromeGui/Font.h>
#include <DromeGui/Widget.h>

namespace DromeGui {

class Label : public Widget
{
	protected:
		DromeCore::RefPtr <Font> m_font;
		std::string m_text;

	public:
		DromeCore::RefPtr <Font> getFont() const { return m_font; }
		void setFont(DromeCore::RefPtr <Font> value) { m_font = value; }

		std::string getText() const { return m_text; }
		void setText(const char *value) { m_text = value; }
		void setText(const std::string &value) { m_text = value; }

		void render(DromeGfx::GfxDriver *driver);

		static DromeCore::RefPtr <Label> create();
};

} // namespace DromeGui

#endif /* __DROMEGUI_LABEL_H__ */
