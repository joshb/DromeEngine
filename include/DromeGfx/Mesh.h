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

#ifndef __DROMEGFX_MESH_H__
#define __DROMEGFX_MESH_H__

#include <vector>
#include <DromeCore/Ref.h>
#include <DromeMath/Vector3.h>
#include "Types.h"
#include "VertexBuffer.h"

namespace DromeGfx {

class Mesh : public DromeCore::RefClass
{
	protected:
		class Command
		{
			public:
				PrimitiveType type;
				DromeCore::RefPtr <VertexBuffer> texCoords;
				unsigned short *indices;
				unsigned int numIndices;

				Command();
				Command(unsigned int numIndicesParam);
				virtual ~Command();
		};

		DromeMath::Vector3 *m_vertexData;
		DromeMath::Vector3 *m_tangentSpaceData;

		DromeCore::RefPtr <VertexBuffer> m_vertices;
		DromeCore::RefPtr <VertexBuffer> m_texCoords;
		DromeCore::RefPtr <VertexBuffer> m_tangentSpace;
		unsigned int m_numVertices;

		std::vector <Command *> m_commands;

		Mesh();
		virtual ~Mesh();

	public:
		void render();
};

} // namespace DromeGfx

#endif /* __DROMEGFX_MESH_H__ */
