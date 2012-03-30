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

#include <cstdio>
#include <cstring>
#include <DromeCore/Button.h>
#include <DromeCore/Util.h>

namespace DromeCore {

struct btn_info {
	Button button;
	const char *name;
	char ascii;
	char shift_ascii;
};

#define BTN_INFO(NAME,ASCII,SHIFTASCII) \
	{ NAME, #NAME, ASCII, SHIFTASCII }

static const btn_info btn_info_array[] = {
	BTN_INFO ( BTN_NONE, -1, -1 ),

	// mouse buttons
	BTN_INFO( BTN_MOUSE1, -1, -1 ),
	BTN_INFO( BTN_MOUSE2, -1, -1 ),
	BTN_INFO( BTN_MOUSE3, -1, -1 ),
	BTN_INFO( BTN_MOUSE_WHEEL_UP, -1, -1 ),
	BTN_INFO( BTN_MOUSE_WHEEL_DOWN, -1, -1 ),

	// top row keys
	BTN_INFO( BTN_ESCAPE, -1, -1 ),
	BTN_INFO( BTN_F1, -1, -1 ),
	BTN_INFO( BTN_F2, -1, -1 ),
	BTN_INFO( BTN_F3, -1, -1 ),
	BTN_INFO( BTN_F4, -1, -1 ),
	BTN_INFO( BTN_F5, -1, -1 ),
	BTN_INFO( BTN_F6, -1, -1 ),
	BTN_INFO( BTN_F7, -1, -1 ),
	BTN_INFO( BTN_F8, -1, -1 ),
	BTN_INFO( BTN_F9, -1, -1 ),
	BTN_INFO( BTN_F10, -1, -1 ),
	BTN_INFO( BTN_F11, -1, -1 ),
	BTN_INFO( BTN_F12, -1, -1 ),
	BTN_INFO( BTN_SYSRQ, -1, -1 ),
	BTN_INFO( BTN_SCROLL_LOCK, -1, -1 ),
	BTN_INFO( BTN_PAUSE, -1, -1 ),

	// number row
	BTN_INFO( BTN_BACK_QUOTE, '`', '~' ),
	BTN_INFO( BTN_1, '1', '!' ),
	BTN_INFO( BTN_2, '2', '@' ),
	BTN_INFO( BTN_3, '3', '#' ),
	BTN_INFO( BTN_4, '4', '$' ),
	BTN_INFO( BTN_5, '5', '%' ),
	BTN_INFO( BTN_6, '6', '^' ),
	BTN_INFO( BTN_7, '7', '&' ),
	BTN_INFO( BTN_8, '8', '*' ),
	BTN_INFO( BTN_9, '9', '(' ),
	BTN_INFO( BTN_0, '0', ')' ),
	BTN_INFO( BTN_MINUS, '-', '_' ),
	BTN_INFO( BTN_EQUALS, '=', '+' ),
	BTN_INFO( BTN_BACKSPACE, -1, -1 ),

	// modifiers/misc keys
	BTN_INFO( BTN_LSHIFT, -1, -1 ),
	BTN_INFO( BTN_RSHIFT, -1, -1 ),
	BTN_INFO( BTN_LCONTROL, -1, -1 ),
	BTN_INFO( BTN_RCONTROL, -1, -1 ),
	BTN_INFO( BTN_LALT, -1, -1 ),
	BTN_INFO( BTN_RALT, -1, -1 ),
	BTN_INFO( BTN_TAB, -1, -1 ),
	BTN_INFO( BTN_CAPS_LOCK, -1, -1 ),
	BTN_INFO( BTN_LCOMMAND, -1, -1 ),
	BTN_INFO( BTN_RCOMMAND, -1, -1 ),
	BTN_INFO( BTN_CONTEXT, -1, -1 ),
	BTN_INFO( BTN_ENTER, -1, -1 ),
	BTN_INFO( BTN_SPACE, ' ', ' ' ),

	// letters
	BTN_INFO( BTN_A, 'a', 'A' ),
	BTN_INFO( BTN_B, 'b', 'B' ),
	BTN_INFO( BTN_C, 'c', 'C' ),
	BTN_INFO( BTN_D, 'd', 'D' ),
	BTN_INFO( BTN_E, 'e', 'E' ),
	BTN_INFO( BTN_F, 'f', 'F' ),
	BTN_INFO( BTN_G, 'g', 'G' ),
	BTN_INFO( BTN_H, 'h', 'H' ),
	BTN_INFO( BTN_I, 'i', 'I' ),
	BTN_INFO( BTN_J, 'j', 'J' ),
	BTN_INFO( BTN_K, 'k', 'K' ),
	BTN_INFO( BTN_L, 'l', 'L' ),
	BTN_INFO( BTN_M, 'm', 'M' ),
	BTN_INFO( BTN_N, 'n', 'N' ),
	BTN_INFO( BTN_O, 'o', 'O' ),
	BTN_INFO( BTN_P, 'p', 'P' ),
	BTN_INFO( BTN_Q, 'q', 'Q' ),
	BTN_INFO( BTN_R, 'r', 'R' ),
	BTN_INFO( BTN_S, 's', 'S' ),
	BTN_INFO( BTN_T, 't', 'T' ),
	BTN_INFO( BTN_U, 'u', 'U' ),
	BTN_INFO( BTN_V, 'v', 'V' ),
	BTN_INFO( BTN_W, 'w', 'W' ),
	BTN_INFO( BTN_X, 'x', 'X' ),
	BTN_INFO( BTN_Y, 'y', 'Y' ),
	BTN_INFO( BTN_Z, 'z', 'Z' ),

	// punctuation
	BTN_INFO( BTN_LBRACKET, '[', '{' ),
	BTN_INFO( BTN_RBRACKET, ']', '}' ),
	BTN_INFO( BTN_BACK_SLASH, '\\', '|' ),
	BTN_INFO( BTN_FORWARD_SLASH, '/', '?' ),
	BTN_INFO( BTN_SEMICOLON, ';', ':' ),
	BTN_INFO( BTN_QUOTE, '\'', '"' ),
	BTN_INFO( BTN_COMMA, ',', '<' ),
	BTN_INFO( BTN_PERIOD, '.', '>' ),

	// insert/home/etc
	BTN_INFO( BTN_INSERT, -1, -1 ),
	BTN_INFO( BTN_DELETE, -1, -1 ),
	BTN_INFO( BTN_HOME, -1, -1 ),
	BTN_INFO( BTN_END, -1, -1 ),
	BTN_INFO( BTN_PAGE_UP, -1, -1 ),
	BTN_INFO( BTN_PAGE_DOWN, -1, -1 ),

	// arrows
	BTN_INFO( BTN_UP, -1, -1 ),
	BTN_INFO( BTN_DOWN, -1, -1 ),
	BTN_INFO( BTN_LEFT, -1, -1 ),
	BTN_INFO( BTN_RIGHT, -1, -1 ),

	// keypad
	BTN_INFO( BTN_NUM_LOCK, -1, -1 ),
	BTN_INFO( BTN_KP_SLASH, '/', '/' ),
	BTN_INFO( BTN_KP_ASTERISK, '*', '*' ),
	BTN_INFO( BTN_KP_MINUS, '-', '-' ),
	BTN_INFO( BTN_KP_PLUS, '+', '+' ),
	BTN_INFO( BTN_KP_ENTER, -1, -1 ),
	BTN_INFO( BTN_KP_PERIOD, '.', '.' ),
	BTN_INFO( BTN_KP_0, '0', '0' ),
	BTN_INFO( BTN_KP_1, '1', '1' ),
	BTN_INFO( BTN_KP_2, '2', '2' ),
	BTN_INFO( BTN_KP_3, '3', '3' ),
	BTN_INFO( BTN_KP_4, '4', '4' ),
	BTN_INFO( BTN_KP_5, '5', '5' ),
	BTN_INFO( BTN_KP_6, '6', '6' ),
	BTN_INFO( BTN_KP_7, '7', '7' ),
	BTN_INFO( BTN_KP_8, '8', '8' ),
	BTN_INFO( BTN_KP_9, '9', '9' )
};
static const unsigned int btn_info_array_size = sizeof(btn_info_array) / sizeof(btn_info_array[0]);

Button
buttonFromName(const char *name)
{
	for(unsigned int i = 0; i < btn_info_array_size; i++) {
		if(strCaseCmp(name, btn_info_array[i].name) == 0)
			return btn_info_array[i].button;
	}

	return BTN_NONE;
}

const char *
buttonName(Button button)
{
	for(unsigned int i = 0; i < btn_info_array_size; i++) {
		if(btn_info_array[i].button == button)
			return btn_info_array[i].name;
	}
}

// return ASCII value of given button, or -1 if not an ASCII key
char
buttonAscii(Button button, bool shift)
{
	char c = -1;

	for(unsigned int i = 0; i < btn_info_array_size; i++) {
		if(btn_info_array[i].button == button) {
			c = shift ? btn_info_array[i].shift_ascii :
			            btn_info_array[i].ascii;
			break;
		}
	}

	return c;
}

} // namespace DromeCore
