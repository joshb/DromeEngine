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

#ifndef __DROMECORE_BUTTONSTATE_H__
#define __DROMECORE_BUTTONSTATE_H__

#include "Button.h"

namespace DromeCore {

/**
 * The ButtonState class is for keeping track of the state of each button in enum Button.
 */
class ButtonState {
	protected:
		bool m_buttons[NUM_BUTTONS];

	public:
		ButtonState();
		virtual ~ButtonState();

		/**
		 * Sets a button's state to pressed/down.
		 *
		 * @param button Button indicating the button that has been pressed.
		 */
		void pressed(Button button);

		/**
		 * Sets a button's state to released/up.
		 *
		 * @param button Button indicating the button that has been released.
		 */
		void released(Button button);

		/**
		 * Returns the state of a button.
		 *
		 * @param button Button indicating which button's state should be returned.
		 * @return True if button's state is pressed/down, false otherwise.
		 */
		bool isPressed(Button button) const;
};

} // namespace DromeCore

#endif /* __DROMECORE_BUTTONSTATE_H__ */
