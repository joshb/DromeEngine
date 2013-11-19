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

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "DromeGLView.h"

@interface DromeGLView()
{
	GLuint _defaultFramebuffer, _colorRenderbuffer;
}

@property (strong, nonatomic) EAGLContext *context;

@end

@implementation DromeGLView

@synthesize context = _context;

+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (void)dealloc
{
	[self tearDownGL];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	self = [super initWithCoder:aDecoder];
	if(self) {
		// set up the CAEAGLLayer
		CAEAGLLayer *layer = (CAEAGLLayer *)self.layer;
		layer.opaque = TRUE;
		layer.drawableProperties = [NSDictionary
			dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE],
				kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8,
				kEAGLDrawablePropertyColorFormat, nil];

		// set up display link
		CADisplayLink *link = [CADisplayLink displayLinkWithTarget:self
			selector:@selector(drawView:)];
		link.frameInterval = 1;
		[link addToRunLoop:[NSRunLoop currentRunLoop]
			forMode:NSDefaultRunLoopMode];
			
		// create EAGLContext
		self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if(!self.context) {
			NSLog(@"Failed to create ES2 context");
		}
		
		[self setupGL];
	}
	
	return self;
}

- (void)layoutSubviews
{
	GLint backingWidth, backingHeight;
	
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [self.context renderbufferStorage:GL_RENDERBUFFER
		fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,
		&backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,
		&backingHeight);
	
	glViewport(0, 0, backingWidth, backingHeight);
}

- (void)setupGL
{
	EAGLContext *context = (EAGLContext *)self.context;
    [EAGLContext setCurrentContext:context];
	
	glGenFramebuffers(1, &_defaultFramebuffer);
	glGenRenderbuffers(1, &_colorRenderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER, _colorRenderbuffer);

	glClearColor(0.5f, 0.5f, 0.75f, 1.0f);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
	
	glDeleteFramebuffers(1, &_defaultFramebuffer);
	glDeleteRenderbuffers(1, &_colorRenderbuffer);
	
	[EAGLContext setCurrentContext:nil];
}

- (void)drawView:(id)sender
{
	EAGLContext *context = (EAGLContext *)self.context;
    [EAGLContext setCurrentContext:context];
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	[context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
