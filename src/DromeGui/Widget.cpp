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

#include <DromeGui/Widget.h>

using namespace DromeMath;

namespace DromeGui {

Rect2i
Widget::getBounds() const
{
	return m_bounds;
}

void
Widget::setBounds(const DromeMath::Rect2i &value)
{
	m_bounds = value;
}

int
Widget::getX() const
{
	return m_bounds.min.x;
}

void
Widget::setX(int value)
{
	m_bounds.max.x += value - m_bounds.min.x;
	m_bounds.min.x = value;
}

int
Widget::getY() const
{
	return m_bounds.min.y;
}

void
Widget::setY(int value)
{
	m_bounds.max.y += value - m_bounds.min.y;
	m_bounds.min.y = value;
}

int
Widget::getWidth() const
{
	return m_bounds.getWidth();
}

void
Widget::setWidth(int value)
{
	m_bounds.max.x = m_bounds.min.x + value;
}

int
Widget::getHeight() const
{
	return m_bounds.getHeight();
}

void
Widget::setHeight(int value)
{
	m_bounds.max.y = m_bounds.min.y + value;
}

} // namespace DromeGui
