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

#ifndef __DROMECORE_IOCONTEXT_H__
#define __DROMECORE_IOCONTEXT_H__

#include <string>
#include "Button.h"
#include "Endian.h"
#include "EventHandler.h"

namespace DromeCore {

/**
 * The IOContext class is an abstract interface for accessing various platform-specific functionality, such as window creation and mouse/keyboard input.
 */
class IOContext {
	protected:
		EventHandler *m_handler;
		int m_ticks;

		void cycle();

	public:
		IOContext();
		virtual ~IOContext();

		/**
		 * @return Pointer to current EventHandler as set by setEventHandler().
		 */
		const EventHandler *getEventHandler() const;

		/**
		 * Sets the IOContext's EventHandler pointer. Events received by the IOContext will be passed to the EventHandler's appropriate event handling function.
		 *
		 * @param value Pointer to EventHandler.
		 */
		void setEventHandler(EventHandler *value);


	// virtual functions
		/**
		 * @return Width of rendering window.
		 */
		virtual int getWindowWidth() const;

		/**
		 * @return Height of rendering window.
		 */
		virtual int getWindowHeight() const;

		/**
		 * Sets the width and height of the rendering window to be created by the Init() function.
		 *
		 * @param width Width of window.
		 * @param height Height of window.
		 */
		virtual void setWindowDimensions(int width, int height);

		/**
		 * @return True if the rendering window is fullscreen, false otherwise.
		 */
		virtual bool getFullScreen() const;

		/**
		 * Sets a boolean value that, if set to true, will cause the rendering window created by the Init() function to be fullscreen.
		 *
		 * @param value True for fullscreen or false for non-fullscreen (windowed).
		 */
		virtual void setFullScreen(bool value);

		/**
		 * @return String containing title of the rendering window.
		 */
		virtual std::string getWindowTitle() const;

		/**
		 * Sets the title of the rendering window to be created by the Init() function.
		 *
		 * @param value String containing window title.
		 */
		virtual void setWindowTitle(const std::string &value);

		/**
		 * Performs platform-specific IO setup, such as setting up the rendering window and input handling.
		 */
		virtual bool init() = 0;

		/**
		 * Performs platform-specific IO shutdown, such as closing the rendering window.
		 */
		virtual void shutdown() = 0;

		virtual bool run() = 0;

		/**
		 * Calls the platform-specific function to get the address of the OpenGL function with the given name.
		 *
		 * @param functionName The name of the function to get the address of.
		 * @return The address of the function with the given name.
		 */
		virtual void *getProcAddress(const char *functionName) const = 0;

		/**
		 * Calls the platform-specific function to swap the OpenGL backbuffer.
		 */
		virtual void swapBuffers() = 0;

		/**
		 * Makes the mouse pointer windowed, so that it can't be moved outside of the rendering window.
		 */
		virtual void grabMousePointer() = 0;

		/**
		 * Makes the mouse pointer non-windowed, so that it can be moved outside of the rendering window.
		 */
		virtual void releaseMousePointer() = 0;

	// static functions
		/**
		 * Creates a new IOContext.
		 *
		 * @param type Type of IOContext to create. Valid types include "null", "cocoa", and "sdl".
		 * @return Pointer to new IOContext.
		 */
		static IOContext *create(const char *type = NULL);
};

} // namespace DromeCore

#endif /* __DROMECORE_IOCONTEXT_H__ */
