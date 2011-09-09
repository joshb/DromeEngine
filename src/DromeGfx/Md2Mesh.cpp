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

#include <cstdio>
#include <DromeCore/Endian.h>
#include <DromeCore/Exception.h>
#include <DromeGfx/Md2Mesh.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

struct Md2Header
{
	int32_t magic;
	int32_t version;
	int32_t skinWidth;
	int32_t skinHeight;
	int32_t frameSize;
	int32_t numSkins;
	int32_t numVertices;
	int32_t numTexCoords;
	int32_t numTriangles;
	int32_t numGlCommands;
	int32_t numFrames;
	int32_t offsetSkins;
	int32_t offsetTexCoords;
	int32_t offsetTriangles;
	int32_t offsetFrames;
	int32_t offsetGlCommands;
	int32_t offsetEnd;
};

struct Md2TriangleVertexByte
{
	uint8_t vertex[3];
	uint8_t lightNormalIndex;
};

struct Md2Frame
{
	float scale[3];
	float translate[3];
	int8_t name[16];
};

struct Md2GlCommandVertex
{
	float s, t;
	int32_t vertexIndex;
};

namespace DromeGfx {

Md2Mesh::Md2Mesh(const char *filePath, const Vector3 &scale)
{
	// open file
	FILE *fp = fopen(filePath, "rb");
	if(!fp)
		throw Exception(string("Md2Mesh::Md2Mesh(): Couldn't open '") + filePath + string("' for reading"));

	// read md2 header
	Md2Header hdr;
	fread(&hdr, sizeof(hdr), 1, fp);

	// endian conversion
	for(unsigned int i = 0; i < (sizeof(hdr) / sizeof(int32_t)); ++i) {
		int32_t *p = (int32_t *)&hdr + i;
		*p = littleToNativeInt32(*p);
	}

	// load vertices
	Vector3 *verts = new Vector3[hdr.numVertices];
	fseek(fp, hdr.offsetFrames, SEEK_SET);
	//for(int32_t i = 0; i < hdr.numFrames; ++i) {
	for(int32_t i = 0; i < 1; ++i) {
		Md2Frame frame;
		fread(&frame, sizeof(frame), 1, fp);

		// endian conversion
		for(int j = 0; j < 3; ++j) {
			frame.scale[j] = littleToNativeFloat(frame.scale[j]);
			frame.translate[j] = littleToNativeFloat(frame.translate[j]);
		}

		// read vertices
		for(int32_t j = 0; j < hdr.numVertices; ++j) {
			Md2TriangleVertexByte vb;
			fread(&vb, sizeof(vb), 1, fp);

			verts[j].x = (float)vb.vertex[0] * frame.scale[0] + frame.translate[0];
			verts[j].y = (float)vb.vertex[1] * frame.scale[1] + frame.translate[1];
			verts[j].z = (float)vb.vertex[2] * frame.scale[2] + frame.translate[2];
			verts[j] *= scale;
		}

		m_vertices = VertexBuffer::create(verts, hdr.numVertices);
	}
	delete [] verts;

	// load glcommands
	float *texCoords = new float[hdr.numVertices * 2];
	fseek(fp, hdr.offsetGlCommands, SEEK_SET);
	for(int32_t i = 0; i < hdr.numGlCommands; ++i) {
		// get number of indices
		int32_t num;
		fread(&num, sizeof(num), 1, fp);
		num = littleToNativeInt32(num);

		// create the command; if the number of indices
		// from the file is negative, the command is a
		// triangle fan, otherwise it's a triangle strip
		Command *cmd;
		if(num < 0) {
			num = -num;
			cmd = new Command(num);
			cmd->type = PRIMITIVE_TYPE_TRIANGLE_FAN;
		} else {
			cmd = new Command(num);
			cmd->type = PRIMITIVE_TYPE_TRIANGLE_STRIP;
		}

		// read glcommand vertices
		Md2GlCommandVertex glcv;
		for(int32_t j = 0; j < num; ++j) {
			fread(&glcv, sizeof(glcv), 1, fp);

			// endian conversion
			glcv.s = littleToNativeFloat(glcv.s);
			glcv.t = littleToNativeFloat(glcv.t);
			glcv.vertexIndex = littleToNativeInt32(glcv.vertexIndex);

			if(glcv.vertexIndex < 0 || glcv.vertexIndex >= hdr.numVertices) {
				delete cmd;
				delete [] texCoords;
				fclose(fp);
				throw Exception("Md2Mesh::Md2Mesh(): Md2GlCommandVertex has invalid vertex index");
			}

			// save texture coordinates and vertex index
			texCoords[glcv.vertexIndex * 2 + 0] = glcv.s;
			texCoords[glcv.vertexIndex * 2 + 1] = glcv.t;
			cmd->indices[j] = glcv.vertexIndex;
		}

		cmd->texCoords = VertexBuffer::create(texCoords, hdr.numVertices * 2);
		m_commands.push_back(cmd);
	}
	delete [] texCoords;

	fclose(fp);
}

RefPtr <Md2Mesh>
Md2Mesh::create(const char *filePath, const Vector3 &scale)
{
	return RefPtr <Md2Mesh> (new Md2Mesh(filePath, scale));
}

RefPtr <Md2Mesh>
Md2Mesh::create(const char *filePath, float scale)
{
	return RefPtr <Md2Mesh> (new Md2Mesh(filePath, Vector3(scale, scale, scale)));
}

} // namespace DromeGfx
