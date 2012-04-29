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

#include <sstream>
#include <DromeMath/Vector3.h>
#include <DromeMath/Util.h>

using namespace std;

namespace DromeMath {

Vector3::Vector3(float x_param, float y_param, float z_param)
{
	x = x_param;
	y = y_param;
	z = z_param;
}

Vector3::Vector3(const char *s)
{
	stringstream stream(s);
	stream >> x >> y >> z;
}

Vector3::Vector3(const string &s)
{
	stringstream stream(s);
	stream >> x >> y >> z;
}

float &
Vector3::operator [] (int i)
{
	switch(i) {
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
	}
}

float
Vector3::operator [] (int i) const
{
	switch(i) {
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
	}
}

float
Vector3::dotProduct(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

float
Vector3::length() const
{
	return sqrtf(dotProduct(*this));
}

Vector3
Vector3::crossProduct(const Vector3 &v) const
{
	return Vector3(y * v.z - z * v.y,
	               z * v.x - x * v.z,
	               x * v.y - y * v.x);
}

Vector3
Vector3::normalize() const
{
	float scale = 1.0f / sqrtf(x*x + y*y + z*z);
	return Vector3(x * scale, y * scale, z * scale);
}

Vector3
Vector3::operator + (const Vector3 &v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3
Vector3::operator + (float f) const
{
	return Vector3(x + f, y + f, z + f);
}

void
Vector3::operator += (const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void
Vector3::operator += (float f)
{
	x += f;
	y += f;
	z += f;
}

Vector3
Vector3::operator - (const Vector3 &v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3
Vector3::operator - (float f) const
{
	return Vector3(x - f, y - f, z - f);
}

void
Vector3::operator -= (const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void
Vector3::operator -= (float f)
{
	x -= f;
	y -= f;
	z -= f;
}

Vector3
Vector3::operator * (const Vector3 &v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3
Vector3::operator * (float f) const
{
	return Vector3(x * f, y * f, z * f);
}

void
Vector3::operator *= (const Vector3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void
Vector3::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
}

Vector3
Vector3::operator / (float f) const
{
	return Vector3(x / f, y / f, z / f);
}

void
Vector3::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
}

string
Vector3::toString() const
{
	stringstream stream;
	stream << x << " " << y << " " << z;

	return stream.str();
}

} // namespace DromeMath
