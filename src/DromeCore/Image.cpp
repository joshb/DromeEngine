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

#ifdef APPLE
	#ifdef IOS
		#include <CoreGraphics/CoreGraphics.h>
	#else
		#include <ApplicationServices/ApplicationServices.h>
	#endif /* IOS */
#else
	#include <png.h>
#endif /* APPLE */

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
Image::crop(const Rect2i &bounds)
{
	// make sure the dimensions are valid
	int width = bounds.getWidth();
	int height = bounds.getHeight();
	if(width <= 0 || height <= 0)
		throw Exception("Image::crop(): Invalid width/height (both must be greater than 0)");

	// make sure the bounds are valid
	if(bounds.min.x < 0 || (unsigned int)bounds.max.x > m_width ||
	   bounds.min.y < 0 || (unsigned int)bounds.max.y > m_height)
		throw Exception("Image::crop(): Invalid bounds (must be entirely within image)");

	// create the new image
	RefPtr <Image> image = RefPtr <Image> (new Image(width, height, m_colorComponents));

	// set the pixels of the new image
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x)
			image->setPixel(x, y, getPixel(bounds.min.x + x, bounds.min.y + y));
	}

	return image;
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
Image::copyFrom(RefPtr <Image> image,
                const Rect2i &srcBounds, const Rect2i &destBounds)
{
	// make sure the destination dimensions are valid
	int width = destBounds.getWidth();
	int height = destBounds.getHeight();
	if(width <= 0 || height <= 0)
		throw Exception("Image::copyFrom(): Invalid width/height for destination bounds (both must be greater than 0)");

	// make sure the destination bounds are valid
	if(destBounds.min.x < 0 || (unsigned int)destBounds.max.x > m_width ||
	   destBounds.min.y < 0 || (unsigned int)destBounds.max.y > m_height)
		throw Exception("Image::copyFrom(): Invalid destination bounds (must be entirely within image)");

	// crop the image if necessary
	if(srcBounds.min.x != 0 || (unsigned int)srcBounds.max.x != image->getWidth() ||
	   srcBounds.min.y != 0 || (unsigned int)srcBounds.max.y != image->getHeight())
		image = image->crop(srcBounds);

	// scale the image if necessary
	if((unsigned int)width != image->getWidth() ||
	   (unsigned int)height != image->getHeight())
		image = image->scale((unsigned int)width, (unsigned int)height);

	// copy pixels
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x)
			setPixel(destBounds.min.x + x, destBounds.min.y + y,
			         image->getPixel(x, y));
	}
}

void
Image::save(const char *filename)
{
	// determine the PNG color type based
	// on the number of color components
	int colorType;
	switch(m_colorComponents) {
		default:
			throw Exception("Image::save(): Invalid number of color components");

		case 1:
			colorType = PNG_COLOR_TYPE_GRAY;
			break;

		case 2:
			colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
			break;

		case 3:
			colorType = PNG_COLOR_TYPE_RGB;
			break;

		case 4:
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
	}

	// open the destination file for writing
	FILE *fp = fopen(filename, "wb");
	if(!fp)
		throw Exception(string("Image::save(): Unable to open ") + filename + " for writing");

	// create the PNG write struct
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) {
		fclose(fp);
		throw Exception("Image::save(): png_create_write_struct failed");
	}

	// create the PNG info struct
	png_infop info = png_create_info_struct(png);
	if(!info) {
		png_destroy_write_struct(&png, NULL);
		fclose(fp);
		throw Exception("Image::save(): png_create_info_struct failed");
    }

	if(setjmp(png_jmpbuf(png))) {
		png_destroy_write_struct(&png, &info);
		fclose(fp);
		throw Exception("Image::save(): setjmp failed");
	}

	// set the PNG IHDR
	png_init_io(png, fp);
	png_set_IHDR(png, info, m_width, m_height, 8, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// create an array of row pointers
	png_bytepp rows = new png_bytep[m_height];
	for(unsigned int i = 0; i < m_height; ++i)
		rows[i] = m_data + (m_width * m_colorComponents * i);

	// set the rows and write the PNG
    png_set_rows(png, info, rows);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

	// close the file
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    delete [] rows;
}

void
Image::save(const string &filename)
{
	save(filename.c_str());
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
