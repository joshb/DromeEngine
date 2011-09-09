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

#ifndef __DROMEMATH_VECTOR2I_H__
#define __DROMEMATH_VECTOR2I_H__

#include <string>

namespace DromeMath {

/**
 * \brief Represents a two-dimensional integer vector.
 */
class Vector2i
{
	public:
		/**
		 * \brief The X component of the two-dimensional vector.
		 */
		int x;

		/**
		 * \brief The Y component of the two-dimensional vector.
		 */
		int y;

		/**
		 * Creates a new Vector2i object representing the vector with the given coordinates.
		 *
		 * @param x_param The X coordinate of the vector.
		 * @param y_param The Y coordinate of the vector.
		 */
		Vector2i(int x_param = 0, int y_param = 0);

		/**
		 * @param v The vector to add to this vector.
		 * @return A Vector2i object that is the result of adding each component of the given vector to the corresponding components of this vector.
		 */
		Vector2i operator + (const Vector2i &v) const;

		/**
		 * @param v The scalar to add to this vector.
		 * @return A Vector2i object that is the result of adding the given scalar to each component of this vector.
		 */
		Vector2i operator + (int i) const;

		/**
		 * Adds each component of the given vector to the corresponding components of this vector.
		 *
		 * @param v The vector to add to this vector.
		 */
		void operator += (const Vector2i &v);

		/**
		 * Adds the given scalar to each component of this vector.
		 *
		 * @param v The scalar to add to this vector.
		 */
		void operator += (int i);

		/**
		 * @param v The vector to subtract from this vector.
		 * @return A Vector2i object that is the result of subtracting each component of the given vector from the corresponding components of this vector.
		 */
		Vector2i operator - (const Vector2i &v) const;

		/**
		 * @param v The scalar to subtract from this vector.
		 * @return A Vector2i object that is the result of subtracting the given scalar from each component of this vector.
		 */
		Vector2i operator - (int i) const;

		/**
		 * Subtracts each component of the given vector from the corresponding components of this vector.
		 *
		 * @param v The vector to subtract from this vector.
		 */
		void operator -= (const Vector2i &v);

		/**
		 * Subtracts the given scalar from each component of this vector.
		 *
		 * @param v The scalar to subtract from this vector.
		 */
		void operator -= (int i);

		/**
		 * @param v The vector to multiply this vector by.
		 * @return A Vector2i object that is the result of multiplying each component of the given vector by the corresponding components of this vector.
		 */
		Vector2i operator * (const Vector2i &v) const;

		/**
		 * @param i The scalar to multiply this vector by.
		 * @return A Vector2i object that is the result of multiplying each component of this vector by the given scalar.
		 */
		Vector2i operator * (int i) const;

		/**
		 * Multiplies each component of this vector by the corresponding components of the given vector.
		 *
		 * @param v The vector to multiply this vector by.
		 */
		void operator *= (const Vector2i &v);

		/**
		 * Multiplies each component of this vector by the given scalar.
		 *
		 * @param i The scalar to multiply this vector by.
		 */
		void operator *= (int i);

		/**
		 * @param v The vector to divide this vector by.
		 * @return A Vector2i object that is the result of dividing each component of this vector by the corresponding components of the given vector.
		 */
		Vector2i operator / (const Vector2i &v) const;

		/**
		 * @param i The scalar to divide this vector by.
		 * @return A Vector2i object that is the result of dividing each component of this vector by the given scalar.
		 */
		Vector2i operator / (int i) const;

		/**
		 * Divides each component of this vector by the corresponding components of the given vector.
		 *
		 * @param v The vector to divide this vector by.
		 */
		void operator /= (const Vector2i &v);

		/**
		 * Divides each component of this vector by the given scalar.
		 *
		 * @param i The scalar to divide this vector by.
		 */
		void operator /= (int i);

		/**
		 * @return A string representation of the vector.
		 */
		std::string toString() const;
};

} // namespace DromeMath

#endif /* __DROMEMATH_VECTOR2I_H__ */
