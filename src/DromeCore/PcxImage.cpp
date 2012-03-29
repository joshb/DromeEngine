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

#include <cstdio>
#include <cstdlib>
#include <DromeCore/Endian.h>
#include <DromeCore/Exception.h>
#include "PcxImage.h"

using namespace std;

namespace DromeCore {

struct pcx_header {
	uint8_t manufacturer;
	uint8_t version;
	uint8_t encoding;
	uint8_t bitsperpixel;
	int16_t xmin;
	int16_t ymin;
	int16_t xmax;
	int16_t ymax;
	uint16_t horizdpi;
	uint16_t vertdpi;
	uint8_t palette[48];
	uint8_t reserved;
	uint8_t colorplanes;
	uint16_t bytesperline;
	uint16_t palettetype;
	uint16_t hscrsize;
	uint16_t vscrsize;
	uint8_t filler[54];
};

static uint8_t *load_pcx_data_8(FILE *fp, int width, int height, unsigned int bytesperline);
static uint8_t *load_pcx_data_24(FILE *fp, int width, int height, unsigned int bytesperline);

PcxImage::PcxImage(const char *filename_arg)
{
	m_filename = filename_arg;

	// open file
	FILE *fp = fopen(filename_arg, "rb");
	if(!fp)
		throw Exception(string("PcxImage::PcxImage(): Couldn't open ") + filename_arg);

	// read header and do byte-swapping
	struct pcx_header header;
	fread(&header, sizeof(struct pcx_header), 1, fp);
	header.xmin = littleToNativeInt16(header.xmin);
	header.ymin = littleToNativeInt16(header.ymin);
	header.xmax = littleToNativeInt16(header.xmax);
	header.ymax = littleToNativeInt16(header.ymax);
	header.bytesperline = littleToNativeUInt16(header.bytesperline);

	// make sure the number of bits per pixel is supported
	if(header.bitsperpixel != 8) {
		fclose(fp);
		throw Exception(string("PcxImage::PcxImage(): ") + filename_arg + " has unsupported number of bits per pixel");
	}

	// calculate dimensions
	m_width = header.xmax - header.xmin + 1;
	m_height = header.ymax - header.ymin + 1;
	if(m_width < 1 || m_height < 1) {
		fclose(fp);
		throw Exception(string("PcxImage::PcxImage(): ") + filename_arg + " has bad dimensions");
	}

	// call the approriate function to load image
	// data based on the number of color planes
	switch(header.colorplanes) {
		default:
			fclose(fp);
			throw Exception(string("PcxImage::PcxImage(): ") + filename_arg + " has unsupported number of color planes");
			break;
		case 1:
			m_data = load_pcx_data_8(fp, m_width, m_height, header.bytesperline);
			break;
		case 3:
			m_data = load_pcx_data_24(fp, m_width, m_height, header.bytesperline);
			break;
	}

	m_colorComponents = 3;
	fclose(fp);

	if(!m_data)
		throw Exception(string("PcxImage::PcxImage(): Unable to load ") + filename_arg);
}

static int
read_scanline(FILE *fp, uint8_t *planes[], unsigned int num_planes,
              unsigned int bytesperline)
{
	unsigned int i, j;
	unsigned int p, count;
	uint8_t byte;

	for(p = 0; p < num_planes; p++) {
		for(i = 0; i < bytesperline;) {
			if(fread(&byte, 1, 1, fp) != 1)
				return 0;

			if(byte >> 6 == 0x3) {
				count = byte & ~(0x3 << 6);
				if(count == 0)
					return 0;
				if(fread(&byte, 1, 1, fp) != 1)
					return 0;
			} else {
				count = 1;
			}

			for(j = 0; j < count; j++)
				planes[p][i++] = byte;

			if(i >= bytesperline) {
				p++;
				if(p >= num_planes)
					return 1;
				i = 0;
			}
		}
	}

	return 1;
}

static uint8_t *
load_pcx_data_8(FILE *fp, int width, int height,
                unsigned int bytesperline)
{
	int i, j;
	int max;
	uint8_t *data, *p_data;
	uint8_t *line, *planes[1];
	unsigned int current_line = 0;
	uint8_t byte;
	uint8_t palette[256][3];

	p_data = new uint8_t[width * height];
	line = new uint8_t[bytesperline];
	planes[0] = line;

	while(current_line < (unsigned int)height) {
		if(read_scanline(fp, planes, 1, bytesperline) == 0) {
			delete [] p_data;
			delete [] line;
			return NULL;
		}

		for(i = 0; i < width; i++)
			p_data[width * current_line + i] = planes[0][i];

		current_line++;
	}

	/* read palette */
	fseek(fp, -769, SEEK_END);
	fread(&byte, 1, 1, fp);
	if(byte != 12) {
		delete [] p_data;
		delete [] line;
		return NULL;
	}
	for(i = 0; i < 256; i++)
		fread(palette[i], 3, 1, fp);

	data = new uint8_t[width * height * 3];
	max = width * height;
	j = 0;
	for(i = 0; i < max; i++) {
		data[j++] = palette[(p_data[i])][0];
		data[j++] = palette[(p_data[i])][1];
		data[j++] = palette[(p_data[i])][2];
	}

	delete [] p_data;
	delete [] line;
	return data;
}

static uint8_t *
load_pcx_data_24(FILE *fp, int width, int height,
                 unsigned int bytesperline)
{
	int i;
	uint8_t *data;
	uint8_t *line, *planes[3];
	unsigned int current_line = 0;

	data = new uint8_t[width * height * 3];
	line = new uint8_t[bytesperline * 3];

	planes[0] = line;
	planes[1] = planes[0] + bytesperline;
	planes[2] = planes[1] + bytesperline;

	while(current_line < (unsigned int)height) {
		if(read_scanline(fp, planes, 3, bytesperline) == 0) {
			delete [] data;
			delete [] line;
			return NULL;
		}

		for(i = 0; i < width; i++) {
			data[width * current_line * 3 + i * 3 + 0] = planes[0][i];
			data[width * current_line * 3 + i * 3 + 1] = planes[1][i];
			data[width * current_line * 3 + i * 3 + 2] = planes[2][i];
		}

		current_line++;
	}

	delete [] line;
	return data;
}

RefPtr <PcxImage>
PcxImage::create(const char *filename)
{
	return RefPtr <PcxImage> (new PcxImage(filename));
}

} // namespace DromeCore
