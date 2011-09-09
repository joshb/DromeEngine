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

#include <DromeGui/Picture.h>

using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

namespace DromeGui {

Picture::Picture(GfxDriver * /*driver*/, RefPtr <Image> image)
{
	m_imageWidth = image->getWidth();
	m_imageHeight = image->getHeight();
	setWidth(m_imageWidth);
	setHeight(m_imageHeight);

	// find the smallest power of 2 greater than or equal
	// to the image's width/height
	int textureSize = 2;
	while(textureSize < getWidth() || textureSize < getHeight())
		textureSize *= 2;

	// create new image with the calculated size
	// and copy the given image to it
	RefPtr <Image> newImage = Image::create(textureSize, textureSize, image->getNumComponents());
	newImage->copyFrom(image);

	// create texture
	m_texture = Texture::create(newImage);
}

void
Picture::render(GfxDriver *driver)
{
	Rect2i src(Vector2i(), Vector2i(m_imageWidth, m_imageHeight));
	driver->drawPic(m_texture, Color(), src, m_bounds);
}

RefPtr <Picture>
Picture::create(GfxDriver *driver, RefPtr <Image> image)
{
	return RefPtr <Picture> (new Picture(driver, image));
}

} // namespace DromeGui
