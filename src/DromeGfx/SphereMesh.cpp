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
#include <DromeGfx/SphereMesh.h>
#include <DromeMath/Util.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

SphereMesh::SphereMesh(unsigned int divisions, const Vector3 &scale)
{
	if(divisions < 1)
		throw Exception("SphereMesh::SphereMesh(): Invalid number of divisions (%u)");

	vector<Vector3> verts;
	vector<float> texCoords;
	vector<Vector3> tangentSpace;

	unsigned int vertsPerSlice = divisions + 1;

	// calculate vertices
	for(unsigned int i = 0; i <= divisions / 2; ++i) {
		float r = ((M_PI * 2.0f) / (float)divisions) * (float)i;
		float c = cosf(r); // position on z axis
		float s = sinf(r); // radius around z axis

		for(unsigned int j = 0; j < vertsPerSlice; ++j) {
			r = ((M_PI * 2.0f) / (float)divisions) * (float)j;

			verts.push_back(Vector3(cosf(r) * s, sinf(r) * s, c) * scale);

			texCoords.push_back((float)j / (float)divisions);
			texCoords.push_back((float)i / (float)(divisions / 2));

			tangentSpace.push_back(Vector3(cosf(r + M_PI/2.0f) * s, sinf(r + M_PI/2.0f) * s, c).normalize());
			tangentSpace.push_back(Vector3(cosf(r) * sinf(r + M_PI/2.0f), sinf(r) * sinf(r + M_PI/2.0f), cosf(r + M_PI/2.0f)).normalize());
			tangentSpace.push_back(Vector3(cosf(r) * s, sinf(r) * s, c).normalize());
		}
	}

	// create commands
	for(unsigned int i = 1; i <= divisions / 2; ++i) {
		Command *cmd = new Command(vertsPerSlice * 2);
		cmd->type = PRIMITIVE_TYPE_TRIANGLE_STRIP;
		for(unsigned int j = 0; j < vertsPerSlice; ++j) {
			cmd->indices[j * 2 + 0] = vertsPerSlice * (i-1) + j;
			cmd->indices[j * 2 + 1] = vertsPerSlice * i + j;
		}
		m_commands.push_back(cmd);
	}

	// create vertex buffers
	m_vertices = VertexBuffer::create(&(verts[0]), verts.size());
	m_texCoords = VertexBuffer::create(&(texCoords[0]), texCoords.size());
	m_tangentSpace = VertexBuffer::create(&(tangentSpace[0]), tangentSpace.size());
}

RefPtr <SphereMesh>
SphereMesh::create(unsigned int divisions, const Vector3 &scale)
{
	return RefPtr <SphereMesh> (new SphereMesh(divisions, scale));
}

RefPtr <SphereMesh>
SphereMesh::create(unsigned int divisions, float radius)
{
	return RefPtr <SphereMesh> (new SphereMesh(divisions, Vector3(radius, radius, radius)));
}

} // namespace DromeGfx
