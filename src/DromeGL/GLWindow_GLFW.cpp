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

#include <iostream>
#include <DromeCore/Exception.h>
#include <DromeGL/OpenGL.h>
#include <DromeGL/GLWindow_GLFW.h>
#include <DromeMath/DromeMath>
#include <GL/glfw.h>

//#define GLWINDOW_DEBUG

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

static GLWindow_GLFW *g_instance = NULL;

GLWindow_GLFW::GLWindow_GLFW()
{
	if(g_instance != NULL)
		throw Exception("GLWindow_GLFW::GLWindow_GLFW(): Another instance of this class already exists");

	g_instance = this;

	m_width = 640;
	m_height = 480;
	m_fullscreen = false;
	m_title = "Drome Engine";
	m_eventHandler = NULL;

	m_grabMouse = false;
	m_lastTime = 0.0f;

	// initialize GLFW
	if(glfwInit() == GL_FALSE) {
		g_instance = NULL;
		throw Exception("GLWindow_GLFW::GLWindow_GLFW(): glfwInit failed");
	}
}

GLWindow_GLFW::~GLWindow_GLFW()
{
	// close the window if necessary and terminate
	close();
	glfwTerminate();

	g_instance = NULL;
}

bool
GLWindow_GLFW::isOpen() const
{
	return (glfwGetWindowParam(GLFW_OPENED) == GL_TRUE);
}

void
GLWindow_GLFW::setDimensions(int width, int height)
{
	if(width != m_width || m_height != height) {
		m_width = width;
		m_height = height;

		// close and reopen the window if necessary
		if(isOpen()) {
			close();
			open();
		}
	}
}

int
GLWindow_GLFW::getWidth() const
{
	return m_width;
}

int
GLWindow_GLFW::getHeight() const
{
	return m_height;
}

void
GLWindow_GLFW::setFullScreen(bool value)
{
	if(value != m_fullscreen) {
		m_fullscreen = value;

		// close and reopen the window if necessary
		if(isOpen()) {
			close();
			open();
		}
	}
}

bool
GLWindow_GLFW::getFullScreen() const
{
	return m_fullscreen;
}

void
GLWindow_GLFW::setTitle(const char *value)
{
	m_title = value;
	if(isOpen())
		glfwSetWindowTitle(value);
}

void
GLWindow_GLFW::setEventHandler(EventHandler *value)
{
	m_eventHandler = value;
	if(m_eventHandler != NULL)
		m_eventHandler->windowDimensionsChanged(m_width, m_height);
}

void
GLWindow_GLFW::setGrabMouse(bool value)
{
	if(value != m_grabMouse) {
		m_grabMouse = value;

		if(isOpen()) {
			if(m_grabMouse) {
				m_lastMouseX = m_width / 2;
				m_lastMouseY = m_height / 2;

				glfwDisable(GLFW_MOUSE_CURSOR);
				glfwSetMousePos(m_lastMouseX, m_lastMouseY);
			} else {
				glfwEnable(GLFW_MOUSE_CURSOR);
			}
		}
	}
}

bool
GLWindow_GLFW::getGrabMouse() const
{
	return m_grabMouse;
}

static void GLFWCALL
windowSizeCallback(int width, int height)
{
	g_instance->windowSizeChanged(width, height);
}

void
GLWindow_GLFW::windowSizeChanged(int width, int height)
{
	m_width = width;
	m_height = height;

	// resize the viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	if(m_eventHandler != NULL)
		m_eventHandler->windowDimensionsChanged(width, height);
}

static int GLFWCALL
windowCloseCallback()
{
	return GL_TRUE;
}

static Button
keyToButton(int key)
{
	switch(key) {
		default: return BTN_NONE;

		// top row keys
		case GLFW_KEY_ESC: return BTN_ESCAPE;
		case GLFW_KEY_F1:  return BTN_F1;
		case GLFW_KEY_F2:  return BTN_F2;
		case GLFW_KEY_F3:  return BTN_F3;
		case GLFW_KEY_F4:  return BTN_F4;
		case GLFW_KEY_F5:  return BTN_F5;
		case GLFW_KEY_F6:  return BTN_F6;
		case GLFW_KEY_F7:  return BTN_F7;
		case GLFW_KEY_F8:  return BTN_F8;
		case GLFW_KEY_F9:  return BTN_F9;
		case GLFW_KEY_F10: return BTN_F10;
		case GLFW_KEY_F11: return BTN_F11;
		case GLFW_KEY_F12: return BTN_F12;
		case GLFW_KEY_SCROLL_LOCK: return BTN_SCROLL_LOCK;
		case GLFW_KEY_PAUSE: return BTN_PAUSE;

		// number row
		case '`': return BTN_BACK_QUOTE;
		case '1': return BTN_1;
		case '2': return BTN_2;
		case '3': return BTN_3;
		case '4': return BTN_4;
		case '5': return BTN_5;
		case '6': return BTN_6;
		case '7': return BTN_7;
		case '8': return BTN_8;
		case '9': return BTN_9;
		case '0': return BTN_0;
		case '-': return BTN_MINUS;
		case '=': return BTN_EQUALS;
		case GLFW_KEY_BACKSPACE: return BTN_BACKSPACE;

		// modifiers/misc keys
		case GLFW_KEY_LSHIFT: return BTN_LSHIFT;
		case GLFW_KEY_RSHIFT: return BTN_RSHIFT;
		case GLFW_KEY_LCTRL:  return BTN_LCONTROL;
		case GLFW_KEY_RCTRL:  return BTN_RCONTROL;
		case GLFW_KEY_LALT:   return BTN_LALT;
		case GLFW_KEY_RALT:   return BTN_RALT;
		case GLFW_KEY_TAB:    return BTN_TAB;
		case GLFW_KEY_CAPS_LOCK: return BTN_CAPS_LOCK;
		case GLFW_KEY_LSUPER: return BTN_LCOMMAND;
		case GLFW_KEY_RSUPER: return BTN_RCOMMAND;
		case GLFW_KEY_MENU:   return BTN_CONTEXT;
		case GLFW_KEY_ENTER:  return BTN_ENTER;
		case GLFW_KEY_SPACE:  return BTN_SPACE;

		// letters
		case 'A': return BTN_A;
		case 'B': return BTN_B;
		case 'C': return BTN_C;
		case 'D': return BTN_D;
		case 'E': return BTN_E;
		case 'F': return BTN_F;
		case 'G': return BTN_G;
		case 'H': return BTN_H;
		case 'I': return BTN_I;
		case 'J': return BTN_J;
		case 'K': return BTN_K;
		case 'L': return BTN_L;
		case 'M': return BTN_M;
		case 'N': return BTN_N;
		case 'O': return BTN_O;
		case 'P': return BTN_P;
		case 'Q': return BTN_Q;
		case 'R': return BTN_R;
		case 'S': return BTN_S;
		case 'T': return BTN_T;
		case 'U': return BTN_U;
		case 'V': return BTN_V;
		case 'W': return BTN_W;
		case 'X': return BTN_X;
		case 'Y': return BTN_Y;
		case 'Z': return BTN_Z;

		// punctuation
		case '[':  return BTN_LBRACKET;
		case ']':  return BTN_RBRACKET;
		case '\\': return BTN_BACK_SLASH;
		case '/':  return BTN_FORWARD_SLASH;
		case ';':  return BTN_SEMICOLON;
		case '\'': return BTN_QUOTE;
		case ',':  return BTN_COMMA;
		case '.':  return BTN_PERIOD;

		// insert/home/etc
		case GLFW_KEY_INSERT: return BTN_INSERT;
		case GLFW_KEY_DEL:    return BTN_DELETE;
		case GLFW_KEY_HOME:   return BTN_HOME;
		case GLFW_KEY_END:    return BTN_END;
		case GLFW_KEY_PAGEUP: return BTN_PAGE_UP;
		case GLFW_KEY_PAGEDOWN: return BTN_PAGE_DOWN;

		// arrows
		case GLFW_KEY_UP:    return BTN_UP;
		case GLFW_KEY_DOWN:  return BTN_DOWN;
		case GLFW_KEY_LEFT:  return BTN_LEFT;
		case GLFW_KEY_RIGHT: return BTN_RIGHT;

		// keypad
		case GLFW_KEY_KP_NUM_LOCK: return BTN_NUM_LOCK;
		case GLFW_KEY_KP_DIVIDE:   return BTN_KP_SLASH;
		case GLFW_KEY_KP_MULTIPLY: return BTN_KP_ASTERISK;
		case GLFW_KEY_KP_SUBTRACT: return BTN_KP_MINUS;
		case GLFW_KEY_KP_ADD:      return BTN_KP_PLUS;
		case GLFW_KEY_KP_ENTER:    return BTN_KP_ENTER;
		case GLFW_KEY_KP_DECIMAL:  return BTN_KP_PERIOD;
		case GLFW_KEY_KP_0:        return BTN_KP_0;
		case GLFW_KEY_KP_1:        return BTN_KP_1;
		case GLFW_KEY_KP_2:        return BTN_KP_2;
		case GLFW_KEY_KP_3:        return BTN_KP_3;
		case GLFW_KEY_KP_4:        return BTN_KP_4;
		case GLFW_KEY_KP_5:        return BTN_KP_5;
		case GLFW_KEY_KP_6:        return BTN_KP_6;
		case GLFW_KEY_KP_7:        return BTN_KP_7;
		case GLFW_KEY_KP_8:        return BTN_KP_8;
		case GLFW_KEY_KP_9:        return BTN_KP_9;
	}
}

static void GLFWCALL
keyCallback(int key, int state)
{
#ifdef GLWINDOW_DEBUG
	cout << "Key: " << key << endl;
#endif

	g_instance->keyEvent(key, state);
}

void
GLWindow_GLFW::keyEvent(int key, int state)
{
	Button button = keyToButton(key);

#ifdef GLWINDOW_DEBUG
	cout << "Button: " << buttonName(button) << endl;
#endif

	if(button == BTN_M)
		setGrabMouse(!m_grabMouse);

	if(m_eventHandler != NULL) {
		if(state == GLFW_PRESS)
			m_eventHandler->buttonPress(button);
		else
			m_eventHandler->buttonRelease(button);
	}
}

static Button
mouseButtonToButton(int button)
{
	switch(button) {
		default: return BTN_NONE;
		case GLFW_MOUSE_BUTTON_1: return BTN_MOUSE1;
		case GLFW_MOUSE_BUTTON_2: return BTN_MOUSE2;
		case GLFW_MOUSE_BUTTON_3: return BTN_MOUSE3;
	}
}

static void GLFWCALL
mouseButtonCallback(int button, int state)
{
#ifdef GLWINDOW_DEBUG
	cout << "Mouse button: " << button << endl;
#endif

	g_instance->mouseButtonEvent(button, state);
}

void
GLWindow_GLFW::mouseButtonEvent(int mouseButton, int state)
{
	Button button = mouseButtonToButton(mouseButton);

#ifdef GLWINDOW_DEBUG
	cout << "Button: " << buttonName(button) << endl;
#endif

	if(m_eventHandler != NULL) {
		if(state == GLFW_PRESS)
			m_eventHandler->buttonPress(button);
		else
			m_eventHandler->buttonRelease(button);
	}
}

static void GLFWCALL
mouseWheelCallback(int position)
{
#ifdef GLWINDOW_DEBUG
	cout << "Mouse wheel: " << position << endl;
#endif
	g_instance->mouseWheelEvent(position);
}

void
GLWindow_GLFW::mouseWheelEvent(int position)
{
	if(position == m_lastMouseWheelPosition)
		return;

	bool up = (position > m_lastMouseWheelPosition);
	m_lastMouseWheelPosition = position;

#ifdef GLWINDOW_DEBUG
	if(up)
		cout << "Mouse wheel up" << endl;
	else
		cout << "Mouse wheel down" << endl;
#endif

	if(m_eventHandler != NULL)
		m_eventHandler->buttonPress(up ? BTN_MOUSE_WHEEL_UP : BTN_MOUSE_WHEEL_DOWN);
}

static void GLFWCALL
mousePosCallback(int x, int y)
{
	g_instance->mousePosEvent(x, y);
}

void
GLWindow_GLFW::mousePosEvent(int x, int y)
{
	int xrel = x - m_lastMouseX;
	int yrel = y - m_lastMouseY;

	if(xrel == 0 && yrel == 0)
		return;

	// move the mouse back to the center of the
	// window if the mouse is grabbed
	if(m_grabMouse) {
		m_lastMouseX = m_width / 2;
		m_lastMouseY = m_height / 2;
		glfwSetMousePos(m_lastMouseX, m_lastMouseY);
	} else {
		m_lastMouseX = x;
		m_lastMouseY = y;
	}

	if(m_eventHandler != NULL)
		m_eventHandler->mouseMove(x, y, xrel, yrel);
}

void
GLWindow_GLFW::open()
{
	if(isOpen())
		return;

	m_lastMouseX = 0;
	m_lastMouseY = 0;
	m_lastMouseWheelPosition = 0;

	// set window hints to use OpenGL 3.2 core profile
#if 0
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	// open the window
	int fullscreen = m_fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW;
	if(glfwOpenWindow((int)m_width, (int)m_height, 8, 8, 8, 8, 32, 0, fullscreen) == GL_FALSE)
		throw Exception("GLWindow_GLFW::open(): glfwOpenWindow failed");

	glfwEnable(GLFW_KEY_REPEAT);

	// set window title and callbacks
	glfwSetWindowTitle(m_title.c_str());
	glfwSetWindowSizeCallback(windowSizeCallback);
	glfwSetWindowCloseCallback(windowCloseCallback);
	glfwSetKeyCallback(keyCallback);
	glfwSetMouseButtonCallback(mouseButtonCallback);
	glfwSetMouseWheelCallback(mouseWheelCallback);
	glfwSetMousePosCallback(mousePosCallback);

	// hide the mouse cursor and set its position to
	// the center of the window if grabbing the mouse
	if(m_grabMouse) {
		m_lastMouseX = m_width / 2;
		m_lastMouseY = m_height / 2;

		glfwDisable(GLFW_MOUSE_CURSOR);
		glfwSetMousePos(m_lastMouseX, m_lastMouseY);
	}

#ifdef GLEW
	// initialize GLEW
	if(glewInit() != GLEW_OK) {
		close();
		g_instance = NULL;
		throw Exception("GLWindow_GLFW::open(): glewInit failed");
	}
#endif

	// do some GL setup
	glClearColor(0.25f, 0.25f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void
GLWindow_GLFW::close()
{
	if(isOpen())
		glfwCloseWindow();
}

void
GLWindow_GLFW::cycle()
{
	double time = glfwGetTime();
	float secondsElapsed = (float)(time - m_lastTime);
	m_lastTime = time;

	if(m_eventHandler != NULL) {
		m_eventHandler->cycle(secondsElapsed);
		if(m_eventHandler->render())
			glfwSwapBuffers();
	}
}

void
GLWindow_GLFW::swapBuffers()
{
	glfwSwapBuffers();
}

} // namespace DromeGL
