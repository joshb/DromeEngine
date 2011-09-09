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

#ifndef __DROMECORE_EVENTHANDLER_H__
#define __DROMECORE_EVENTHANDLER_H__

#include "Button.h"

namespace DromeCore {

/**
 * The abstract EventHandler class is used to handle input/output events, such as mouse movement and button presses.
 */
class EventHandler {
	protected:
		EventHandler() { }

	public:
		virtual ~EventHandler() { }

		/**
		 * Called when the application may perform tasks such as rendering and logic.
		 *
		 * @param secondsElapsed The number of seconds that have elapsed since the last cycle.
		 */
		virtual void cycle(float secondsElapsed) = 0;

		/**
		 * Called when the application window's dimensions have changed.
		 *
		 * @param width The new width of the application window.
		 * @param height The new height of the application window.
		 */
		virtual void windowDimensionsChanged(int width, int height);

		/**
		 * Handles a mouse movement event.
		 *
		 * @param x New X position of mouse pointer.
		 * @param y New Y position of mouse pointer.
		 * @param xrel New X position of mouse pointer relative to the previous X position.
		 * @param yrel New Y position of mouse pointer relative to the previous Y position.
		 */
		virtual void mouseMove(int x, int y, int xrel, int yrel);

		/**
		 * Handles a button press event.
		 *
		 * @param button Button indicating the button that has been pressed.
		 */
		virtual void buttonPress(Button button);

		/**
		 * Handles a button release event.
		 *
		 * @param button Button indicating the button that has been released.
		 */
		virtual void buttonRelease(Button button);
};

} // namespace DromeCore

#endif /* __DROMECORE_EVENTHANDLER_H__ */
