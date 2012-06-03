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
#import <DromeCore/DromeCore>
#import <DromeGL/DromeGL>
#import <DromeMath/DromeMath>
#import "ES1Renderer.h"

using namespace std;
using namespace DromeCore;
using namespace DromeGL;
using namespace DromeMath;

@implementation ES1Renderer

- (id)init
{
    if((self = [super init])) {
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if(!context || ![EAGLContext setCurrentContext:context]) {
			[self release];
			return nil;
		}

		// set up framebuffer and renderbuffer
		glGenFramebuffers(1, &defaultFramebuffer);
		glGenRenderbuffers(1, &colorRenderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_RENDERBUFFER, colorRenderbuffer);
			
		// set up other GL stuff
		glClearColor(0.5f, 0.5f, 0.75f, 1.0f);
		glClearDepthf(1.0f);
    }

    return self;
}

- (void)render
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	[context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,
		&backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,
		&backingHeight);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"Failed to make complete framebuffer object %x",
			glCheckFramebufferStatus(GL_FRAMEBUFFER));
		return NO;
	}

	return YES;
}

- (void)dealloc
{
	if(defaultFramebuffer) {
		glDeleteFramebuffers(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}

	if(colorRenderbuffer) {
		glDeleteRenderbuffers(1, &colorRenderbuffer);
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
