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

#ifndef __DROMECORE_IMAGE_H__
#define __DROMECORE_IMAGE_H__

#include <string>
#include <DromeMath/Vector3.h>
#include "Endian.h"
#include "Ref.h"

namespace DromeCore {

class Color {
	public:
		uint8_t r, g, b, a;

		Color()
		{
			r = g = b = a = 255;
		}

		Color(uint8_t rParam, uint8_t gParam, uint8_t bParam, uint8_t aParam = 255)
		{
			this->r = rParam;
			this->g = gParam;
			this->b = bParam;
			this->a = aParam;
		}

		Color(uint32_t c)
		{
			r = (c >> 24) & 0xff;
			g = (c >> 16) & 0xff;
			b = (c >> 8) & 0xff;
			a = c & 0xff;
		}

		Color(float rParam, float gParam, float bParam, float aParam = 1.0f)
		{
			this->r = (uint8_t)(rParam * 255.0f);
			this->g = (uint8_t)(gParam * 255.0f);
			this->b = (uint8_t)(bParam * 255.0f);
			this->a = (uint8_t)(aParam * 255.0f);
		}

		Color(const DromeMath::Vector3 &c, float aParam = 1.0f)
		{
			this->r = (uint8_t)(c.x * 255.0f);
			this->g = (uint8_t)(c.y * 255.0f);
			this->b = (uint8_t)(c.z * 255.0f);
			this->a = (uint8_t)(aParam * 255.0f);
		}

		Color operator * (const Color &color) const
		{
			Color tmp;

			tmp.setR(getFloatR() * color.getFloatR());
			tmp.setG(getFloatG() * color.getFloatG());
			tmp.setB(getFloatB() * color.getFloatB());
			tmp.setA(getFloatA() * color.getFloatA());

			return tmp;
		}
		void operator *= (const Color &color) { *this = *this * color; }

		Color operator * (float f) const
		{
			Color tmp;

			tmp.setR(getFloatR() * f);
			tmp.setG(getFloatG() * f);
			tmp.setB(getFloatB() * f);
			tmp.setA(getFloatA() * f);

			return tmp;
		}
		void operator *= (float f) { *this = *this * f; }

		float getFloatR() const { return (float)r / 255.0f; }
		float getFloatG() const { return (float)g / 255.0f; }
		float getFloatB() const { return (float)b / 255.0f; }
		float getFloatA() const { return (float)a / 255.0f; }

		void setR(uint8_t value) { r = value; }
		void setG(uint8_t value) { g = value; }
		void setB(uint8_t value) { b = value; }
		void setA(uint8_t value) { a = value; }

		void setR(float value) { r = (uint8_t)(value * 255.0f); }
		void setG(float value) { g = (uint8_t)(value * 255.0f); }
		void setB(float value) { b = (uint8_t)(value * 255.0f); }
		void setA(float value) { a = (uint8_t)(value * 255.0f); }

		uint32_t
		toUInt32() const
		{
			return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
		}

		Color
		invert() const
		{
			return Color(255 - r, 255 - g, 255 - b, a);
		}
};

class Image : public RefClass {
	protected:
		std::string m_filename;
		uint8_t *m_data;
		unsigned int m_width, m_height;
		int m_colorComponents;

		Image();
		Image(unsigned int width, unsigned int height, int colorComponents, const uint8_t *data = NULL);
		virtual ~Image();

	public:
		std::string getFilename() const;
		const uint8_t *getData() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		int getNumComponents() const;

		Color getPixel(unsigned int x, unsigned int y) const;
		void setPixel(unsigned int x, unsigned int y, Color c);
		void copyFrom(DromeCore::RefPtr <Image> image);

		RefPtr <Image> scale(unsigned int width, unsigned int height);

		static RefPtr <Image> create(const std::string &filename);
		static RefPtr <Image> create(const char *filename);
		static RefPtr <Image> create(unsigned int width, unsigned int height, int colorComponents, const uint8_t *data = NULL);
};

} // namespace DromeCore

#endif /* __DROMECORE_IMAGE_H__ */
