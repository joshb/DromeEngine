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

#include <DromeCore/Exception.h>
#include <DromeGL/Cube.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

Cube::Cube(RefPtr <ShaderProgram> program)
{
	float v[] = {
		// -x
		-1.0f, +1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, +1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f, -1.0f, +1.0f,	0.0f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 1.0f,

		// +x
		+1.0f, +1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,  +1.0f, 0.0f, 0.0f,	1.0f, 1.0f,

		// -y
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 1.0f,

		// +y
		-1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, -1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, +1.0f, 0.0f,	1.0f, 1.0f,

		// -z
		+1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		+1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		-1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, +1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		+1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, -1.0f,	1.0f, 1.0f,

		// +z
		-1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 1.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 0.0f,
		+1.0f, +1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	0.0f, 1.0f,
		+1.0f, -1.0f, +1.0f,	1.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, +1.0f,	1.0f, 1.0f
	};
    
    for(int i = 0; i < sizeof(v) / sizeof(float); i += 14) {
        addVertex(Vertex(Vector3(v[i+0], v[i+1], v[i+2]), Vector3(v[i+3], v[i+4], v[i+5]), Vector3(v[i+6], v[i+7], v[i+8]), Vector3(v[i+9], v[i+10], v[i+11]), v[i+12], v[i+13]));
    }

    finalize(program);
}

} // namespace DromeGL
