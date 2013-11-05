/*
 * Copyright (C) 2011-2012 Josh A. Beam
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

#import "MyNSOpenGLView.h"
#import "Matrix4.h"
#import <OpenGL/gl3.h>

@interface MyNSOpenGLView()
{
	Matrix4 *_projectionMatrix;
    NSTrackingArea *_trackingArea;
}

@end

@implementation MyNSOpenGLView

- (Matrix4 *)projectionMatrix
{
	return _projectionMatrix;
}

- (void)awakeFromNib
{
	NSOpenGLPixelFormatAttribute attr[] = {
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 32,
		0
	};

	NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc]
		initWithAttributes:attr];
	NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:format
		shareContext:nil];

	[self setOpenGLContext:context];
	[self.openGLContext makeCurrentContext];
}

- (void)reshape
{
	NSRect frame = self.frame;
	
	// set viewport
	glViewport(0, 0, (GLsizei)frame.size.width, (GLsizei)frame.size.height);
	
    // create projection matrix
    _projectionMatrix = [Matrix4 perspectiveMatrixWithFieldOfView:((float)M_PI / 4.0f) aspect:((float)frame.size.width / (float)frame.size.height) near:0.1f far:200.0f];
    
    // remove existing tracking area if necessary
    if(_trackingArea != nil)
        [self removeTrackingArea:_trackingArea];
	
    // create new tracking area
    _trackingArea = [[NSTrackingArea alloc] initWithRect:[self frame] options:NSTrackingMouseMoved|NSTrackingActiveWhenFirstResponder owner:self userInfo:nil];
    [self addTrackingArea:_trackingArea];
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)keyDown:(NSEvent *)theEvent
{
	switch(theEvent.keyCode) {
		default:
			break;
		case 0x35: // escape
			[self.window close];
			break;
	}
}

- (void)flush
{
	[self.openGLContext flushBuffer];
}

@end
