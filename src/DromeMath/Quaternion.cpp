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

#include <DromeMath/Quaternion.h>
#include <DromeMath/Util.h>

namespace DromeMath {

Quaternion
Quaternion::normalize() const
{
	float f = 1.0f / sqrt(v.dotProduct(v) + w*w);

	return Quaternion(v * f, w * f);
}

Quaternion
Quaternion::conjugate() const
{
	return Quaternion(v * -1.0f, w);
}

Matrix4
Quaternion::toMatrix4() const
{
	Matrix4 tmp;

	tmp[0] = 1.0f - (2.0f * v.y*v.y) - (2.0f * v.z*v.z);
	tmp[1] = (2.0f * v.x * v.y) + (2.0f * v.z * w);
	tmp[2] = (2.0f * v.x * v.z) - (2.0f * v.y * w);
	tmp[3] = 0.0f;

	tmp[4] = (2.0f * v.x * v.y) - (2.0f * v.z * w);
	tmp[5] = 1.0f - (2.0f * v.x*v.x) - (2.0f * v.z*v.z);
	tmp[6] = (2.0f * v.y * v.z) + (2.0f * v.x * w);
	tmp[7] = 0.0f;

	tmp[8] = (2.0f * v.x * v.z) + (2.0f * v.y * w);
	tmp[9] = (2.0f * v.y * v.z) - (2.0f * v.x * w);
	tmp[10] = 1.0f - (2.0f * v.x*v.x) - (2.0f * v.y*v.y);
	tmp[11] = 0.0f;

	tmp[12] = 0.0f;
	tmp[13] = 0.0f;
	tmp[14] = 0.0f;
	tmp[15] = 1.0f;

	return tmp.transpose();
}

Quaternion
Quaternion::operator * (const Quaternion &q) const
{
	return Quaternion((q.v * w) + (v * q.w) + v.crossProduct(q.v),
	                  (w * q.w) - v.dotProduct(q.v));
}

Quaternion
Quaternion::operator * (float f) const
{
	return Quaternion(v * f, w * f);
}

Quaternion
Quaternion::fromAxis(const Vector3 &axis, float theta)
{
	theta /= 2.0f;
	float s = sin(theta);

	return Quaternion(axis * s, cos(theta));
}

Quaternion
Quaternion::fromRotation(const Vector3 &rotation)
{
	Quaternion q1 = fromAxis(Vector3(1.0f, 0.0f, 0.0f), degToRad(rotation.x));
	Quaternion q2 = fromAxis(Vector3(0.0f, 1.0f, 0.0f), degToRad(rotation.y));
	Quaternion q3 = fromAxis(Vector3(0.0f, 0.0f, 1.0f), degToRad(rotation.z));

	return (q1 * q2) * q3;
}

} // namespace DromeMath
