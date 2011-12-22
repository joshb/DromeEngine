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
#include <DromeGfx/CubeMesh.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

CubeMesh::CubeMesh(const Vector3 &scale, float sScale, float tScale)
{
	vector<Vector3> verts;
	vector<float> texCoords;
	vector<Vector3> tangentSpace;

	// create vertices for front (+y)
	verts.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	verts.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));

	// create vertices for back (-y)
	verts.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	verts.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	verts.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));

	// create vertices for left (-x)
	verts.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.y * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.y * sScale);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(-1.0f, 0.0f, 0.0f));

	// create vertices for right (+x)
	verts.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.y * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	verts.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.y * sScale);
	texCoords.push_back(1.0f * scale.z * tScale);
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));

	// create vertices for top (+z)
	verts.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, 1.0f));
	verts.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.y * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, 1.0f));
	verts.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, 1.0f));
	verts.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(1.0f * scale.y * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, 1.0f));

	// create vertices for bottom (-z)
	verts.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	verts.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f * scale.y * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	verts.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(0.0f);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));
	verts.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	texCoords.push_back(1.0f * scale.x * sScale);
	texCoords.push_back(1.0f * scale.y * tScale);
	tangentSpace.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, -1.0f, 0.0f));
	tangentSpace.push_back(Vector3(0.0f, 0.0f, -1.0f));

	// create commands
	for(unsigned int i = 0; i < 6; ++i) {
		Command *cmd = new Command(4);
		cmd->type = PRIMITIVE_TYPE_TRIANGLE_STRIP;
		for(unsigned int j = 0; j < 4; ++j)
			cmd->indices[j] = i * 4 + j;
		m_commands.push_back(cmd);
	}

	// create vertex buffers
	m_vertices = VertexBuffer::create(&(verts[0]), (unsigned int)verts.size());
	m_texCoords = VertexBuffer::create(&(texCoords[0]), (unsigned int)texCoords.size());
	m_tangentSpace = VertexBuffer::create(&(tangentSpace[0]), (unsigned int)tangentSpace.size());
}

RefPtr <CubeMesh>
CubeMesh::create(const Vector3 &scale, float sScale, float tScale)
{
	return RefPtr <CubeMesh> (new CubeMesh(scale, sScale, tScale));
}

RefPtr <CubeMesh>
CubeMesh::create(float sideLength, float sScale, float tScale)
{
	return RefPtr <CubeMesh> (new CubeMesh(Vector3(sideLength, sideLength, sideLength), sScale, tScale));
}

} // namespace DromeGfx
