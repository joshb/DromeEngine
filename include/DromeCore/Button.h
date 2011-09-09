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

#ifndef __DROMECORE_BUTTON_H__
#define __DROMECORE_BUTTON_H__

namespace DromeCore {

/**
 * This enum contains each button that may be used by the IOContext system.
 */
enum Button {
	BTN_NONE = 0,

	// mouse buttons
	BTN_MOUSE1,
	BTN_MOUSE2,
	BTN_MOUSE3,
	BTN_MOUSE_WHEEL_UP,
	BTN_MOUSE_WHEEL_DOWN,

	// top row keys
	BTN_ESCAPE,
	BTN_F1, BTN_F2, BTN_F3, BTN_F4, BTN_F5, BTN_F6,
	BTN_F7, BTN_F8, BTN_F9, BTN_F10, BTN_F11, BTN_F12,
	BTN_SYSRQ,
	BTN_SCROLL_LOCK,
	BTN_PAUSE,

	// number row
	BTN_BACK_QUOTE,
	BTN_1, BTN_2, BTN_3, BTN_4, BTN_5,
	BTN_6, BTN_7, BTN_8, BTN_9, BTN_0,
	BTN_MINUS,
	BTN_EQUALS,
	BTN_BACKSPACE,

	// modifiers/misc keys
	BTN_LSHIFT,
	BTN_RSHIFT,
	BTN_LCONTROL,
	BTN_RCONTROL,
	BTN_LALT,
	BTN_RALT,
	BTN_TAB,
	BTN_CAPS_LOCK,
	BTN_WINDOWS,
	BTN_CONTEXT,
	BTN_ENTER,
	BTN_SPACE,

	// letters
	BTN_A, BTN_B, BTN_C, BTN_D, BTN_E, BTN_F, BTN_G, BTN_H, BTN_I, BTN_J, BTN_K,
	BTN_L, BTN_M, BTN_N, BTN_O, BTN_P, BTN_Q, BTN_R, BTN_S, BTN_T, BTN_U, BTN_V,
	BTN_W, BTN_X, BTN_Y, BTN_Z,

	// punctuation
	BTN_LBRACKET,
	BTN_RBRACKET,
	BTN_BACK_SLASH,
	BTN_FORWARD_SLASH,
	BTN_SEMICOLON,
	BTN_QUOTE,
	BTN_COMMA,
	BTN_PERIOD,

	// insert/home/etc
	BTN_INSERT, BTN_DELETE, BTN_HOME, BTN_END, BTN_PAGE_UP, BTN_PAGE_DOWN,

	// arrows
	BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT,

	// keypad
	BTN_NUM_LOCK,
	BTN_KP_SLASH,
	BTN_KP_ASTERISK,
	BTN_KP_MINUS,
	BTN_KP_PLUS,
	BTN_KP_ENTER,
	BTN_KP_PERIOD,
	BTN_KP_0, BTN_KP_1, BTN_KP_2, BTN_KP_3, BTN_KP_4,
	BTN_KP_5, BTN_KP_6, BTN_KP_7, BTN_KP_8, BTN_KP_9,

	NUM_BUTTONS // should always be last
};

Button buttonFromName(const char *name);
const char *buttonName(Button button);
char buttonAscii(Button button, bool shift);

} // namespace DromeCore

#endif /* __DROMECORE_BUTTON_H__ */
