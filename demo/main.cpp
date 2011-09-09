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

#include <iostream>
#include "MyScene1.h"

using namespace std;
using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeGui;
using namespace DromeMath;

const int RESOLUTIONS[][2] = {
	{ 640, 480 },
	{ 400, 300 },
	{ 320, 240 }
};
const int NUM_RESOLUTIONS = sizeof(RESOLUTIONS) / sizeof(RESOLUTIONS[0]);

class MyEventHandler : public EventHandler
{
	protected:
		IOContext *m_io;
		GfxDriver *m_driver;
		ButtonState m_buttonState;
		Scene *m_scene;

		int m_resolutionIndex;
		bool m_wireframe, m_grabMouse;

	public:
		MyEventHandler()
		{
			m_resolutionIndex = 0;
			m_wireframe = false;
			m_grabMouse = false;

			// create and initialize IOContext
			m_io = IOContext::create();
			m_io->setEventHandler(this);
			m_io->setWindowDimensions(RESOLUTIONS[m_resolutionIndex][0], RESOLUTIONS[m_resolutionIndex][1]);
			m_io->setWindowTitle("Drome Engine Demo");
			m_io->init();

			// create graphics driver
			m_driver = GfxDriver::create();
			m_driver->setViewportDimensions(m_io->getWindowWidth(), m_io->getWindowHeight());
			m_driver->setProjectionMatrix(m_io->getWindowWidth(), m_io->getWindowHeight());

			// create scene
			m_scene = new MyScene1(m_io, m_driver);
		}

		~MyEventHandler()
		{
			delete m_scene;
			delete m_driver;
			delete m_io;
		}

		bool
		run()
		{
			return m_io->run();
		}

		void
		cycle(float secondsElapsed)
		{
			m_scene->cycle(secondsElapsed);
			m_scene->render(m_driver);

			m_driver->flush();
			m_io->swapBuffers();
		}

		void
		windowDimensionsChanged(int width, int height)
		{
			m_driver->setViewportDimensions(width, height);
			m_driver->setProjectionMatrix(width, height);
			m_scene->windowDimensionsChanged(width, height);
		}

		void
		mouseMove(int x, int y, int xrel, int yrel)
		{
			m_scene->mouseMove(x, y, xrel, yrel);
		}

		void
		buttonPress(Button button)
		{
			switch(button) {
				default:
					m_scene->buttonPress(button);
					break;
				case BTN_ESCAPE:
					m_io->shutdown();
					break;
				case BTN_W:
					m_wireframe = !m_wireframe;
					m_driver->setWireframe(m_wireframe);
					break;
				case BTN_R:
					++m_resolutionIndex;
					m_resolutionIndex %= NUM_RESOLUTIONS;
					m_io->setWindowDimensions(RESOLUTIONS[m_resolutionIndex][0], RESOLUTIONS[m_resolutionIndex][1]);
					break;
				case BTN_F:
					m_io->setFullScreen(m_io->getFullScreen() == false);
					break;
				case BTN_SPACE:
					m_grabMouse = !m_grabMouse;
					if(m_grabMouse)
						m_io->grabMousePointer();
					else
						m_io->releaseMousePointer();
					break;
			}
		}

		void
		buttonRelease(Button button)
		{
			m_scene->buttonRelease(button);
		}
};

int
main(int argc, char *argv[])
{
	bool result = false;

	try {
		File::init(argc, (const char **)argv);

		// create and run MyEventHandler
		MyEventHandler *handler = new MyEventHandler();
		result = handler->run();

		// cleanup
		delete handler;
	} catch(Exception ex) {
		cout << "EXCEPTION: " << ex.toString() << endl;
	}

	return result ? 0 : 1;
}
