/*
 * Copyright (C) 2004, 2010 Josh A. Beam
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
#include <cstring>
#include <DromeCore/DromeCore>
#include <DromeGfx/Image.h>
#include <DromeMath/DromeMath>

static float height_scale = 64.0f;

using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeMath;

static uint8_t *
generate_normalmap(RefPtr <Image> bumpimg)
{
	unsigned int width = bumpimg->getWidth();
	unsigned int height = bumpimg->getHeight();
	uint8_t *normal = new uint8_t[width * height * 3];

	for(unsigned int i = 0; i < height; ++i) {
		for(unsigned int j = 0; j < width; ++j) {
			Vector3 v1, v2, v3;

			v1.x = 0.0f;
			v1.y = 0.0f;
			v1.z = bumpimg->getPixel(j, i).getFloatR() * height_scale;

			v2.x = 1.0f;
			v2.y = 0.0f;
			v2.z = bumpimg->getPixel((j+1) % width, i).getFloatR() * height_scale;

			v3.x = 0.0f;
			v3.y = -1.0f;
			v3.z = bumpimg->getPixel(j, (i+1) % height).getFloatR() * height_scale;

			// create normal
			v2 -= v1;
			v3 -= v1;
			Vector3 n = v3.crossProduct(v2).normalize();

			// put normal in [0, 1] range
			n += 1.0f;
			n /= 2.0f;

			// write to pixel data
			unsigned int index = (width * 3 * (height-i-1)) + (j * 3);
			normal[index + 0] = (uint8_t)(n.x * 255.0f);
			normal[index + 1] = (uint8_t)(n.y * 255.0f);
			normal[index + 2] = (uint8_t)(n.z * 255.0f);
		}
	}

	return normal;
}

static void
write_to_tga(const uint8_t *data, unsigned int width, unsigned int height,
             FILE *fp)
{
	unsigned char buf[18];

	for(int i = 0; i < 18; i++)
		buf[i] = 0;
	buf[2] = 2;
	buf[12] = width & 0xff;
	buf[13] = (width >> 8) & 0xff;
	buf[14] = height & 0xff;
	buf[15] = (height >> 8) & 0xff;
	buf[16] = 24; // bpp
	buf[17] = 0; // number of alpha bits

	fwrite(buf, 18, 1, fp);
	for(unsigned int i = 0; i < width * 3 * height; i += 3) {
		uint8_t pixel[3];

		pixel[0] = data[i+2];
		pixel[1] = data[i+1];
		pixel[2] = data[i+0];

		fwrite(pixel, 3, 1, fp);
	}
}

int
main(int argc, char *argv[])
{
	// parse command line options
	for(int i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "--scale") == 0) {
			sscanf(argv[i+1], "%f", &height_scale);
			fprintf(stderr, "Height scale set to %f\n", height_scale);

			for(int j = i; j < argc - 1; ++j)
				argv[j] = argv[j + 1];
			--argc;
			for(int j = i; j < argc - 1; ++j)
				argv[j] = argv[j + 1];
			--argc;
		}
	}

	if(argc != 3) {
		fprintf(stderr, "This program generates a normal map from a heightfield bump map.\n");
		fprintf(stderr, "Usage:\n\t%s [--scale <height scale>] <input filename> <output filename>\n", argv[0]);
		return 1;
	}

	fprintf(stderr, "Loading bump map %s ...\n", argv[1]);
	RefPtr <Image> img = Image::create(argv[1]);

	fprintf(stderr, "Generating normal map...\n");
	uint8_t *normal = generate_normalmap(img);

	fprintf(stderr, "Saving normal map to %s ...\n", argv[2]);
	FILE *fp = fopen(argv[2], "wb");
	if(!fp) {
		fprintf(stderr, "Error: Couldn't open %s for writing\n", argv[2]);
		return 1;
	}
	write_to_tga(normal, img->getWidth(), img->getHeight(), fp);
	fclose(fp);

	fprintf(stderr, "Done\n");
	delete [] normal;

	return 0;
}
