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
#include <DromeGfx/OpenGL.h>
#include <DromeGfx/VertexBuffer.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

VertexBuffer::VertexBuffer(const float *data, int size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

unsigned int
VertexBuffer::getId() const
{
	return m_id;
}

RefPtr <VertexBuffer>
VertexBuffer::none()
{
	return RefPtr <VertexBuffer> ();
}

RefPtr <VertexBuffer>
VertexBuffer::create(const float *data, int size)
{
	return RefPtr <VertexBuffer> (new VertexBuffer(data, size));
}

RefPtr <VertexBuffer>
VertexBuffer::create(const Vector3 *data, int size)
{
	return RefPtr <VertexBuffer> (new VertexBuffer((const float *)data, size * 3));
}

RefPtr <VertexBuffer>
VertexBuffer::create(const Matrix4 *data, int size)
{
	return RefPtr <VertexBuffer> (new VertexBuffer((const float *)data, size * 16));
}

} // namespace DromeGfx
