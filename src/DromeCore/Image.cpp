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

#include <DromeCore/Exception.h>
#include <DromeCore/File.h>
#include <DromeCore/String.h>
#include <DromeCore/Util.h>
#include <DromeCore/Image.h>
#include "PcxImage.h"
#include "PngImage.h"

using namespace std;
using namespace DromeMath;

namespace DromeCore {

/*
 * Image
 */
Image::Image()
{
	m_data = NULL;
	m_width = m_height = m_colorComponents = 0;
}

Image::Image(unsigned int width, unsigned int height,
             int colorComponents, const uint8_t *data)
{
	m_width = width;
	m_height = height;
	m_colorComponents = colorComponents;

	m_data = new uint8_t[m_width * m_height * m_colorComponents];
	if(data != NULL) {
		for(unsigned int i = 0; i < m_width * m_height * m_colorComponents; ++i)
			m_data[i] = data[i];
	} else {
		for(unsigned int i = 0; i < m_width * m_height * m_colorComponents; ++i)
			m_data[i] = 0;
	}
}

Image::~Image()
{
	if(m_data)
		delete [] m_data;
}

string
Image::getFilename() const
{
	return m_filename;
}

const uint8_t *
Image::getData() const
{
	return m_data;
}

unsigned int
Image::getWidth() const
{
	return m_width;
}

unsigned int
Image::getHeight() const
{
	return m_height;
}

int
Image::getNumComponents() const
{
	return m_colorComponents;
}

Color4
Image::getPixel(unsigned int x, unsigned int y) const
{
	if(!m_data)
		throw Exception("Image::getPixel(): No image data available");

	if(x >= m_width || y >= m_height)
		throw Exception(String("Image::getPixel(): Pixel ") + String(x) + "x" + String(y) + " is out of image bounds (" + String(m_width) + "x" + String(m_height) + ")");

	Color4 pixel;
	int tmp;

	switch(m_colorComponents) {
		default:
			break;
		case 1:
			pixel.r = pixel.g = pixel.b = (float)m_data[(m_width * y) + x] / 255.0f;
			pixel.a = 1.0f;
			break;
		case 2:
			tmp = ((m_width * y) + x) * 2;
			pixel.r = pixel.g = pixel.b = (float)m_data[tmp + 0] / 255.0f;
			pixel.a = (float)m_data[tmp + 1] / 255.0f;
			break;
		case 3:
			tmp = ((m_width * y) + x) * 3;
			pixel.r = (float)m_data[tmp + 0] / 255.0f;
			pixel.g = (float)m_data[tmp + 1] / 255.0f;
			pixel.b = (float)m_data[tmp + 2] / 255.0f;
			pixel.a = 1.0f;
			break;
		case 4:
			tmp = ((m_width * y) + x) * 4;
			pixel.r = (float)m_data[tmp + 0] / 255.0f;
			pixel.g = (float)m_data[tmp + 1] / 255.0f;
			pixel.b = (float)m_data[tmp + 2] / 255.0f;
			pixel.a = (float)m_data[tmp + 3] / 255.0f;
			break;
	}

	return pixel;
}

void
Image::setPixel(unsigned int x, unsigned int y, Color4 c)
{
	if(!m_data)
		throw Exception("Image::setPixel(): No image data available");

	if(x >= m_width || y >= m_height)
		throw Exception(String("Image::setPixel(): Pixel ") + String(x) + "x" + String(y) + " is out of image bounds (" + String(m_width) + "x" + String(m_height) + ")");

	switch(m_colorComponents) {
		default:
			break;
		case 1:
			m_data[(m_width * y) + x] = (uint8_t)(c.r * 255.0f);
			break;
		case 2:
			m_data[(m_width * 2 * y) + (x * 2) + 0] = (uint8_t)(c.r * 255.0f);
			m_data[(m_width * 2 * y) + (x * 2) + 1] = (uint8_t)(c.a * 255.0f);
			break;
		case 3:
			m_data[(m_width * 3 * y) + (x * 3) + 0] = (uint8_t)(c.r * 255.0f);
			m_data[(m_width * 3 * y) + (x * 3) + 1] = (uint8_t)(c.g * 255.0f);
			m_data[(m_width * 3 * y) + (x * 3) + 2] = (uint8_t)(c.b * 255.0f);
			break;
		case 4:
			m_data[(m_width * 4 * y) + (x * 4) + 0] = (uint8_t)(c.r * 255.0f);
			m_data[(m_width * 4 * y) + (x * 4) + 1] = (uint8_t)(c.g * 255.0f);
			m_data[(m_width * 4 * y) + (x * 4) + 2] = (uint8_t)(c.b * 255.0f);
			m_data[(m_width * 4 * y) + (x * 4) + 3] = (uint8_t)(c.a * 255.0f);
			break;
	}
}

RefPtr <Image>
Image::scale(unsigned int width, unsigned int height)
{
	// make sure the dimensions are valid
	if(width == 0 || height == 0)
		throw Exception("Image::scale(): Invalid width/height (both must be non-zero)");

	// create the new image
	RefPtr <Image> image = RefPtr <Image> (new Image(width, height, m_colorComponents));

	// set the pixels of the new image
	for(unsigned int y = 0; y < height; ++y) {
		for(unsigned int x = 0; x < width; ++x) {
			unsigned int src_y = (unsigned int)(((float)y / (float)height) * (float)m_height);
			unsigned int src_x = (unsigned int)(((float)x / (float)width) * (float)m_width);

			image->setPixel(x, y, getPixel(src_x, src_y));
		}
	}

	return image;
}

void
Image::copyFrom(RefPtr <Image> image)
{
	// get smallest dimensions
	unsigned int w = image->getWidth();
	unsigned int h = image->getHeight();
	if(m_width < w)
		w = m_width;
	if(m_height < h)
		h = m_height;

	// copy pixels
	for(unsigned int y = 0; y < h; ++y) {
		for(unsigned int x = 0; x < w; ++x)
			setPixel(x, y, image->getPixel(x, y));
	}
}

RefPtr <Image>
Image::create(const string &filename)
{
	// get file extension position
	size_t tmp = filename.find_last_of('.');
	if(tmp == string::npos)
		throw Exception(string("Image::create(): No file extension: ") + filename);

	// get file extension and use it to call the
	// appropriate function to load the image
	string extension = filename.substr(tmp);
	if(strCaseCmp(extension.c_str(), ".pcx") == 0)
		return PcxImage::create(File::getPath(filename).c_str());
	else if(strCaseCmp(extension.c_str(), ".png") == 0)
		return PngImage::create(File::getPath(filename).c_str());
	else
		throw Exception(string("Image::create(): Unsupported file extension: ") + filename);
}

RefPtr <Image>
Image::create(const char *filename)
{
	return create(string(filename));
}

RefPtr <Image>
Image::create(unsigned int width, unsigned int height,
              int colorComponents, const uint8_t *data)
{
	return RefPtr <Image> (new Image(width, height, colorComponents, data));
}

} // namespace DromeCore
