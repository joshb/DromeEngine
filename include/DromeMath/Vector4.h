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

#ifndef __DROMEMATH_VECTOR4_H__
#define __DROMEMATH_VECTOR4_H__

#include <string>
#include "Vector3.h"

namespace DromeMath {

/**
 * \brief Represents a four-dimensional vector or four-component color.
 */
class Vector4
{
	public:
		union {
			struct {
				/**
				 * \brief The X component of the vector.
				 */
				float x;

				/**
				 * \brief The Y component of the vector.
				 */
				float y;

				/**
				 * \brief The Z component of the vector.
				 */
				float z;

				/**
				 * \brief The W component of the vector.
				 */
				float w;
			};

			struct {
				/**
				 * \brief The red component of the color.
				 */
				float r;

				/**
				 * \brief The green component of the color.
				 */
				float g;

				/**
				 * \brief The blue component of the color.
				 */
				float b;

				/**
				 * \brief The alpha component of the color.
				 */
				float a;
			};
		};
		
		/**
		 * Creates a new Vector4 object representing the vector with the given coordinates.
		 *
		 * @param x_param The X coordinate of the vector.
		 * @param y_param The Y coordinate of the vector.
		 * @param z_param The Z coordinate of the vector.
		 * @param w_param The W coordinate of the vector.
		 */
		Vector4(float x_param = 0.0f, float y_param = 0.0f, float z_param = 0.0f, float w_param = 0.0f);

		/**
		 * Creates a new Vector4 object representing the vector with the given coordinates.
		 *
		 * @param v A Vector3 object containing the X, Y, and Z coordinates of the vector.
		 * @param w_param The W coordinate of the vector.
		 */
		Vector4(const Vector3 &v, float w_param = 0.0f);

		/**
		 * Creates a new Vector4 object from a string representation of a vector.
		 *
		 * @param s The string representation of the vector.
		 */
		Vector4(const char *s);

		/**
		 * Creates a new Vector4 object from a string representation of a vector.
		 *
		 * @param s The string representation of the vector.
		 */
		Vector4(const std::string &s);

		/**
		 * @param i The index of the vector component to access.
		 * @return A reference to the vector component at the given index.
		 */
		float &operator [] (int i);

		/**
		 * @param i The index of the vector component to access.
		 * @return A reference to the vector component at the given index.
		 */
		float operator [] (int i) const;

		/**
		 * @return A Vector3 object containing the X, Y, and Z coordinates of this vector.
		 */
		Vector3 xyz() const;

		/**
		 * @return A Color3 object containing the red, green, and blue components of this color.
		 */
		Color3 rgb() const;

		/**
		 * Calculates the dot product of this vector with another vector, which is defined as the sum of each component of this vector multiplied by the corresponding component in the other vector.
		 *
		 * @param v The vector to take the dot product of with this vector.
		 * @return The dot product of this vector and the given vector.
		 */
		float dotProduct(const Vector4 &v) const;

		/**
		 * Calculates the length (or magnitude) of this vector, which is defined as the square root of the sum of the squared components of the vector (sqrt(x^2 + y^2 + z^2)).
		 *
		 * @return The length (or magnitude) of this vector.
		 */
		float length() const;

		/**
		 * Calculates the normalized version of this vector, which is a new vector that has the same direction as this vector but a length (magnitude) of 1.
		 *
		 * @return A Vector4 object that is the result of normalizing this vector.
		 */
		Vector4 normalize() const;

		/**
		 * @param v The vector to add to this vector.
		 * @return A Vector4 object that is the result of adding each component of the given vector to the corresponding components of this vector.
		 */
		Vector4 operator + (const Vector4 &v) const;

		/**
		 * @param f The scalar to add to this vector.
		 * @return A Vector4 object that is the result of adding the given scalar to each component of this vector.
		 */
		Vector4 operator + (float f) const;

		/**
		 * Adds each component of the given vector to the corresponding components of this vector.
		 *
		 * @param v The vector to add to this vector.
		 */
		void operator += (const Vector4 &v);

		/**
		 * Adds the given scalar to each component of this vector.
		 *
		 * @param f The scalar to add to this vector.
		 */
		void operator += (float f);

		/**
		 * @param v The vector to subtract from this vector.
		 * @return A Vector4 object that is the result of subtracting each component of the given vector from the corresponding components of this vector.
		 */
		Vector4 operator - (const Vector4 &v) const;

		/**
		 * @param f The scalar to subtract from this vector.
		 * @return A Vector4 object that is the result of subtracting the given scalar from each component of this vector.
		 */
		Vector4 operator - (float f) const;

		/**
		 * Subtracts each component of the given vector from the corresponding components of this vector.
		 *
		 * @param v The vector to subtract from this vector.
		 */
		void operator -= (const Vector4 &v);

		/**
		 * Subtracts the given scalar from each component of this vector.
		 *
		 * @param f The scalar to subtract from this vector.
		 */
		void operator -= (float f);

		/**
		 * @param v The vector to multiply this vector by.
		 * @return A Vector4 object that is the result of multiplying each component of this vector by the corresponding components of the given vector.
		 */
		Vector4 operator * (const Vector4 &v) const;

		/**
		 * @param f The scalar to multiply this vector by.
		 * @return A Vector4 object that is the result of multiplying each component of this vector by the given scalar.
		 */
		Vector4 operator * (float f) const;

		/**
		 * Multiplies each component of this vector by the corresponding components of the given vector.
		 *
		 * @param v The vector to multiply this vector by.
		 */
		void operator *= (const Vector4 &v);

		/**
		 * Multiplies each component of this vector by the given scalar.
		 *
		 * @param f The scalar to multiply this vector by.
		 */
		void operator *= (float f);

		/**
		 * @param f The scalar to divide this vector by.
		 * @return A Vector4 object that is the result of dividing each component of this vector by the given scalar.
		 */
		Vector4 operator / (float f) const;

		/**
		 * Divides each component of this vector by the given scalar.
		 *
		 * @param f The scalar to divide this vector by.
		 */
		void operator /= (float f);

		/**
		 * @return A string representation of the vector.
		 */
		std::string toString() const;
};

typedef Vector4 Color4;

} // namespace DromeMath

#endif /* __DROMEMATH_VECTOR4_H__ */
