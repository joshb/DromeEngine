/*
 * Copyright (C) 2013 Josh A. Beam
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
#include <DromeGL/StaticMesh.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {
    
Vertex::Vertex(const Vector3 &position, const Vector3 &tangent, const Vector3 &bitangent, const Vector3 &normal, float s, float t)
{
    this->position = position;
    this->tangent = tangent;
    this->bitangent = bitangent;
    this->normal = normal;
    this->s = s;
    this->t = t;
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
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
StaticMesh::addVertex(const Vertex &vertex)
{
    m_vertices.push_back(vertex);
}

void
StaticMesh::finalize(RefPtr <ShaderProgram> program)
{
    m_program = program;
    m_numVertices = m_vertices.size();

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_numVertices, m_vertices.data(), GL_STATIC_DRAW);

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

    m_vertices.clear();
}

void
StaticMesh::render()
{
#ifdef GLES
	glBindVertexArrayOES(m_vaoId);
#else
	glBindVertexArray(m_vaoId);
#endif
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_numVertices);
}

} // namespace DromeGL
