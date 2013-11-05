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

#import <ApplicationServices/ApplicationServices.h>
#import <OpenGL/gl3.h>
#import "Texture.h"

struct ImageInfo
{
	unsigned int width;
	unsigned int height;
	unsigned int components;
	uint8_t *data;
};

static struct ImageInfo
loadImage(const char *filePath)
{
	struct ImageInfo info;
	info.width = 0;
	info.height = 0;
	info.components = 0;
	info.data = NULL;
	
	// create data provider
	CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(filePath);
	if(!dataProvider)
		return info;

	// create the CGImage using the data provider
	CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, NULL,
		false, kCGRenderingIntentDefault);

	// release the data provider
	CGDataProviderRelease(dataProvider);

	info.width = (unsigned int)CGImageGetWidth(image);
	info.height = (unsigned int)CGImageGetHeight(image);
	info.components = (unsigned int)CGImageGetBitsPerPixel(image) / 8;

	// get image data
	CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(image));

	// copy image data
	info.data = malloc(info.width * info.height * info.components);
	CFRange range;
	range.location = 0;
	range.length = info.width * info.height * info.components;
	CFDataGetBytes(imageData, range, (UInt8 *)info.data);

	// release image
	CFRelease(imageData);
	CGImageRelease(image);

	return info;
}

@implementation Texture

@synthesize textureId = _textureId;
@synthesize width = _width;
@synthesize height = _height;

- (id)initFromFile:(NSString *)filePath
{
	if((self = [super init])) {
		// load image
		NSString *fullPath = [[[[NSBundle mainBundle] resourcePath]
			stringByAppendingString:@"/"] stringByAppendingString:filePath];
		struct ImageInfo info = loadImage([fullPath UTF8String]);
		if(info.data == NULL)
			return nil;
		
		// determine the texture format from the image
		GLint format;
		switch(info.components) {
			default:
				free(info.data);
				return nil;
				break;
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
		}
		
		// generate and bind texture
		glGenTextures(1, &_textureId);
		glBindTexture(GL_TEXTURE_2D, _textureId);

		// set parameters
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		// create the texture using the image data
		glTexImage2D(GL_TEXTURE_2D, 0, format, info.width, info.height, 0,
			format, GL_UNSIGNED_BYTE, (const GLvoid *)info.data);
		_width = info.width;
		_height = info.height;

		free(info.data);
		return self;
	}
	
	return nil;
}

- (void)dealloc
{
	glDeleteTextures(1, &_textureId);
}

@end