/*
 * Copyright (C) 2010-2012 Josh A. Beam
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

#include <DromeMath/Vector4.h>
#include <DromeMath/Util.h>
#include <sstream>

using namespace std;

namespace DromeMath {

Vector4::Vector4(float x_param, float y_param, float z_param, float w_param)
{
	x = x_param;
	y = y_param;
	z = z_param;
	w = w_param;
}

Vector4::Vector4(const Vector3 &v, float w_param)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = w_param;
}

Vector4::Vector4(const char *s)
{
	stringstream stream(s);
	stream >> x >> y >> z >> w;
}

Vector4::Vector4(const string &s)
{
	stringstream stream(s);
	stream >> x >> y >> z >> w;
}

float &
Vector4::operator [] (int i)
{
	switch(i) {
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
	}
}

float
Vector4::operator [] (int i) const
{
	switch(i) {
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
	}
}

Vector3
Vector4::xyz() const
{
	return Vector3(x, y, z);
}

Color3
Vector4::rgb() const
{
	return Color3(r, g, b);
}

float
Vector4::dotProduct(const Vector4 &v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

float
Vector4::length() const
{
	return sqrtf(dotProduct(*this));
}

Vector4
Vector4::normalize() const
{
	float scale = 1.0f / sqrtf(x*x + y*y + z*z + w*w);
	return Vector4(x * scale, y * scale, z * scale, w * scale);
}

Vector4
Vector4::operator + (const Vector4 &v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4
Vector4::operator + (float f) const
{
	return Vector4(x + f, y + f, z + f, w + f);
}

void
Vector4::operator += (const Vector4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void
Vector4::operator += (float f)
{
	x += f;
	y += f;
	z += f;
	w += f;
}

Vector4
Vector4::operator - (const Vector4 &v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4
Vector4::operator - (float f) const
{
	return Vector4(x - f, y - f, z - f, w - f);
}

void
Vector4::operator -= (const Vector4 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void
Vector4::operator -= (float f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
}

Vector4
Vector4::operator * (const Vector4 &v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4
Vector4::operator * (float f) const
{
	return Vector4(x * f, y * f, z * f, w * f);
}

void
Vector4::operator *= (const Vector4 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void
Vector4::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
}

Vector4
Vector4::operator / (float f) const
{
	return Vector4(x / f, y / f, z / f, w / f);
}

void
Vector4::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
}

string
Vector4::toString() const
{
	stringstream stream;
	stream << x << " " << y << " " << z << " " << w;

	return stream.str();
}

} // namespace DromeMath
