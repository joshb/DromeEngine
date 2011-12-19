/*
 * Copyright (C) 2010-2011 Josh A. Beam
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

/*
 * IOContext_Cocoa
 */
class IOContext_Cocoa : public IOContext {
	protected:
		NSWindow *m_window;
		NSOpenGLView *m_view;

		int m_windowWidth, m_windowHeight;
		bool m_fullscreen;
		std::string m_windowTitle;

		bool m_grabMousePointer;

	public:
		IOContext_Cocoa(NSWindow *window, NSOpenGLView *view);
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

		void cocoaKeyDown(NSEvent *theEvent);
		void cocoaKeyUp(NSEvent *theEvent);
		void cocoaMouseDown(NSEvent *theEvent);
		void cocoaMouseUp(NSEvent *theEvent);
		void cocoaScrollWheel(NSEvent *theEvent);
		void cocoaMouseMoved(NSEvent *theEvent);
		void cocoaReshape();
};

} // namespace DromeCore

/*
 * DromeNSWindow interface
 */
@interface DromeNSWindow : NSWindow
{
	DromeCore::IOContext_Cocoa *io;
}

@property DromeCore::IOContext_Cocoa *io;

@end

/*
 * DromeNSOpenGLView interface
 */
@interface DromeNSOpenGLView : NSOpenGLView
{
	DromeCore::IOContext_Cocoa *io;
	NSTrackingArea *trackingArea;
}

@property DromeCore::IOContext_Cocoa *io;

@end

#endif /* __DROMECORE_IOCONTEXT_COCOA_H__ */
