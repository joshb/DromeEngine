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

#include <cstdio>
#include <cstdlib>
#ifdef APPLE
	#ifdef IOS
		#include <CoreGraphics/CoreGraphics.h>
	#else
		#include <ApplicationServices/ApplicationServices.h>
	#endif /* IOS */
#else
	#include <png.h>
#endif /* APPLE */
#include <DromeCore/Exception.h>
#include "PngImage.h"

using namespace std;
using namespace DromeCore;

namespace DromeGfx {

PngImage::PngImage(const char *filename_arg)
{
	m_filename = filename_arg;

#ifdef APPLE
	// create data provider
	CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(filename_arg);
	if(!dataProvider)
		throw Exception(string("PngImage::PngImage(): Couldn't open ") + filename_arg);

	// create the CGImage using the data provider
	CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, NULL, false, kCGRenderingIntentDefault);

	// release the data provider
	CGDataProviderRelease(dataProvider);

	m_width = CGImageGetWidth(image);
	m_height = CGImageGetHeight(image);
	m_colorComponents = CGImageGetBitsPerPixel(image) / 8;

	// get image data
	CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(image));

	// copy image data
	m_data = new uint8_t[m_width * m_height * m_colorComponents];
	CFRange range;
	range.location = 0;
	range.length = m_width * m_height * m_colorComponents;
	CFDataGetBytes(imageData, range, (UInt8 *)m_data);

	// release image
	CGImageRelease(image);
#else
	// open file
	FILE *fp = fopen(filename_arg, "rb");
	if(!fp)
		throw Exception(string("PngImage::PngImage(): Couldn't open ") + filename_arg);

	// read header
	png_byte header[9];
	fread(header, 1, 8, fp);

	// make sure header is correct
	if(png_sig_cmp(header, 0, 8) != 0) {
		fclose(fp);
		throw Exception("PngImage::PngImage(): png_sig_cmp failed");
	}

	// create png_struct
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) {
		fclose(fp);
		throw Exception("PngImage::PngImage(): png_create_read_struct failed");
	}

	// create png_info
	png_infop info = png_create_info_struct(png);
	if(!info) {
		png_destroy_read_struct(&png, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		throw Exception("PngImage::PngImage(): png_create_info_struct failed");
	}

	// setjmp
	if(setjmp(png_jmpbuf(png))) {
		png_destroy_read_struct(&png, &info, (png_infopp)NULL);
		fclose(fp);
		throw Exception("PngImage::PngImage(): setjmp failed");
	}

	// get ready to read image
	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);

	// read info
	png_read_info(png, info);

	// get IHDR
	png_uint_32 pngWidth, pngHeight;
	int bitDepth, colorType;
	png_get_IHDR(png, info, &pngWidth, &pngHeight, &bitDepth, &colorType, NULL, NULL, NULL);

	m_width = pngWidth;
	m_height = pngHeight;

	switch(colorType) {
		default:
			throw Exception("PngImage::PngImage(): Unsupported color type");
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			m_colorComponents = 4;
			break;
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png);
		case PNG_COLOR_TYPE_RGB:
			m_colorComponents = 3;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			m_colorComponents = 2;
			break;
		case PNG_COLOR_TYPE_GRAY:
			m_colorComponents = 1;
			break;
	}

	// pack or strip bits if necessary
	if(bitDepth < 8) {
		//if(colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		//	png_set_gray_1_2_4_to_8(png);
		//else
			png_set_packing(png);
	} else if(bitDepth == 16) {
		png_set_strip_16(png);
	}

	// allocate memory for image data
	m_data = new uint8_t[m_width * m_colorComponents * m_height];

	// read image
	png_bytepp rows = new png_bytep[m_height];
	for(unsigned int i = 0; i < m_height; ++i)
		rows[i] = (png_bytep)m_data + (m_width * m_colorComponents * i);
	png_start_read_image(png);
	png_read_image(png, rows);
	png_read_end(png, info);

	// finish up
	delete [] rows;
	png_destroy_read_struct(&png, &info, (png_infopp)NULL);
	fclose(fp);
#endif /* APPLE */
}

RefPtr <PngImage>
PngImage::create(const char *filename)
{
	return RefPtr <PngImage> (new PngImage(filename));
}

} // namespace DromeGfx
