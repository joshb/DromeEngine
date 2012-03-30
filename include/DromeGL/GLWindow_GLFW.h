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

#ifndef __DROMEGL_GLWINDOW_GLFW_H__
#define __DROMEGL_GLWINDOW_GLFW_H__

#include <string>
#include <DromeCore/EventHandler.h>

namespace DromeGL {

class GLWindow_GLFW
{
	private:
		int m_width, m_height;
		bool m_fullscreen;
		std::string m_title;
		DromeCore::EventHandler *m_eventHandler;

		bool m_grabMouse;
		int m_lastMouseX, m_lastMouseY;
		int m_lastMouseWheelPosition;

	public:
		GLWindow_GLFW();
		virtual ~GLWindow_GLFW();

		bool isOpen() const;

		void setDimensions(int width, int height);
		void setFullScreen(bool value);
		void setTitle(const char *value);
		void setEventHandler(DromeCore::EventHandler *value);
		void setGrabMouse(bool value);

		void windowSizeChanged(int width, int height);
		void keyEvent(int key, int state);
		void mouseButtonEvent(int button, int state);
		void mouseWheelEvent(int position);
		void mousePosEvent(int x, int y);

		void open();
		void close();
		void swapBuffers();
};

} // namespace DromeGL

#endif /* __DROMEGL_GLWINDOW_GLFW_H__ */
