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

#include <DromeCore/Exception.h>
#include <DromeGL/Texture.h>

using namespace DromeCore;

namespace DromeGL {

Texture::Texture()
{
	// generate texture
	glGenTextures(1, &m_id);

	m_width = 0;
	m_height = 0;
}

Texture::Texture(RefPtr <Image> image)
{
	// determine the texture format from the image
	GLenum format;
	switch(image->getNumComponents()) {
		default:
			throw Exception("Texture::Texture(): Unsupported number of color components");
			break;
		case 1:
#ifndef GL_RED
            format = GL_LUMINANCE;
#else
            format = GL_RED;
#endif
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
	}

	m_width = image->getWidth();
	m_height = image->getHeight();

	// generate and bind texture
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// set parameters
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// create the texture using the image data
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image->getData());
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

GLuint
Texture::getId() const
{
	return m_id;
}

unsigned int
Texture::getWidth() const
{
	return m_width;
}

unsigned int
Texture::getHeight() const
{
	return m_height;
}

RefPtr <Texture>
Texture::none()
{
	return RefPtr <Texture> ();
}

RefPtr <Texture>
Texture::create(RefPtr <Image> image)
{
	return RefPtr <Texture> (new Texture(image));
}

} // namespace DromeGL
