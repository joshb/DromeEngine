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

#import <iostream>
#import <DromeCore/Exception.h>
#import "ES1Renderer.h"

using namespace std;
using namespace DromeCore;
using namespace DromeMath;
using namespace DromeGfx;
using namespace DromeGui;

@implementation ES1Renderer

- (id)init
{
    if((self = [super init])) {
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		if(!context || ![EAGLContext setCurrentContext:context]) {
			[self release];
			return nil;
		}

		glGenFramebuffersOES(1, &defaultFramebuffer);
		glGenRenderbuffersOES(1, &colorRenderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);

		// setup driver/camera
		driver = GfxDriver::create();
		camera.setPosition(Vector3(0.0f, -4.0f, 0.0f));
		camera.update();

		// create cylinder
		mesh = CylinderMesh::create(36);
		texture = Texture::create(Image::create("texture.png"));

		// create logo widget
		logo = DromeGui::Picture::create(driver, Image::create("drome.png"));
		logo->setWidth(logo->getWidth() / 2);
		logo->setHeight(logo->getHeight() / 2);
    }

    return self;
}

- (void)render
{
	driver->clearBuffers();

	driver->setModelViewMatrix(camera.getMatrix());
	driver->bindTexture(texture);
	mesh->render();

	// render GUI elements
	driver->enable2D();
	logo->render(driver);
	driver->disable2D();

	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}

	driver->setViewportDimensions(backingWidth, backingHeight);
	driver->setProjectionMatrix(backingWidth, backingHeight);
	logo->setY(backingHeight - logo->getHeight());

	return YES;
}

- (void)moveCamera:(CGPoint)delta
{
	camera.rotateYaw(delta.x * -0.5f);
	camera.rotatePitch(delta.y * -0.5f);
	camera.update();
}

- (void)dealloc
{
	if(defaultFramebuffer) {
		glDeleteFramebuffersOES(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}

	if(colorRenderbuffer) {
		glDeleteRenderbuffersOES(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}

	// Tear down context
	if([EAGLContext currentContext] == context)
		[EAGLContext setCurrentContext:nil];

	[context release];
	context = nil;

	[super dealloc];
}

@end
