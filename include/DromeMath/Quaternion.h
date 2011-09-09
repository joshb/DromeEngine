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

#ifndef __DROMEMATH_QUATERNION_H__
#define __DROMEMATH_QUATERNION_H__

#include "Matrix4.h"
#include "Vector3.h"

namespace DromeMath {

/**
 * \brief Represents a quaternion.
 */
class Quaternion
{
	public:
		/**
		 * \brief The 3-dimensional vector component of the quaternion.
		 */
		Vector3 v;

		/**
		 * \brief The scalar component of the quaternion.
		 */
		float w;

		/**
		 * Creates a new Quaternion object that has given the 3-dimensional vector component and scalar component.
		 *
		 * @param v_param The vector component of the quaternion.
		 * @param w_param The scalar component of the quaternion.
		 */
		Quaternion(const Vector3 &v_param = Vector3(), float w_param = 1.0f)
		{
			v = v_param;
			w = w_param;
		}

		/**
		 * Calculates the normalized version of this quaternion, which is defined as the square root of the sum of the squared components of the quaternion (v.x^2 + v.y^2 + v.z^2 + w^2).
		 *
		 * @return A Quaternion object that is the result of normalizing this quaternion.
		 */
		Quaternion normalize() const;

		/**
		 * Calculates the conjugate version of this quaternion, which is the same quaternion but with the vector component mulitplied by -1.
		 *
		 * @return A Quaternion object that is the result of conjugating this quaternion.
		 */
		Quaternion conjugate() const;

		/**
		 * @return A Matrix4 object that is the result of finding the matrix representation of this quaternion.
		 */
		Matrix4 toMatrix4() const;

		/**
		 * @param q The quaternion to multiply this one by.
		 * @return A Quaternion object that is the result of multiplying this quaternion by another one.
		 */
		Quaternion operator * (const Quaternion &q) const;

		/**
		 * @param f The scalar to multiply this quaternion by.
		 * @return A Quaternion object that is the result of multiplying this quaternion by a scalar.
		 */
		Quaternion operator * (float f) const;

		/**
		 * Multiplies this quaternion by another quaternion.
		 *
		 * @param q The quaternion to multiply this one by.
		 */
		void operator *= (const Quaternion &q) { *this = *this * q; }

		/**
		 * Multiplies this quaternion by a scalar.
		 *
		 * @param f The scalar to multiply this quaternion by.
		 */
		void operator *= (float f) { *this = *this * f; }

		static Quaternion fromAxis(const Vector3 &axis, float theta);
		static Quaternion fromRotation(const Vector3 &rotation);
};

} // namespace DromeMath

#endif /* __DROMECORE_MATH_H__ */
