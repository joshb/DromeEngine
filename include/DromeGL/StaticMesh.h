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

#ifndef __DROMEGL_STATICMESH_H__
#define __DROMEGL_STATICMESH_H__

#include <vector>
#include "OpenGL.h"
#include "ShaderProgram.h"

namespace DromeGL {

class Vertex
{
    public:
        DromeMath::Vector3 position;
        DromeMath::Vector3 tangent;
        DromeMath::Vector3 bitangent;
        DromeMath::Vector3 normal;
        float s, t;

        Vertex(const DromeMath::Vector3 &position, const DromeMath::Vector3 &tangent, const DromeMath::Vector3 &bitangent, const DromeMath::Vector3 &normal, float s, float t);
};

class StaticMesh
{
	private:
        std::vector <Vertex> m_vertices;
        size_t m_numVertices;

		GLuint m_vaoId;
		GLuint m_vboId;
		DromeCore::RefPtr <ShaderProgram> m_program;

	public:
        StaticMesh();
        virtual ~StaticMesh();

        void addVertex(const Vertex &vertex);
        void finalize(DromeCore::RefPtr <ShaderProgram> program);
        void render();
};

} // namespace DromeGL

#endif /* __DROMEGL_STATICMESH_H__ */
