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
#include <DromeMath/Rect2i.h>
#include <DromeMath/Vector4.h>
#include "Endian.h"
#include "Ref.h"

namespace DromeCore {

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

		DromeMath::Color4 getPixel(unsigned int x, unsigned int y) const;
		void setPixel(unsigned int x, unsigned int y, DromeMath::Color4 c);
		void copyFrom(DromeCore::RefPtr <Image> image, const DromeMath::Rect2i &srcBounds, const DromeMath::Rect2i &destBounds);

		RefPtr <Image> crop(const DromeMath::Rect2i &bounds);
		RefPtr <Image> scale(unsigned int width, unsigned int height);
		void save(const char *filename);
		void save(const std::string &filename);

		static RefPtr <Image> create(const std::string &filename);
		static RefPtr <Image> create(const char *filename);
		static RefPtr <Image> create(unsigned int width, unsigned int height, int colorComponents, const uint8_t *data = NULL);
};

} // namespace DromeCore

#endif /* __DROMECORE_IMAGE_H__ */
