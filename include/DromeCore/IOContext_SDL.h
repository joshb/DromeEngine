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

#ifndef __DROMECORE_IOCONTEXT_SDL_H__
#define __DROMECORE_IOCONTEXT_SDL_H__

#include <SDL/SDL.h>
#include "IOContext.h"

namespace DromeCore {

class IOContext_SDL : public IOContext {
	protected:
		int m_windowWidth, m_windowHeight;
		bool m_fullscreen;
		string m_windowTitle;

		bool m_initialized;
		bool m_running;
		bool m_grabInput;

	protected:
		SDL_Surface *setVideoMode();

	public:
		IOContext_SDL();

		int getWindowWidth() const;
		int getWindowHeight() const;
		void setWindowDimensions(int width, int height);
		bool getFullScreen() const;
		void setFullScreen(bool value);
		string getWindowTitle() const;
		void setWindowTitle(const string &value);

		bool init();
		void cycle();
		void shutdown();
		void quit();

		void *getProcAddress(const char *functionName) const;
		void swapBuffers();
		void checkInput();
		void grabMousePointer();
		void releaseMousePointer();
};

} // namespace DromeCore

#endif /* __DROMECORE_IOCONTEXT_SDL_H__ */
