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

#include <cmath>
#include <sstream>
#include <DromeMath/Matrix4.h>

using namespace std;

namespace DromeMath {

Matrix4::Matrix4()
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			m_matrix[i*4+j] = (i == j) ? 1.0f : 0.0f;
	}
}

Matrix4::Matrix4(const float matrix[16])
{
	for(int i = 0; i < 16; ++i)
		m_matrix[i] = matrix[i];
}

const float *
Matrix4::getData() const
{
	return m_matrix;
}

float &
Matrix4::operator [] (int i)
{
	return m_matrix[i];
}

Matrix4
Matrix4::operator * (float scalar) const
{
	float m[16];

	for(int i = 0; i < 16; ++i)
		m[i] = m_matrix[i] * scalar;

	return Matrix4(m);
}

Matrix4
Matrix4::operator + (const Matrix4 &matrix) const
{
	float m[16];

	for(int i = 0; i < 16; ++i)
		m[i] = m_matrix[i] + matrix.m_matrix[i];

	return Matrix4(m);
}

Matrix4
Matrix4::operator - (const Matrix4 &matrix) const
{
	float m[16];

	for(int i = 0; i < 16; ++i)
		m[i] = m_matrix[i] - matrix.m_matrix[i];

	return Matrix4(m);
}

Matrix4
Matrix4::operator * (const Matrix4 &matrix) const
{
	const float *m1 = m_matrix;
	const float *m2 = matrix.m_matrix;
	float m[16];

	m[ 0] = m1[ 0]*m2[ 0] + m1[ 1]*m2[ 4] + m1[ 2]*m2[ 8] + m1[ 3]*m2[12];
	m[ 1] = m1[ 0]*m2[ 1] + m1[ 1]*m2[ 5] + m1[ 2]*m2[ 9] + m1[ 3]*m2[13];
	m[ 2] = m1[ 0]*m2[ 2] + m1[ 1]*m2[ 6] + m1[ 2]*m2[10] + m1[ 3]*m2[14];
	m[ 3] = m1[ 0]*m2[ 3] + m1[ 1]*m2[ 7] + m1[ 2]*m2[11] + m1[ 3]*m2[15];
	m[ 4] = m1[ 4]*m2[ 0] + m1[ 5]*m2[ 4] + m1[ 6]*m2[ 8] + m1[ 7]*m2[12];
	m[ 5] = m1[ 4]*m2[ 1] + m1[ 5]*m2[ 5] + m1[ 6]*m2[ 9] + m1[ 7]*m2[13];
	m[ 6] = m1[ 4]*m2[ 2] + m1[ 5]*m2[ 6] + m1[ 6]*m2[10] + m1[ 7]*m2[14];
	m[ 7] = m1[ 4]*m2[ 3] + m1[ 5]*m2[ 7] + m1[ 6]*m2[11] + m1[ 7]*m2[15];
	m[ 8] = m1[ 8]*m2[ 0] + m1[ 9]*m2[ 4] + m1[10]*m2[ 8] + m1[11]*m2[12];
	m[ 9] = m1[ 8]*m2[ 1] + m1[ 9]*m2[ 5] + m1[10]*m2[ 9] + m1[11]*m2[13];
	m[10] = m1[ 8]*m2[ 2] + m1[ 9]*m2[ 6] + m1[10]*m2[10] + m1[11]*m2[14];
	m[11] = m1[ 8]*m2[ 3] + m1[ 9]*m2[ 7] + m1[10]*m2[11] + m1[11]*m2[15];
	m[12] = m1[12]*m2[ 0] + m1[13]*m2[ 4] + m1[14]*m2[ 8] + m1[15]*m2[12];
	m[13] = m1[12]*m2[ 1] + m1[13]*m2[ 5] + m1[14]*m2[ 9] + m1[15]*m2[13];
	m[14] = m1[12]*m2[ 2] + m1[13]*m2[ 6] + m1[14]*m2[10] + m1[15]*m2[14];
	m[15] = m1[12]*m2[ 3] + m1[13]*m2[ 7] + m1[14]*m2[11] + m1[15]*m2[15];

	return Matrix4(m);
}

Vector3
Matrix4::operator * (const Vector3 &vector) const
{
	float x = m_matrix[0]*vector.x + m_matrix[1]*vector.y + m_matrix[2]*vector.z + m_matrix[3];
	float y = m_matrix[4]*vector.x + m_matrix[5]*vector.y + m_matrix[6]*vector.z + m_matrix[7];
	float z = m_matrix[8]*vector.x + m_matrix[9]*vector.y + m_matrix[10]*vector.z + m_matrix[11];

	return Vector3(x, y, z);
}

Matrix4
Matrix4::transpose() const
{
	float m[16];

	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			m[i*4+j] = m_matrix[j*4+i];
	}

	return Matrix4(m);
}

Matrix4
Matrix4::translate(const Vector3 &vector) const
{
	Matrix4 m = *this;

	m.m_matrix[12] += vector.x;
	m.m_matrix[13] += vector.y;
	m.m_matrix[14] += vector.z;

	return m;
}

Matrix4
Matrix4::removeTranslation() const
{
	Matrix4 m = *this;

	m.m_matrix[12] = m.m_matrix[13] = m.m_matrix[14] = 0.0f;
	m.m_matrix[15] = 1.0f;

	return m;
}

string
Matrix4::toString() const
{
	stringstream stream;

	for(int i = 0; i < 4; ++i) {
		if(i != 0)
			stream << '\n';
		for(int j = 0; j < 4; ++j) {
			if(j != 0)
				stream << ' ';
			stream << m_matrix[i*4 + j];
		}
	}

	return stream.str();
}

Matrix4
Matrix4::perspective(float fov, float aspect, float znear, float zfar)
{
	Matrix4 m;
	float f = 1.0f / tan(fov / 2.0f);

	m[0] = f / aspect;
	m[5] = f;
	m[10] = (zfar + znear) / (znear - zfar);
	m[11] = -1.0f;
	m[14] = (2.0f * zfar * znear) / (znear - zfar);
	m[15] = 0.0f;

	return m;
}

} // namespace DromeMath
