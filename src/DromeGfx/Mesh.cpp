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
#include <DromeGfx/Mesh.h>
#include <DromeGfx/OpenGL.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

/*
 * Mesh::Command
 */
Mesh::Command::Command()
{
	type = PRIMITIVE_TYPE_TRIANGLE_STRIP;
	indices = NULL;
	numIndices = 0;
}

Mesh::Command::Command(unsigned int numIndicesParam)
{
	type = PRIMITIVE_TYPE_TRIANGLE_STRIP;
	indices = new unsigned short [numIndicesParam];
	numIndices = numIndicesParam;
}

Mesh::Command::~Command()
{
	if(indices)
		delete [] indices;
}

/*
 * Mesh
 */
Mesh::Mesh()
{
	m_vertexData = NULL;
	m_tangentSpaceData = NULL;

	m_numVertices = 0;
}

Mesh::~Mesh()
{
	// delete vertex/tangent space data
	if(m_vertexData)
		delete [] m_vertexData;
	if(m_tangentSpaceData)
		delete [] m_tangentSpaceData;

	// delete commands
	for(unsigned int i = 0; i < m_commands.size(); i++)
		delete m_commands[i];
}

void
Mesh::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices->getId());
	glVertexPointer(3, GL_FLOAT, 0, 0);

	// use tangent space if available
	if(m_tangentSpace.isSet()) {
		// use texture unit 1 for tangents
		glClientActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentSpace->getId());
		glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 9, 0);

		// use texture unit 2 for binormals
		glClientActiveTexture(GL_TEXTURE2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentSpace->getId());
		glTexCoordPointer(3, GL_FLOAT, sizeof(float) * 9, (void *)(sizeof(float) * 3));

		glClientActiveTexture(GL_TEXTURE0);

		// normals
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentSpace->getId());
		glNormalPointer(GL_FLOAT, sizeof(float) * 9, (void *)(sizeof(float) * 6));
	}

	// render each command
	RefPtr <VertexBuffer> lastTexCoords;
	for(unsigned int i = 0; i < m_commands.size(); i++) {
		RefPtr <VertexBuffer> texCoords = m_commands[i]->texCoords.isSet() ? m_commands[i]->texCoords : m_texCoords;

		// use texture coordinates if available
		if(texCoords != lastTexCoords) {
			if(texCoords.isSet()) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, texCoords->getId());
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
			} else {
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}

		lastTexCoords = texCoords;

		glDrawElements(primitiveTypeToGL(m_commands[i]->type), m_commands[i]->numIndices, GL_UNSIGNED_SHORT, m_commands[i]->indices);
	}

	// disable vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	// disable the arrays used for the tangent space matrices
	if(m_tangentSpace.isSet()) {
		glNormalPointer(GL_FLOAT, 0, 0);
		glDisableClientState(GL_NORMAL_ARRAY);

		glClientActiveTexture(GL_TEXTURE2);
		glTexCoordPointer(3, GL_FLOAT, 0, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer(3, GL_FLOAT, 0, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTexture(GL_TEXTURE0);
	}

	// disable texcoord array
	if(lastTexCoords.isSet()) {
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

} // namespace DromeGfx
