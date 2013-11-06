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

#import <cmath>
#import <sys/time.h>
#import <OpenGL/gl3.h>
#import "CocoaOpenGLAppDelegate.h"
#import "Scene.h"

using namespace DromeCore;

@interface CocoaOpenGLAppDelegate()
{
	NSTimer *_timer;
	Scene *_scene;
}

@end

@implementation CocoaOpenGLAppDelegate

@synthesize window = _window;
@synthesize view = _view;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// do some GL setup
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// initialize the scene
	_scene = new Scene();
	
	// create timer to render the scene at 60fps
    _timer = [NSTimer timerWithTimeInterval:(1.0f / 60.0f)
		target:self
		selector:@selector(timerFireMethod:)
		userInfo:nil
		repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:_timer forMode:NSDefaultRunLoopMode];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

static long
getTicks(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

static float
getSecondsElapsed(void)
{
    static long prevTicks = 0;
	long ticks;
	float secondsElapsed;
    
    // calculate the number of seconds that have
    // passed since the last call to this function
	if(prevTicks == 0)
		prevTicks = getTicks();
	ticks = getTicks();
	secondsElapsed = (float)(ticks - prevTicks) / 1000.0f;
	prevTicks = ticks;
    
    return secondsElapsed;
}

- (void)timerFireMethod:(NSTimer *)theTimer
{
	// render the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _scene->setProjectionMatrix(_view.projectionMatrix);
    _scene->render();
	glFlush();
	[_view flush];
	
	// cycle the scene
	_scene->cycle(getSecondsElapsed());
}

@end
