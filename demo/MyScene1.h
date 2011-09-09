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

#include <vector>
#include <DromeCore/DromeCore>
#include <DromeGfx/DromeGfx>
#include <DromeGui/DromeGui>
#include "Block.h"

class MyScene1 : public DromeGfx::Scene
{
	protected:
		DromeCore::ButtonState m_buttonState;
		bool m_autoCamera;
		float m_autoCameraRotation;
		DromeGfx::Camera m_camera;
		DromeMath::BoundingBox m_player;

		std::vector <Block *> m_sceneObjects;

		// GUI
		DromeCore::RefPtr <DromeGui::Font> m_font;
		DromeCore::RefPtr <DromeGui::Label> m_label;
		DromeCore::RefPtr <DromeGui::Picture> m_logo;

		// lights
		DromeCore::RefPtr <DromeGfx::Framebuffer> m_lightFramebuffers[3];
		DromeCore::RefPtr <DromeGfx::Texture> m_lightTextures[3];
		DromeMath::Vector3 m_lightColor[3];
		float m_lightRotation;

		// other
		DromeCore::RefPtr <DromeGfx::Mesh> m_sphere;
		DromeCore::RefPtr <DromeGfx::ShaderProgram> m_shaderProgram;

	public:
		MyScene1(DromeCore::IOContext *io, DromeGfx::GfxDriver *driver);

		void windowDimensionsChanged(int width, int height);
		void mouseMove(int x, int y, int xrel, int yrel);
		void buttonPress(DromeCore::Button button);
		void buttonRelease(DromeCore::Button button);

		void cycle(float secondsElapsed);
		void render(DromeGfx::GfxDriver *driver);

		void loadSceneFile(const char *filePath);
};
