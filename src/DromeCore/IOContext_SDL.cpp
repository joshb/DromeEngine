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

#include <cstdlib>
#include <DromeCore/IOContext_SDL.h>

using namespace std;

namespace DromeCore {

IOContext_SDL::IOContext_SDL()
{
	m_windowWidth = 640;
	m_windowHeight = 480;
	m_fullscreen = false;
	m_windowTitle = "Drome Engine";

	m_initialized = false;
	m_running = false;
	m_grabInput = false;
}

int
IOContext_SDL::getWindowWidth() const
{
	return m_windowWidth;
}

int
IOContext_SDL::getWindowHeight() const
{
	return m_windowHeight;
}

void
IOContext_SDL::setWindowDimensions(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	if(m_initialized) {
		SDL_Surface *screen = setVideoMode();
		if(!screen) {
			fprintf(stderr, "IOContext_SDL::setWindowDimensions(): setVideoMode failed: %s\n", SDL_GetError());
			SDL_Quit();
		}

		if(m_handler)
			m_handler->windowDimensionsChanged(m_windowWidth, m_windowHeight);
	}
}

bool
IOContext_SDL::getFullScreen() const
{
	return m_fullscreen;
}

void
IOContext_SDL::setFullScreen(bool value)
{
	m_fullscreen = value;

	if(m_initialized) {
		SDL_Surface *screen = setVideoMode();
		if(!screen) {
			fprintf(stderr, "IOContext_SDL::setWindowDimensions(): setVideoMode failed: %s\n", SDL_GetError());
			SDL_Quit();
		}
	}
}

string
IOContext_SDL::getWindowTitle() const
{
	return m_windowTitle;
}

void
IOContext_SDL::setWindowTitle(const string &value)
{
	m_windowTitle = value;

	if(m_initialized) {
		if(m_windowTitle.length() > 0)
			SDL_WM_SetCaption(m_windowTitle.c_str(), NULL);
		else
			SDL_WM_SetCaption("Drome Engine", NULL);
	}
}

SDL_Surface *
IOContext_SDL::setVideoMode()
{
	unsigned int flags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
	if(m_fullscreen)
		flags |= SDL_FULLSCREEN;

	return SDL_SetVideoMode(m_windowWidth, m_windowHeight, 32, flags);
}

bool
IOContext_SDL::init()
{
	if(!(SDL_WasInit(0) & SDL_INIT_VIDEO)) {
		if(SDL_Init(SDL_INIT_VIDEO) != 0) {
			fprintf(stderr, "IOContext_SDL::init(): Couldn't initialize SDL: %s\n", SDL_GetError());
			return false;
		}
	}

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Surface *screen = setVideoMode();
	if(!screen) {
		fprintf(stderr, "IOContext_SDL::init(): setVideoMode failed: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	if(m_windowTitle.length() > 0)
		SDL_WM_SetCaption(m_windowTitle.c_str(), NULL);
	else
		SDL_WM_SetCaption("Drome Engine", NULL);

	if(m_grabInput) {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}

	fprintf(stderr, "SDL initialized\n");

	m_initialized = true;
	return true;
}

void
IOContext_SDL::shutdown()
{
	m_running = false;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	fprintf(stderr, "SDL shutdown\n");
}

bool
IOContext_SDL::run()
{
	m_running = true;

	while(m_running) {
		checkInput();
		cycle();
	}

	return true;
}

void
IOContext_SDL::quit()
{
	m_running = false;
}

void *
IOContext_SDL::getProcAddress(const char *functionName) const
{
	return SDL_GL_GetProcAddress(functionName);
}

void
IOContext_SDL::swapBuffers()
{
	if(!m_running)
		return;

	SDL_GL_SwapBuffers();
}

static Button
io_sdl_keysym_to_button(int keysym)
{
	switch(keysym) {
		default:		return BTN_NONE;

		// top row keys
		case SDLK_ESCAPE:   return BTN_ESCAPE;
		case SDLK_F1:       return BTN_F1;
		case SDLK_F2:       return BTN_F2;
		case SDLK_F3:       return BTN_F3;
		case SDLK_F4:       return BTN_F4;
		case SDLK_F5:       return BTN_F5;
		case SDLK_F6:       return BTN_F6;
		case SDLK_F7:       return BTN_F7;
		case SDLK_F8:       return BTN_F8;
		case SDLK_F9:       return BTN_F9;
		case SDLK_F10:      return BTN_F10;
		case SDLK_F11:      return BTN_F11;
		case SDLK_F12:      return BTN_F12;
		case SDLK_SYSREQ:   return BTN_SYSRQ;
		case SDLK_SCROLLOCK:return BTN_SCROLL_LOCK;
		case SDLK_PAUSE:    return BTN_PAUSE;

		// number row
		case SDLK_BACKQUOTE:return BTN_BACK_QUOTE;
		case SDLK_1:	return BTN_1;
		case SDLK_2:	return BTN_2;
		case SDLK_3:	return BTN_3;
		case SDLK_4:	return BTN_4;
		case SDLK_5:	return BTN_5;
		case SDLK_6:	return BTN_6;
		case SDLK_7:	return BTN_7;
		case SDLK_8:	return BTN_8;
		case SDLK_9:	return BTN_9;
		case SDLK_0:	return BTN_0;
		case SDLK_MINUS:    return BTN_MINUS;
		case SDLK_EQUALS:   return BTN_EQUALS;
		case SDLK_BACKSPACE:return BTN_BACKSPACE;

		// modifiers/misc keys
		case SDLK_LSHIFT:   return BTN_LSHIFT;
		case SDLK_RSHIFT:   return BTN_RSHIFT;
		case SDLK_LCTRL:    return BTN_LCONTROL;
		case SDLK_RCTRL:    return BTN_RCONTROL;
		case SDLK_LALT:     return BTN_LALT;
		case SDLK_RALT:     return BTN_RALT;
		case SDLK_TAB:      return BTN_TAB;
		case SDLK_CAPSLOCK: return BTN_CAPS_LOCK;
		case SDLK_MENU:     return BTN_CONTEXT;
		case SDLK_RETURN:   return BTN_ENTER;
		case SDLK_SPACE:    return BTN_SPACE;

		// letters
		case SDLK_a:	return BTN_A;
		case SDLK_b:	return BTN_B;
		case SDLK_c:	return BTN_C;
		case SDLK_d:	return BTN_D;
		case SDLK_e:	return BTN_E;
		case SDLK_f:	return BTN_F;
		case SDLK_g:	return BTN_G;
		case SDLK_h:	return BTN_H;
		case SDLK_i:	return BTN_I;
		case SDLK_j:	return BTN_J;
		case SDLK_k:	return BTN_K;
		case SDLK_l:	return BTN_L;
		case SDLK_m:	return BTN_M;
		case SDLK_n:	return BTN_N;
		case SDLK_o:	return BTN_O;
		case SDLK_p:	return BTN_P;
		case SDLK_q:	return BTN_Q;
		case SDLK_r:	return BTN_R;
		case SDLK_s:	return BTN_S;
		case SDLK_t:	return BTN_T;
		case SDLK_u:	return BTN_U;
		case SDLK_v:	return BTN_V;
		case SDLK_w:	return BTN_W;
		case SDLK_x:	return BTN_X;
		case SDLK_y:	return BTN_Y;
		case SDLK_z:	return BTN_Z;

		// punctuation
		case SDLK_LEFTBRACKET:  return BTN_LBRACKET;
		case SDLK_RIGHTBRACKET: return BTN_RBRACKET;
		case SDLK_BACKSLASH:return BTN_BACK_SLASH;
		case SDLK_SLASH:    return BTN_FORWARD_SLASH;
		case SDLK_SEMICOLON:return BTN_SEMICOLON;
		case SDLK_QUOTE:    return BTN_QUOTE;
		case SDLK_COMMA:    return BTN_COMMA;
		case SDLK_PERIOD:   return BTN_PERIOD;

		// insert/home/etc
		case SDLK_INSERT:   return BTN_INSERT;
		case SDLK_DELETE:   return BTN_DELETE;
		case SDLK_HOME:     return BTN_HOME;
		case SDLK_END:      return BTN_END;
		case SDLK_PAGEUP:   return BTN_PAGE_UP;
		case SDLK_PAGEDOWN: return BTN_PAGE_DOWN;

		// arrows
		case SDLK_UP:       return BTN_UP;
		case SDLK_DOWN:     return BTN_DOWN;
		case SDLK_LEFT:     return BTN_LEFT;
		case SDLK_RIGHT:    return BTN_RIGHT;

		// keypad
		case SDLK_NUMLOCK:  return BTN_NUM_LOCK;
		case SDLK_KP_DIVIDE:return BTN_KP_SLASH;
		case SDLK_KP_MULTIPLY:return BTN_KP_ASTERISK;
		case SDLK_KP_MINUS: return BTN_KP_MINUS;
		case SDLK_KP_PLUS:  return BTN_KP_PLUS;
		case SDLK_KP_ENTER: return BTN_KP_ENTER;
		case SDLK_KP_PERIOD:return BTN_KP_PERIOD;
		case SDLK_KP0:      return BTN_KP_0;
		case SDLK_KP1:      return BTN_KP_1;
		case SDLK_KP2:      return BTN_KP_2;
		case SDLK_KP3:      return BTN_KP_3;
		case SDLK_KP4:      return BTN_KP_4;
		case SDLK_KP5:      return BTN_KP_5;
		case SDLK_KP6:      return BTN_KP_6;
		case SDLK_KP7:      return BTN_KP_7;
		case SDLK_KP8:      return BTN_KP_8;
		case SDLK_KP9:      return BTN_KP_9;
	}
}

static void
io_sdl_button_press(EventHandler *handler, SDL_Event *event)
{
	Button button;

	if(event->type == SDL_KEYDOWN) {
		button = io_sdl_keysym_to_button(event->key.keysym.sym);
	} else { /* mouse button */
		switch(event->button.button) {
			default:
				button = BTN_NONE;
				break;
			case SDL_BUTTON_LEFT:
				button = BTN_MOUSE1;
				break;
			case SDL_BUTTON_RIGHT:
				button = BTN_MOUSE2;
				break;
			case SDL_BUTTON_MIDDLE:
				button = BTN_MOUSE3;
				break;
			case SDL_BUTTON_WHEELUP:
				button = BTN_MOUSE_WHEEL_UP;
				break;
			case SDL_BUTTON_WHEELDOWN:
				button = BTN_MOUSE_WHEEL_DOWN;
				break;
		}
	}

	if(handler)
		handler->buttonPress(button);
}

static void
io_sdl_button_release(EventHandler *handler, SDL_Event *event)
{
	Button button;

	if(event->type == SDL_KEYUP) {
		button = io_sdl_keysym_to_button(event->key.keysym.sym);
	} else { /* mouse button */
		switch(event->button.button) {
			default:
				button = BTN_NONE;
				break;
			case SDL_BUTTON_LEFT:
				button = BTN_MOUSE1;
				break;
			case SDL_BUTTON_RIGHT:
				button = BTN_MOUSE2;
				break;
			case SDL_BUTTON_MIDDLE:
				button = BTN_MOUSE3;
				break;
		}
	}

	if(handler)
		handler->buttonRelease(button);
}

static void
io_sdl_mouse_move(EventHandler *handler, SDL_Event *event,
                  int center_x, int center_y)
{
	if(event->motion.x == center_x && event->motion.y == center_y)
		return;

	if(handler)
		handler->mouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
}

void
IOContext_SDL::checkInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			default:
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_KEYDOWN:
				io_sdl_button_press(m_handler, &event);
				break;
			case SDL_MOUSEBUTTONUP:
			case SDL_KEYUP:
				io_sdl_button_release(m_handler, &event);
				break;
			case SDL_MOUSEMOTION:
				io_sdl_mouse_move(m_handler, &event, m_windowWidth / 2, m_windowHeight / 2);
				break;
		}
	}
}

void
IOContext_SDL::grabMousePointer()
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	m_grabInput = true;
}

void
IOContext_SDL::releaseMousePointer()
{
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	m_grabInput = false;
}

} // namespace DromeCore
