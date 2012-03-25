/*
 * Copyright (C) 2012 Josh A. Beam
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

#include <iostream>
#include <fstream>
#include <string>
#include <DromeCore/Exception.h>
#include <DromeCore/String.h>
#include <DromeGfx/Image.h>

using namespace std;
using namespace DromeCore;
using namespace DromeGfx;

int
main(int argc, char *argv[])
{
	// make sure file paths were given
	if(argc != 3) {
		cerr << "This program generates a C header file containing" << endl << "raw image data for a given image file." << endl << endl;
		cerr << "Usage: " << argv[0] << " <input image file path> <output header file path>" << endl;
		return 1;
	}

	// load image
	RefPtr <Image> image;
	try {
		image = Image::create(argv[1]);
	} catch(Exception ex) {
		cerr << "Unable to load image: " << ex.toString() << endl;
		return 1;
	}

	// open header for writing
	fstream header(argv[2], fstream::out);
	if(header.is_open() == false) {
		cerr << "Unable to open header file for writing" << endl;
		return 1;
	}

	// write image dimensions and number of components to the header
	header << "const unsigned int imageWidth = " << image->getWidth() << ';' << endl;
	header << "const unsigned int imageHeight = " << image->getHeight() << ';' << endl;
	header << "const unsigned int imageNumComponents = " << image->getNumComponents() << ';' << endl;

	// write image data to the header
	header << "const uint8_t imageData[] = {" << endl << '\t';
	unsigned int numBytes = image->getWidth() * image->getHeight() * image->getNumComponents();
	const uint8_t *imageData = image->getData();
	for(unsigned int i = 0; i < numBytes; ++i) {
		header << (unsigned int)imageData[i];
		if(i != numBytes - 1) {
			header << ',';
			if((i % 16) == 15)
				header << endl << '\t';
		}
	}
	header << endl << "};" << endl;

	header.close();
	return 0;
}
