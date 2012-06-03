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

#include <vector>
#include <DromeMath/DromeMath>
#include <DromeGL/Sphere.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

const int numDivisions = 12;

struct Vertex
{
	float x, y, z;
};

Sphere::Sphere(RefPtr <ShaderProgram> program)
{
	m_program = program;

	vector <Vertex> vertices;
	for(int i = 0; i < (numDivisions / 2); ++i) {
		float theta1 = (float)M_PI * (float)i / (float)(numDivisions / 2);
		float c1 = cosf(theta1);
		float s1 = sinf(theta1);
		float theta2 = (float)M_PI * (float)(i+1) / (float)(numDivisions / 2);
		float c2 = cosf(theta2);
		float s2 = sinf(theta2);

		for(int j = 0; j < numDivisions; ++j) {
			float theta3 = (float)M_PI * 2.0f * (float)j / (float)numDivisions;
			float c3 = cosf(theta3);
			float s3 = -sinf(theta3);
			float theta4 = (float)M_PI * 2.0f * (float)(j+1) / (float)numDivisions;
			float c4 = cosf(theta4);
			float s4 = -sinf(theta4);

			Vertex vert;
			vert.x = c3 * s1; vert.y = c1; vert.z = s3 * s1;
			vertices.push_back(vert);
			vert.x = c3 * s2; vert.y = c2; vert.z = s3 * s2;
			vertices.push_back(vert);
			vert.x = c4 * s1; vert.y = c1; vert.z = s4 * s1;
			vertices.push_back(vert);

			vert.x = c4 * s1; vert.y = c1; vert.z = s4 * s1;
			vertices.push_back(vert);
			vert.x = c3 * s2; vert.y = c2; vert.z = s3 * s2;
			vertices.push_back(vert);
			vert.x = c4 * s2; vert.y = c2; vert.z = s4 * s2;
			vertices.push_back(vert);
		}
	}

	m_numVertices = (GLsizei)vertices.size();
	float *v = new float[m_numVertices * 3];
	for(unsigned int i = 0; i < vertices.size(); ++i) {
		v[i*3+0] = vertices[i].x;
		v[i*3+1] = vertices[i].y;
		v[i*3+2] = vertices[i].z;
	}

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, v, GL_STATIC_DRAW);
	delete [] v;

	// set vertex position attribute
	GLint location = m_program->getAttribLocation("vertexPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// set vertex normal attribute
	location = m_program->getAttribLocation("vertexNormal");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Sphere::~Sphere()
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
Sphere::render()
{
#ifdef GLES
	glBindVertexArrayOES(m_vaoId);
#else
	glBindVertexArray(m_vaoId);
#endif
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}

} // namespace DromeGL
