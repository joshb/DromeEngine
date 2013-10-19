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

#import <DromeCore/Button.h>
#import <DromeGL/OpenGL.h>
#import "DromeNSOpenGLView.h"

using namespace DromeCore;

@implementation DromeNSOpenGLView

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

- (void)prepareOpenGL
{
	glClearColor(0.25f, 0.25f, 0.5f, 1.0f);
	glClearDepth(1.0);
}

- (void)reshape
{
	NSRect frame = [self frame];

	// set viewport
	glViewport(0, 0, (GLsizei)frame.size.width, (GLsizei)frame.size.height);
}

- (BOOL)acceptsFirstResponder
{
	return YES;
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
		case 0x36:	return BTN_RCOMMAND;
		case 0x37:	return BTN_LCOMMAND;
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

- (void)keyDown:(NSEvent *)theEvent
{
	printf("%02x\n", [theEvent keyCode]);
	
	switch([theEvent keyCode]) {
		default:
			break;
	}
}

- (void)flagsChanged:(NSEvent *)theEvent
{
	printf("%02x\n", [theEvent keyCode]);
}

- (void)flush
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(self.eventHandler != NULL)
		self.eventHandler->render();
	
	[[self openGLContext] flushBuffer];
}

@end
