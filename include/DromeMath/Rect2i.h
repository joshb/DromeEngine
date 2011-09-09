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

#ifndef __DROMEMATH_RECT2I_H__
#define __DROMEMATH_RECT2I_H__

#include <string>
#include "Vector2i.h"

namespace DromeMath {

/**
 * \brief Represents a two-dimensional integer rectangle.
 */
class Rect2i
{
	public:
		/**
		 * The minimum boundaries of the rectangle.
		 */
		Vector2i min;

		/**
		 * The maximum boundaries of the rectangle.
		 */
		Vector2i max;

		/**
		 * Creates a new Rect2i object representing the rectangle with the given boundaries.
		 *
		 * @param min_param The minimum boundaries of the rectangle.
		 * @param max_param The maximum boundaries of the rectangle.
		 */
		Rect2i(const Vector2i &min_param = Vector2i(), const Vector2i &max_param = Vector2i());

		/**
		 * @return The width of the rectangle.
		 */
		int getWidth() const;

		/**
		 * @return The height of the rectangle.
		 */
		int getHeight() const;

		/**
		 * @return A Vector2i object with the X component equal to the width of the rectangle and the Y component equal to the height of the triangle.
		 */
		Vector2i getDimensions() const;

		/**
		 * @return A string representation of the rectangle.
		 */
		std::string toString() const;
};

} // namespace DromeMath

#endif /* __DROMEMATH_RECT2I_H__ */
