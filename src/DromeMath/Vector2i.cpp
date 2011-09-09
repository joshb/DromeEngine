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
#include <DromeMath/Vector2i.h>

using namespace std;

namespace DromeMath {

Vector2i::Vector2i(int x_param, int y_param)
{
	x = x_param;
	y = y_param;
}

Vector2i
Vector2i::operator + (const Vector2i &v) const
{
	return Vector2i(x + v.x, y + v.y);
}

Vector2i
Vector2i::operator + (int i) const
{
	return Vector2i(x + i, y + i);
}

void
Vector2i::operator += (const Vector2i &v)
{
	x += v.x;
	y += v.y;
}

void
Vector2i::operator += (int i)
{
	x += i;
	y += i;
}

Vector2i
Vector2i::operator - (const Vector2i &v) const
{
	return Vector2i(x - v.x, y - v.y);
}

Vector2i
Vector2i::operator - (int i) const
{
	return Vector2i(x - i, y - i);
}

void
Vector2i::operator -= (const Vector2i &v)
{
	x -= v.x;
	y -= v.y;
}

void
Vector2i::operator -= (int i)
{
	x -= i;
	y -= i;
}

Vector2i
Vector2i::operator * (const Vector2i &v) const
{
	return Vector2i(x * v.x, y * v.y);
}

Vector2i
Vector2i::operator * (int i) const
{
	return Vector2i(x * i, y * i);
}

void
Vector2i::operator *= (const Vector2i &v)
{
	x *= v.x;
	y *= v.y;
}

void
Vector2i::operator *= (int i)
{
	x *= i;
	y *= i;
}

Vector2i
Vector2i::operator / (const Vector2i &v) const
{
	return Vector2i(x / v.x, y / v.y);
}

Vector2i
Vector2i::operator / (int i) const
{
	return Vector2i(x / i, y / i);
}

void
Vector2i::operator /= (const Vector2i &v)
{
	x /= v.x;
	y /= v.y;
}

void
Vector2i::operator /= (int i)
{
	x /= i;
	y /= i;
}

string
Vector2i::toString() const
{
	stringstream stream;
	stream << x << " " << y;

	return stream.str();
}

} // namespace DromeMath
