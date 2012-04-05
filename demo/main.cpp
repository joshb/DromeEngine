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
#include <DromeCore/DromeCore>
#include <DromeGL/DromeGL>
#include <DromeGL/GLWindow_GLFW.h>
#include <DromeMath/DromeMath>

using namespace std;
using namespace DromeCore;
using namespace DromeGL;
using namespace DromeMath;

static GLWindow_GLFW *g_window;

class MyEventHandler : public EventHandler
{
	private:
		bool m_wireframe;
		ButtonState m_buttonState;
		float m_xrot, m_yrot;

		Matrix4 m_projection;
		Matrix4 m_modelview;
		Matrix4 m_mvp;
		Vector3 m_cameraPosition;

		RefPtr <ShaderProgram> m_program;
		Cube *m_cube;

		bool m_instanced;
		int m_numCubesPerDimension;
		float m_cubeSeparation;

	public:
		MyEventHandler()
		{
			m_wireframe = false;
			m_xrot = 0.0f;
			m_yrot = 0.0f;

			m_cameraPosition = Vector3(0.0f, 0.0f, 15.0f);

			m_program = ShaderProgram::create();
			m_program->attachVertexShaderFromFile("shader.vp");
			m_program->attachFragmentShaderFromFile("shader.fp");
			m_program->linkShaders();

			glUseProgram(m_program->getId());
			m_cube = new Cube(m_program);

			m_instanced = true;
			m_numCubesPerDimension = 1;
			m_cubeSeparation = 3.0f;
		}

		~MyEventHandler()
		{
			delete m_cube;
		}

		bool
		render()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(m_program->getId());
			m_program->setUniform("modelviewMatrix", m_modelview);
			m_program->setUniform("mvpMatrix", m_projection * m_modelview);
			m_program->setUniform("numCubesPerDimension", m_instanced ? m_numCubesPerDimension : 1);
			m_program->setUniform("cubeSeparation", m_cubeSeparation);

			if(m_instanced) {
				m_program->setUniform("instancePosition", Vector3(0.0f, 0.0f, 0.0f));
				m_cube->render(m_numCubesPerDimension*m_numCubesPerDimension*m_numCubesPerDimension);
			} else {
				int npd = m_numCubesPerDimension;
				for(int instanceID = 0; instanceID < (npd*npd*npd); ++instanceID) {
					int z = (instanceID / (npd*npd)) - (npd / 2);
					int y = ((instanceID % (npd*npd)) / npd) - (npd / 2);
					int x = (instanceID % npd) - (npd / 2);

					m_program->setUniform("instancePosition", Vector3((float)x, (float)y, (float)z) * m_cubeSeparation);
					m_cube->render();
				}
			}

			return true;
		}

		void
		cycle(float secondsElapsed)
		{
			m_xrot += 0.25f * secondsElapsed;
			m_yrot += 0.9f * secondsElapsed;

			if(m_buttonState.isPressed(BTN_Z))
				m_cameraPosition.z += 20.0f * secondsElapsed;
			else if(m_buttonState.isPressed(BTN_X))
				m_cameraPosition.z -= 20.0f * secondsElapsed;

			if(m_buttonState.isPressed(BTN_A)) {
				m_cubeSeparation -= 10.0f * secondsElapsed;
				if(m_cubeSeparation < 2.0f)
					m_cubeSeparation = 2.0f;
			} else if(m_buttonState.isPressed(BTN_S)) {
				m_cubeSeparation += 10.0f * secondsElapsed;
			}

			m_modelview = Matrix4::translation(m_cameraPosition * -1.0f);
			m_modelview = m_modelview * Matrix4::rotation(m_xrot, Vector3(1.0f, 0.0f, 0.0f));
			m_modelview = m_modelview * Matrix4::rotation(m_yrot, Vector3(0.0f, 1.0f, 0.0f));
			m_mvp = m_projection * m_modelview;
		}

		void
		windowDimensionsChanged(int width, int height)
		{
			// set projection matrix
			m_projection = Matrix4::perspective(M_PI / 4.0f, (float)width / (float)height, 1.0f, 1000.0f);
		}

		void
		buttonPress(Button button)
		{
			m_buttonState.pressed(button);

			switch(button) {
				default:
					break;

				case BTN_ESCAPE:
					g_window->close();
					delete this;
					break;

				case BTN_SPACE:
					g_window->setGrabMouse(!(g_window->getGrabMouse()));
					break;

				case BTN_W:
					m_wireframe = !m_wireframe;
					glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
					break;

				case BTN_F:
					g_window->setFullScreen(!(g_window->getFullScreen()));
					break;

				case BTN_MINUS:
					if(m_numCubesPerDimension > 1) {
						--m_numCubesPerDimension;
						cout << "Number of cubes: " << (m_numCubesPerDimension*m_numCubesPerDimension*m_numCubesPerDimension) << endl;
					}
					break;

				case BTN_EQUALS:
					++m_numCubesPerDimension;
					cout << "Number of cubes: " << (m_numCubesPerDimension*m_numCubesPerDimension*m_numCubesPerDimension) << endl;
					break;

				case BTN_I:
					m_instanced = !m_instanced;
					cout << "Instanced: " << (m_instanced ? "Yes" : "No") << endl;
					break;
			}
		}

		void
		buttonRelease(Button button)
		{
			m_buttonState.released(button);
		}

		void
		mouseMove(int x, int y, int xrel, int yrel)
		{
			if(m_buttonState.isPressed(BTN_MOUSE1) || g_window->getGrabMouse()) {
				m_xrot += (float)yrel * 0.01f;
				m_yrot += (float)xrel * 0.01f;
			}
		}
};

int
main(int argc, char *argv[])
{
	File::init(argc, (const char **)argv);

	try {
		// create and open window
		g_window = new GLWindow_GLFW();
		g_window->setTitle("Drome Engine Demo");
		g_window->open();
		g_window->setEventHandler(new MyEventHandler());

		// main loop
		while(g_window->isOpen())
			g_window->cycle();
	} catch(Exception ex) {
		cout << ex.toString() << endl;
		return 1;
	}

	return 0;
}
