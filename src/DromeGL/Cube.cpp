/*
 * Copyright (C) 2012 Josh A. Beam
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
#include <DromeGL/Cube.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

Cube::Cube(RefPtr <ShaderProgram> program)
{
	m_program = program;

	float v[] = {
		// -x
		-1.0f, +1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, +1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, -1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 1.0f,

		// +x
		+1.0f, +1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 1.0f,

		// -y
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 1.0f,

		// +y
		-1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 1.0f,

		// -z
		+1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		+1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		-1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 1.0f,

		// +z
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 1.0f
	};

	// create VAO and VBO
#ifdef GLES
	glGenVertexArraysOES(1, &m_vaoId);
	glBindVertexArrayOES(m_vaoId);
#else
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
#endif
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

	// set vertex position attribute
	GLint location = m_program->getAttribLocation("vertexPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 14, 0);
    
    // set vertex tangent attribute
	try {
		location = m_program->getAttribLocation("vertexTangent");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 14, (const GLvoid *)(sizeof(float) * 3));
	} catch(Exception) {}
    
    // set vertex bitangent attribute
	try {
		location = m_program->getAttribLocation("vertexBitangent");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 14, (const GLvoid *)(sizeof(float) * 6));
	} catch(Exception) {}

	// set vertex normal attribute
	try {
		location = m_program->getAttribLocation("vertexNormal");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 14, (const GLvoid *)(sizeof(float) * 9));
	} catch(Exception) {}

	// set vertex texture coordinates attribute
	try {
		location = m_program->getAttribLocation("vertexTexCoords");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 14, (const GLvoid *)(sizeof(float) * 12));
	} catch(Exception) {}
}

Cube::~Cube()
{
	// delete VAO and VBO
#ifdef GLES
	glDeleteVertexArraysOES(1, &m_vaoId);
#else
	glDeleteVertexArrays(1, &m_vaoId);
#endif
	glDeleteBuffers(1, &m_vboId);
}

void
Cube::render()
{
#ifdef GLES
	glBindVertexArrayOES(m_vaoId);
#else
	glBindVertexArray(m_vaoId);
#endif
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

} // namespace DromeGL
