/*
 * Copyright (C) 2011 Josh A. Beam
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

#import "DromeDemoAppDelegate.h"

using namespace DromeCore;

@implementation DromeDemoAppDelegate

@synthesize window;
@synthesize view;

- (id)init
{
	if(self = [super init]) {
		io = NULL;
		handler = NULL;
	}
	
	return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// create IOContext and MyEventHandler instances
	try {
		io = new IOContext_Cocoa(window, view);
		handler = new MyEventHandler(io);
	} catch(Exception ex) {
		printf("EXCEPTION: %s\n", ex.toString().c_str());
		[window close];
		return;
	}
	
	// create timer to render the scene at 60fps
    timer = [NSTimer timerWithTimeInterval: (1.0f / 60.0f)
		target: self
		selector: @selector(timerFireMethod:)
		userInfo: nil
		repeats: YES];
	[[NSRunLoop currentRunLoop] addTimer: timer forMode: NSDefaultRunLoopMode];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	if(handler != NULL)
		delete handler;
	if(io != NULL)
		delete io;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return YES;
}

- (void)timerFireMethod:(NSTimer *)theTimer
{
	io->cycle();
}

@end
