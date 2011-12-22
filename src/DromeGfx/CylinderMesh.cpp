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
#include <DromeGfx/CylinderMesh.h>
#include <DromeMath/Util.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

CylinderMesh::CylinderMesh(unsigned int divisions)
{
	if(divisions < 1)
		throw Exception("CylinderMesh::CylinderMesh(): Invalid number of divisions (%u)");

	vector<Vector3> verts;
	vector<float> texCoords;
	vector<Vector3> tangentSpace;

	// calculate vertices and texture coordinates
	for(unsigned int i = 0; i <= divisions; ++i) {
		float r = (360.0f / (float)divisions) * (float)i;
		Vector3 tangent = Vector3(cosf(degToRad(r+90.0f)), sinf(degToRad(r+90.0f)), 0.0f);
		Vector3 bitangent = Vector3(0.0f, 0.0f, -1.0f);
		Vector3 normal = Vector3(cosf(degToRad(r)), sinf(degToRad(r)), 0.0f);

		verts.push_back(Vector3(normal.x, normal.y, 1.0f));
		texCoords.push_back((float)i / (float)divisions);
		texCoords.push_back(0.0f);
		tangentSpace.push_back(tangent);
		tangentSpace.push_back(bitangent);
		tangentSpace.push_back(normal);

		verts.push_back(Vector3(normal.x, normal.y, -1.0f));
		texCoords.push_back((float)i / (float)divisions);
		texCoords.push_back(1.0f);
		tangentSpace.push_back(tangent);
		tangentSpace.push_back(bitangent);
		tangentSpace.push_back(normal);
	}

	// create command for rendering
	Command *cmd = new Command((unsigned int)verts.size());
	for(unsigned int i = 0; i < cmd->numIndices; i++)
		cmd->indices[i] = i;
	m_commands.insert(m_commands.end(), cmd);

	// create vertex buffers
	m_vertices = VertexBuffer::create(&(verts[0]), (unsigned int)verts.size());
	m_texCoords = VertexBuffer::create(&(texCoords[0]), (unsigned int)texCoords.size());
	m_tangentSpace = VertexBuffer::create(&(tangentSpace[0]), (unsigned int)tangentSpace.size());
}

RefPtr <CylinderMesh>
CylinderMesh::create(unsigned int divisions)
{
	return RefPtr <CylinderMesh> (new CylinderMesh(divisions));
}

} // namespace DromeGfx
