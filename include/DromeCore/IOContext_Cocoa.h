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

#ifndef __DROMECORE_IOCONTEXT_COCOA_H__
#define __DROMECORE_IOCONTEXT_COCOA_H__

#include <Cocoa/Cocoa.h>
#include "IOContext.h"

namespace DromeCore {
	class IOContext_Cocoa;
} // namespace DromeCore

/*
 * DromeAppDelegate interface
 */
@interface DromeAppDelegate : NSObject <NSApplicationDelegate>
{
	DromeCore::IOContext_Cocoa *io;
}

@property DromeCore::IOContext_Cocoa *io;

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)app;
@end

/*
 * DromeWindowDelegate interface
 */
@interface DromeWindowDelegate : NSObject <NSWindowDelegate>
{
	DromeCore::IOContext_Cocoa *io;
}

@property DromeCore::IOContext_Cocoa *io;

- (BOOL) windowShouldClose:(id)sender;
@end

/*
 * DromeGLView interface
 */
@interface DromeGLView : NSOpenGLView
{
	DromeCore::IOContext_Cocoa *io;
}

@property DromeCore::IOContext_Cocoa *io;

- (BOOL) acceptsFirstResponder;

- (void) timerFireMethod:(NSTimer *)theTimer;
- (void) keyDown:(NSEvent *)theEvent;
- (void) keyUp:(NSEvent *)theEvent;
- (void) flagsChanged:(NSEvent *)theEvent;
- (void) mouseDown:(NSEvent *)theEvent;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) rightMouseDown:(NSEvent *)theEvent;
- (void) rightMouseUp:(NSEvent *)theEvent;
- (void) otherMouseDown:(NSEvent *)theEvent;
- (void) otherMouseUp:(NSEvent *)theEvent;
- (void) scrollWheel:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) rightMouseDragged:(NSEvent *)theEvent;
- (void) otherMouseDragged:(NSEvent *)theEvent;
- (void) mouseMoved:(NSEvent *)theEvent;
@end

namespace DromeCore {

/*
 * IOContext_Cocoa
 */
class IOContext_Cocoa : public IOContext {
	protected:
		NSAutoreleasePool *m_pool;
		NSWindow *m_window;
		DromeGLView *m_view;
		NSTimer *m_timer;

		int m_windowWidth, m_windowHeight;
		bool m_fullscreen;
		std::string m_windowTitle;

		bool m_grabMousePointer;

	public:
		IOContext_Cocoa();
		~IOContext_Cocoa();

		int getWindowWidth() const;
		int getWindowHeight() const;
		void setWindowDimensions(int width, int height);
		bool getFullScreen() const;
		void setFullScreen(bool value);
		std::string getWindowTitle() const;
		void setWindowTitle(const std::string &value);

		bool init();
		void shutdown();
		bool run();

		void *getProcAddress(const char *functionName) const;
		void swapBuffers();
		void grabMousePointer();
		void releaseMousePointer();

		void cocoaTimerFireMethod(NSTimer *theTimer);
		void cocoaKeyDown(NSEvent *theEvent);
		void cocoaKeyUp(NSEvent *theEvent);
		void cocoaMouseDown(NSEvent *theEvent);
		void cocoaMouseUp(NSEvent *theEvent);
		void cocoaScrollWheel(NSEvent *theEvent);
		void cocoaMouseMoved(NSEvent *theEvent);
};

} // namespace DromeCore

#endif /* __DROMECORE_IOCONTEXT_COCOA_H__ */
