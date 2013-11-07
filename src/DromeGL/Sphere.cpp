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
#include <DromeCore/Exception.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

Sphere::Sphere(RefPtr <ShaderProgram> program, int numDivisions)
{
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

            Vertex vertex;
            vertex.position = Vector3(c3 * s1, c1, s3 * s1);
            vertex.tangent = Vector3(cosf(theta3 + M_PI / 2.0f), 0.0f, -sinf(theta3 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta1 + M_PI / 2.0f), -sinf(theta1 + M_PI / 2.0f));
            vertex.normal = vertex.position;
            vertex.s = (float)j / (float)numDivisions;
            vertex.t = (float)i / (float)(numDivisions / 2);
            addVertex(vertex);
            vertex.position = Vector3(c3 * s2, c2, s3 * s2);
            vertex.tangent = Vector3(cosf(theta3 + M_PI / 2.0f), 0.0f, -sinf(theta3 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta2 + M_PI / 2.0f), -sinf(theta2 + M_PI / 2.0f));
            vertex.normal = vertex.position;
            vertex.s = (float)j / (float)numDivisions;
            vertex.t = (float)(i+1) / (float)(numDivisions / 2);
            addVertex(vertex);
            vertex.position = Vector3(c4 * s1, c1, s4 * s1);
            vertex.tangent = Vector3(cosf(theta4 + M_PI / 2.0f), 0.0f, -sinf(theta4 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta1 + M_PI / 2.0f), -sinf(theta1 + M_PI / 2.0f));
            vertex.normal = vertex.position;
            vertex.s = (float)(j+1) / (float)numDivisions;
            vertex.t = (float)i / (float)(numDivisions / 2);
            addVertex(vertex);

            vertex.position = Vector3(c4 * s1, c1, s4 * s1);
            vertex.normal = vertex.position;
            vertex.tangent = Vector3(cosf(theta4 + M_PI / 2.0f), 0.0f, -sinf(theta4 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta1 + M_PI / 2.0f), -sinf(theta1 + M_PI / 2.0f));
            vertex.s = (float)(j+1) / (float)numDivisions;
            vertex.t = (float)i / (float)(numDivisions / 2);
            addVertex(vertex);
            vertex.position = Vector3(c3 * s2, c2, s3 * s2);
            vertex.tangent = Vector3(cosf(theta3 + M_PI / 2.0f), 0.0f, -sinf(theta3 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta2 + M_PI / 2.0f), -sinf(theta2 + M_PI / 2.0f));
            vertex.normal = vertex.position;
            vertex.s = (float)j / (float)numDivisions;
            vertex.t = (float)(i+1) / (float)(numDivisions / 2);
            addVertex(vertex);
            vertex.position = Vector3(c4 * s2, c2, s4 * s2);
            vertex.tangent = Vector3(cosf(theta4 + M_PI / 2.0f), 0.0f, -sinf(theta4 + M_PI / 2.0f));
            vertex.bitangent = Vector3(0.0f, cosf(theta2 + M_PI / 2.0f), -sinf(theta2 + M_PI / 2.0f));
            vertex.normal = vertex.position;
            vertex.s = (float)(j+1) / (float)numDivisions;
            vertex.t = (float)(i+1) / (float)(numDivisions / 2);
            addVertex(vertex);
		}
	}

    finalize(program);
}

} // namespace DromeGL
