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

#include <mach-o/dyld.h>
#include <Cocoa/Cocoa.h>
#include <DromeCore/IOContext.h>

using namespace std;

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
		string m_windowTitle;

		bool m_grabMousePointer;

	public:
		IOContext_Cocoa();
		~IOContext_Cocoa();

		int getWindowWidth() const;
		int getWindowHeight() const;
		void setWindowDimensions(int width, int height);
		bool getFullScreen() const;
		void setFullScreen(bool value);
		string getWindowTitle() const;
		void setWindowTitle(const string &value);

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

IOContext_Cocoa::IOContext_Cocoa()
{
	m_window = nil;
	m_view = nil;

	m_windowWidth = 640;
	m_windowHeight = 480;
	m_fullscreen = false;
	m_windowTitle = "Drome Engine";

	m_grabMousePointer = false;

	// create autorelease pool
	m_pool = [[NSAutoreleasePool alloc] init];
}

IOContext_Cocoa::~IOContext_Cocoa()
{
	shutdown();
}

int
IOContext_Cocoa::getWindowWidth() const
{
	return m_windowWidth;
}

int
IOContext_Cocoa::getWindowHeight() const
{
	return m_windowHeight;
}

void
IOContext_Cocoa::setWindowDimensions(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	if(m_window != nil) {
		// resize the window
		NSSize size;
		size.width = (float)m_windowWidth;
		size.height = (float)m_windowHeight;
		[m_window setContentSize: size];
		[m_window center];

		if(m_handler)
			m_handler->windowDimensionsChanged(m_windowWidth, m_windowHeight);
	}
}

bool
IOContext_Cocoa::getFullScreen() const
{
	return m_fullscreen;
}

void
IOContext_Cocoa::setFullScreen(bool value)
{
	m_fullscreen = value;
}

string
IOContext_Cocoa::getWindowTitle() const
{
	return m_windowTitle;
}

void
IOContext_Cocoa::setWindowTitle(const string &value)
{
	m_windowTitle = value;

	if(m_window != nil)
		[m_window setTitle:[[NSString alloc] initWithCString: m_windowTitle.c_str() encoding: NSASCIIStringEncoding]];
}

bool
IOContext_Cocoa::init()
{
	[NSApplication sharedApplication];

	// set application delegate
	DromeAppDelegate *appDelegate = [DromeAppDelegate alloc];
	appDelegate.io = this;
	[NSApp setDelegate: appDelegate];

	NSRect viewRect;
	NSUInteger style;

	if(false && m_fullscreen) {
		viewRect = [[NSScreen mainScreen] frame];
		style = NSBorderlessWindowMask;
	} else {
		viewRect = NSMakeRect(0, 0, m_windowWidth, m_windowHeight);
		style = NSTitledWindowMask | NSClosableWindowMask |
		        NSMiniaturizableWindowMask;
	}

	// create window
	m_window = [NSWindow alloc];
	[m_window initWithContentRect: viewRect
	          styleMask: style
	          backing: NSBackingStoreBuffered
	          defer: YES];
	[m_window setAcceptsMouseMovedEvents: YES];

	// set window delegate
	DromeWindowDelegate *windowDelegate = [DromeWindowDelegate alloc];
	windowDelegate.io = this;
	[m_window setDelegate: windowDelegate];

	if(false && m_fullscreen) {
		[m_window setLevel: NSMainMenuWindowLevel + 1];
		[m_window setOpaque: YES];
		[m_window setHidesOnDeactivate: YES];
	} else {
		[m_window center];
	}

	// create NSOpenGLContext
	NSOpenGLPixelFormatAttribute attributes[] = {
//		NSOpenGLPFAFullScreen,
//		NSOpenGLPFAScreenMask,
//		CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFADepthSize, 32,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		0
	};
	NSOpenGLPixelFormat *pixelFormat = [NSOpenGLPixelFormat alloc];
	[pixelFormat initWithAttributes: attributes];
	NSOpenGLContext *glContext = [NSOpenGLContext alloc];
	[glContext initWithFormat: pixelFormat
	           shareContext: nil];
	[pixelFormat release];


	// create DromeGLView
	//m_view = [[DromeGLView alloc] initWithFrame: viewRect
	                              //pixelFormat: pixelFormat];
	m_view = [DromeGLView alloc];
	[m_view initWithFrame: viewRect
	        pixelFormat: [NSOpenGLView defaultPixelFormat]];
	m_view.io = this;
	[m_view setOpenGLContext: glContext];
	[m_window setContentView: m_view];

	// set window title
	[m_window setTitle:[[NSString alloc] initWithCString:m_windowTitle.c_str() encoding:NSASCIIStringEncoding]];

	// show window
	[m_window makeKeyAndOrderFront: nil];

	[glContext clearDrawable];
	[glContext setView: m_view];
	[glContext makeCurrentContext];
	[glContext release];

	// create timer
	m_timer = [NSTimer timerWithTimeInterval: (1.0f / 240.0f)
	                   target: m_view
	                   selector:@selector(timerFireMethod:)
	                   userInfo:nil
	                   repeats: YES];
	[[NSRunLoop currentRunLoop] addTimer: m_timer forMode: NSDefaultRunLoopMode];

	return true;
}

void
IOContext_Cocoa::shutdown()
{
	if(m_view == nil)
		return;

	[m_timer invalidate];
	[m_timer release];

	[NSApp stop: nil];

	[m_view release];
	m_view = nil;

	[m_window release];
	m_window = nil;
}

bool
IOContext_Cocoa::run()
{
	[NSApp run];
	return true;
}

void *
IOContext_Cocoa::getProcAddress(const char *functionName) const
{
	string name = string("_") + functionName;

	if(NSIsSymbolNameDefined(name.c_str()))
		return NSAddressOfSymbol(NSLookupAndBindSymbol(name.c_str()));

	return NULL;
}

void
IOContext_Cocoa::swapBuffers()
{
	[[m_view openGLContext] flushBuffer];
}

void
IOContext_Cocoa::grabMousePointer()
{
	m_grabMousePointer = true;
	CGDisplayHideCursor(kCGDirectMainDisplay);

	// warp mouse to center of window
	NSRect rect = [m_window frame];
	CGPoint point;
	point.x = rect.origin.x + rect.size.width / 2;
	point.y = rect.origin.y + rect.size.height / 2;
	CGWarpMouseCursorPosition(point);
}

void
IOContext_Cocoa::releaseMousePointer()
{
	m_grabMousePointer = false;
	CGDisplayShowCursor(kCGDirectMainDisplay);
}

void
IOContext_Cocoa::cocoaTimerFireMethod(NSTimer *theTimer)
{
	cycle();
}

static Button
keyCodeToButton(unsigned short keyCode)
{
	switch(keyCode) {
		default:	return BTN_NONE;

		// top row keys
		case 0x35:	return BTN_ESCAPE;
		case 0x7A:	return BTN_F1;
		case 0x78:	return BTN_F2;
		case 0x63:	return BTN_F3;
		case 0x76:	return BTN_F4;
		case 0x60:	return BTN_F5;
		case 0x61:	return BTN_F6;
		case 0x62:	return BTN_F7;
		case 0x64:	return BTN_F8;
		case 0x65:	return BTN_F9;
		case 0x6D:	return BTN_F10;
		case 0x67:	return BTN_F11;
		case 0x6F:	return BTN_F12;
		case 0x69:	return BTN_SYSRQ;

		// number row
		case 0x32:	return BTN_BACK_QUOTE;
		case 0x12:	return BTN_1;
		case 0x13:	return BTN_2;
		case 0x14:	return BTN_3;
		case 0x15:	return BTN_4;
		case 0x17:	return BTN_5;
		case 0x16:	return BTN_6;
		case 0x1A:	return BTN_7;
		case 0x1C:	return BTN_8;
		case 0x19:	return BTN_9;
		case 0x1D:	return BTN_0;
		case 0x1B:	return BTN_MINUS;
		case 0x18:	return BTN_EQUALS;
		case 0x33:	return BTN_BACKSPACE;

		// modifiers/misc keys
		case 0x38:	return BTN_LSHIFT;
		case 0x3C:	return BTN_RSHIFT;
		case 0x3B:	return BTN_LCONTROL;
		case 0x3E:	return BTN_RCONTROL;
		case 0x3A:	return BTN_LALT;
		case 0x3D:	return BTN_RALT;
		case 0x30:	return BTN_TAB;
		case 0x39:	return BTN_CAPS_LOCK;
		case 0x36:
		case 0x37:	return BTN_WINDOWS;
		case 0x6E:	return BTN_CONTEXT;
		case 0x24:	return BTN_ENTER;
		case 0x31:	return BTN_SPACE;

		// letters
		case 0x00:	return BTN_A;
		case 0x0B:	return BTN_B;
		case 0x08:	return BTN_C;
		case 0x02:	return BTN_D;
		case 0x0E:	return BTN_E;
		case 0x03:	return BTN_F;
		case 0x05:	return BTN_G;
		case 0x04:	return BTN_H;
		case 0x22:	return BTN_I;
		case 0x26:	return BTN_J;
		case 0x28:	return BTN_K;
		case 0x25:	return BTN_L;
		case 0x2E:	return BTN_M;
		case 0x2D:	return BTN_N;
		case 0x1F:	return BTN_O;
		case 0x23:	return BTN_P;
		case 0x0C:	return BTN_Q;
		case 0x0F:	return BTN_R;
		case 0x01:	return BTN_S;
		case 0x11:	return BTN_T;
		case 0x20:	return BTN_U;
		case 0x09:	return BTN_V;
		case 0x0D:	return BTN_W;
		case 0x07:	return BTN_X;
		case 0x10:	return BTN_Y;
		case 0x06:	return BTN_Z;

		// punctuation
		case 0x21:	return BTN_LBRACKET;
		case 0x1E:	return BTN_RBRACKET;
		case 0x2A:	return BTN_BACK_SLASH;
		case 0x2C:	return BTN_FORWARD_SLASH;
		case 0x29:	return BTN_SEMICOLON;
		case 0x27:	return BTN_QUOTE;
		case 0x2B:	return BTN_COMMA;
		case 0x2F:	return BTN_PERIOD;

		// insert/home/etc
		case 0x75:	return BTN_DELETE;
		case 0x73:	return BTN_HOME;
		case 0x77:	return BTN_END;
		case 0x74:	return BTN_PAGE_UP;
		case 0x79:	return BTN_PAGE_DOWN;

		// arrows
		case 0x7E:	return BTN_UP;
		case 0x7D:	return BTN_DOWN;
		case 0x7B:	return BTN_LEFT;
		case 0x7C:	return BTN_RIGHT;

		// keypad
		case 0x47:	return BTN_NUM_LOCK;
		case 0x4B:	return BTN_KP_SLASH;
		case 0x43:	return BTN_KP_ASTERISK;
		case 0x4E:	return BTN_KP_MINUS;
		case 0x45:	return BTN_KP_PLUS;
		case 0x4C:	return BTN_KP_ENTER;
		case 0x41:	return BTN_KP_PERIOD;
		case 0x52:	return BTN_KP_0;
		case 0x53:	return BTN_KP_1;
		case 0x54:	return BTN_KP_2;
		case 0x55:	return BTN_KP_3;
		case 0x56:	return BTN_KP_4;
		case 0x57:	return BTN_KP_5;
		case 0x58:	return BTN_KP_6;
		case 0x59:	return BTN_KP_7;
		case 0x5B:	return BTN_KP_8;
		case 0x5C:	return BTN_KP_9;
	}
}

void
IOContext_Cocoa::cocoaKeyDown(NSEvent *theEvent)
{
	if(!m_handler)
		return;

	Button button = keyCodeToButton([theEvent keyCode]);
	if(button != BTN_NONE)
		m_handler->buttonPress(button);
}

void
IOContext_Cocoa::cocoaKeyUp(NSEvent *theEvent)
{
	if(!m_handler)
		return;

	Button button = keyCodeToButton([theEvent keyCode]);
	if(button != BTN_NONE)
		m_handler->buttonRelease(button);
}

static Button
mouseButtonNumberToButton(NSInteger buttonNumber)
{
	switch(buttonNumber) {
		default:	return BTN_NONE;
		case 0:		return BTN_MOUSE1;
		case 1:		return BTN_MOUSE2;
		case 2:		return BTN_MOUSE3;
	}
}

void
IOContext_Cocoa::cocoaMouseDown(NSEvent *theEvent)
{
	if(!m_handler)
		return;

	Button button = mouseButtonNumberToButton([theEvent buttonNumber]);
	if(button != BTN_NONE)
		m_handler->buttonPress(BTN_MOUSE1);
}

void
IOContext_Cocoa::cocoaMouseUp(NSEvent *theEvent)
{
	if(!m_handler)
		return;

	Button button = mouseButtonNumberToButton([theEvent buttonNumber]);
	if(button != BTN_NONE)
		m_handler->buttonRelease(button);
}

void
IOContext_Cocoa::cocoaScrollWheel(NSEvent *theEvent)
{
	if(!m_handler)
		return;

	Button button = ([theEvent deltaY] > 0.0f) ? BTN_MOUSE_WHEEL_UP : BTN_MOUSE_WHEEL_DOWN;
	m_handler->buttonPress(button);
	m_handler->buttonRelease(button);
}

void
IOContext_Cocoa::cocoaMouseMoved(NSEvent *theEvent)
{
	if(m_grabMousePointer) {
		// warp mouse to center of window
		NSRect rect = [m_window frame];
		CGPoint point;
		point.x = rect.origin.x + rect.size.width / 2;
		point.y = rect.origin.y + rect.size.height / 2;
		CGWarpMouseCursorPosition(point);
	}

	if(!m_handler)
		return;

	NSPoint point = [m_window mouseLocationOutsideOfEventStream];
	m_handler->mouseMove((int)point.x, m_windowHeight - (int)point.y, (int)[theEvent deltaX], (int)[theEvent deltaY]);
}

IOContext *
io_cocoa_new_context()
{
	return new IOContext_Cocoa();
}

} // namespace DromeCore

/*
 * DromeAppDelegate implementation
 */
@implementation DromeAppDelegate
@synthesize io;

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)app
{
	io->shutdown();
	return NSTerminateCancel;
}
@end

/*
 * DromeWindowDelegate implementation
 */
@implementation DromeWindowDelegate
@synthesize io;

- (BOOL) windowShouldClose:(id)sender
{
	io->shutdown();
	return NO;
}
@end

/*
 * DromeGLView implementation
 */
@implementation DromeGLView
@synthesize io;

- (BOOL) acceptsFirstResponder
{
	return YES;
}

- (void) timerFireMethod:(NSTimer *)theTimer
{
	io->cocoaTimerFireMethod(theTimer);
}

- (void) keyDown:(NSEvent *)theEvent
{
	io->cocoaKeyDown(theEvent);
}

- (void) keyUp:(NSEvent *)theEvent
{
	io->cocoaKeyUp(theEvent);
}

- (void) flagsChanged:(NSEvent *)theEvent
{
	io->cocoaKeyDown(theEvent);
}

- (void) mouseDown:(NSEvent *)theEvent
{
	io->cocoaMouseDown(theEvent);
}

- (void) mouseUp:(NSEvent *)theEvent
{
	io->cocoaMouseUp(theEvent);
}

- (void) rightMouseDown:(NSEvent *)theEvent
{
	io->cocoaMouseDown(theEvent);
}

- (void) rightMouseUp:(NSEvent *)theEvent
{
	io->cocoaMouseUp(theEvent);
}

- (void) otherMouseDown:(NSEvent *)theEvent
{
	io->cocoaMouseDown(theEvent);
}

- (void) otherMouseUp:(NSEvent *)theEvent
{
	io->cocoaMouseUp(theEvent);
}

- (void) scrollWheel:(NSEvent *)theEvent
{
	io->cocoaScrollWheel(theEvent);
}

- (void) mouseDragged:(NSEvent *)theEvent
{
	io->cocoaMouseMoved(theEvent);
}

- (void) rightMouseDragged:(NSEvent *)theEvent
{
	io->cocoaMouseMoved(theEvent);
}

- (void) otherMouseDragged:(NSEvent *)theEvent
{
	io->cocoaMouseMoved(theEvent);
}

- (void) mouseMoved:(NSEvent *)theEvent
{
	io->cocoaMouseMoved(theEvent);
}
@end
