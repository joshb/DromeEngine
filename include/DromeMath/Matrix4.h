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

#ifndef __DROMEMATH_MATRIX4_H__
#define __DROMEMATH_MATRIX4_H__

#include <string>
#include "Vector3.h"

namespace DromeMath {

/** \brief Represents a 4x4 matrix.
 */
class Matrix4
{
	protected:
		float m_matrix[16];

	public:
		/**
		 * Creates a new Matrix4 object representing a 4x4 identity matrix.
		 */
		Matrix4();

		/**
		 * Creates a new Matrix4 object initialized with the given matrix elements.
		 *
		 * @param matrix An array of 16 floats containing the elements of the new 4x4 matrix.
		 */
		Matrix4(const float matrix[16]);

		/**
		 * @return A pointer to the array of floats that makes up the matrix.
		 */
		const float *getData() const;

		/**
		 * @param i The index of the matrix element to access.
		 * @return A reference to the matrix element at the given index.
		 */
		float &operator [] (int i);

		/**
		 * @param scalar A scalar to multiply this matrix by.
		 * @return A Matrix4 object that is the result of multiplying this matrix by a scalar.
		 */
		Matrix4 operator * (float scalar) const;

		/**
		 * @param matrix A matrix to add to this matrix.
		 * @return A Matrix4 object that is the result of adding another matrix to this matrix.
		 */
		Matrix4 operator + (const Matrix4 &matrix) const;

		/**
		 * @param matrix A matrix to subtract from this matrix.
		 * @return A Matrix4 object that is the result of subtracting another matrix from this matrix.
		 */
		Matrix4 operator - (const Matrix4 &matrix) const;

		/**
		 * @param matrix A matrix to multiply this matrix by.
		 * @return A Matrix4 object that is the result of multiplying this matrix by another matrix.
		 */
		Matrix4 operator * (const Matrix4 &matrix) const;

		/**
		 * @param vector A vector multiply this matrix by.
		 * @return A Vector3 object that is the result of multiplying this matrix by a vector.
		 */
		Vector3 operator * (const Vector3 &vector) const;

		/**
		 * @return A Matrix4 object that is the result of taking the transpose of this matrix.
		 */
		Matrix4 transpose() const;

		/**
		 * @param vector A vector to translate this matrix by.
		 * @return A Matrix4 object that is the result of translating this matrix by the given vector.
		 */
		Matrix4 translate(const Vector3 &vector) const;

		/**
		 * @return A Matrix4 object that is the result of removing the translation from this matrix.
		 */
		Matrix4 removeTranslation() const;

		/**
		 * @return A string representation of this matrix.
		 */
		std::string toString() const;

		/**
		 * @param left The left coordinate of the orthographic matrix.
		 * @param right The right coordinate of the orthographic matrix.
		 * @param bottom The bottom coordinate of the orthographic matrix.
		 * @param top The top coordinate of the orthographic matrix.
		 * @param znear The Z near plane coordinate of the orthographic matrix.
		 * @param zfar The Z far plane coordinate of the orthographic matrix.
		 * @return A Matrix4 object representing the orthographic matrix generated from the given parameters.
		 */
		static Matrix4 orthographic(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = -1.0f, float znear = -1.0f, float zfar = 1.0f);

		/**
		 * @param fov The field of view of the perspective matrix.
		 * @param aspect The aspect ratio of the perspective matrix.
		 * @param znear The distance to the near Z plane of the perspective matrix.
		 * @param zfar The distance to the far Z plane of the perspective matrix.
		 * @return A Matrix4 object representing the perspective matrix generated from the given parameters.
		 */
		static Matrix4 perspective(float fov, float aspect, float znear, float zfar);

		/**
		 * @param vector A vector to translate by.
		 * @return A translation matrix that translates by the given vector.
		 */
		static Matrix4 translation(const Vector3 &vector);

		/**
		 * @param vector A vector to scale by.
		 * @return A scaling matrix that scales by the given vector.
		 */
		static Matrix4 scaling(const Vector3 &vector);

		/**
		 * @param angle An angle (in radians) to rotate around the vector.
		 * @param vector A vector to rotate around. This vector does not need to be normalized.
		 * @return A rotation matrix that rotates by the given angle around the given vector.
		 */
		static Matrix4 rotation(float angle, const Vector3 &vector);
};

} // namespace DromeMath

#endif /* __DROMEMATH_MATRIX4_H__ */
