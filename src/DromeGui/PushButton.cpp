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

#include <DromeGui/PushButton.h>

using namespace std;
using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

namespace DromeGui {

PushButton::PushButton(GfxDriver *driver, RefPtr <Image> image,
                       RefPtr <Font> font)
 : Picture(driver, image)
{
	m_font = font;
}

string
PushButton::getText() const
{
	return m_text;
}

void
PushButton::setText(const string &value)
{
	m_text = value;
}

void
PushButton::setText(const char *value)
{
	m_text = value;
	m_textBounds = m_bounds;
}

void
PushButton::render(GfxDriver *driver)
{
	Picture::render(driver);

	Vector2i textSize = m_font->getStringDimensions(m_text.c_str(), m_bounds.getDimensions());
	Rect2i bounds = m_bounds.min + ((m_bounds.getDimensions() - textSize) / 2);
	bounds.max = bounds.min + textSize;

	m_textBounds = m_font->drawString(driver, m_text.c_str(), bounds, Color());
}

RefPtr <PushButton>
PushButton::create(GfxDriver *driver, RefPtr <Image> image, RefPtr <Font> font)
{
	return RefPtr <PushButton> (new PushButton(driver, image, font));
}

} // namespace DromeGui
