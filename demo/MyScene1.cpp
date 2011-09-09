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

#include <DromeGfx/DromeGfx>
#include <DromeGui/DromeGui>
#include "MyScene1.h"

using namespace std;
using namespace DromeCore;
using namespace DromeGfx;
using namespace DromeGui;
using namespace DromeMath;

MyScene1::MyScene1(IOContext *io, GfxDriver *driver)
{
	m_player.setPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_player.setBounciness(0.1f);

	// load font
	m_font = Font::create(driver, "Data/Fonts/VeraBd.ttf", 12.0f);

	// create label widget
	m_label = Label::create();
	m_label->setFont(m_font);

	// create logo widget
	m_logo = Picture::create(driver, Image::create("Data/drome.png"));
	m_logo->setWidth(m_logo->getWidth() / 2);
	m_logo->setHeight(m_logo->getHeight() / 2);
	m_logo->setY(io->getWindowHeight() - m_logo->getHeight());

	// load normalmap shaders
	m_shaderProgram = ShaderProgram::create();
	m_shaderProgram->attachVertexShaderFromFile("Data/Shaders/normalmap.vp");
	m_shaderProgram->attachFragmentShaderFromFile("Data/Shaders/normalmap.fp");
	try {
		m_shaderProgram->linkShaders();
	} catch(Exception ex) {
		m_shaderProgram = NULL;
	}

	// create/initialize other stuff
	m_sphere = SphereMesh::create(10, 0.1f);
	m_autoCamera = true;
	m_camera.setPosition(Vector3(0.0f, -4.0f, 0.0f));
	m_camera.update();

	// initialize lights
	m_lightColor[0] = Vector3(1.0f, 0.0f, 0.0f);
	m_lightColor[1] = Vector3(0.0f, 1.0f, 0.0f);
	m_lightColor[2] = Vector3(0.0f, 0.0f, 1.0f);
	m_lightRotation = 0.0f;

	// create light textures and shadow map framebuffers
	for(int i = 0; i < 3; ++i) {
		RefPtr <Image> image = Image::create(1, 1, 3);
		image->setPixel(0, 0, Color(m_lightColor[i]));
		m_lightTextures[i] = Texture::create(image);

		if(m_shaderProgram.isSet())
			m_lightFramebuffers[i] = Framebuffer::create(512, 512, true);
	}

	// load scene definition file
	loadSceneFile("Data/scene1.xml");
}

void
MyScene1::windowDimensionsChanged(int /*width*/, int height)
{
	m_logo->setY(height - m_logo->getHeight());
}

void
MyScene1::mouseMove(int /*x*/, int /*y*/, int xrel, int yrel)
{
	m_camera.rotateYaw(xrel * -0.5f);
	m_camera.rotatePitch(yrel * -0.5f);
	m_camera.update();
}

void
MyScene1::buttonPress(Button button)
{
	m_buttonState.pressed(button);
}

void
MyScene1::buttonRelease(Button button)
{
	m_buttonState.released(button);
}

void
MyScene1::cycle(float secondsElapsed)
{
	float fps = 1.0f / secondsElapsed;
	m_label->setText(String("Frames per second: ") + String((int)fps));

	m_lightRotation += (M_PI / 4.0f) * secondsElapsed;

	// handle player movement
	if(m_buttonState.isPressed(BTN_UP)) {
		Matrix4 m = m_camera.getMatrix();
		Vector3 v = Vector3(m[2], m[6], m[10]) * -10.0f * secondsElapsed;
		m_player.setPosition(m_player.getPosition() + v);
		m_autoCamera = false;
	}
	if(m_buttonState.isPressed(BTN_DOWN)) {
		Matrix4 m = m_camera.getMatrix();
		Vector3 v = Vector3(m[2], m[6], m[10]) * 10.0f * secondsElapsed;
		m_player.setPosition(m_player.getPosition() + v);
		m_autoCamera = false;
	}
	if(m_buttonState.isPressed(BTN_LEFT)) {
		Matrix4 m = m_camera.getMatrix();
		Vector3 v = Vector3(m[0], m[4], m[8]) * -10.0f * secondsElapsed;
		m_player.setPosition(m_player.getPosition() + v);
		m_autoCamera = false;
	}
	if(m_buttonState.isPressed(BTN_RIGHT)) {
		Matrix4 m = m_camera.getMatrix();
		Vector3 v = Vector3(m[0], m[4], m[8]) * 10.0f * secondsElapsed;
		m_player.setPosition(m_player.getPosition() + v);
		m_autoCamera = false;
	}

	if(m_autoCamera) {
		m_autoCameraRotation -= (M_PI / 8.0f) * secondsElapsed;
		m_camera.setPosition((Vector3(cosf(m_autoCameraRotation), sinf(m_autoCameraRotation)) * 8.5f) + Vector3(0.0f, 0.0f, 4.0f));
		m_camera.lookAt(Vector3(0.0f, 0.0f, -2.0f));

		m_player.setPosition(m_camera.getPosition());
	} else {
		// handle collision
		for(unsigned int i = 0; i < m_sceneObjects.size(); ++i)
			m_player.collisionResponse(m_sceneObjects[i]);

		m_camera.setPosition(m_player.getPosition());
	}

	m_camera.update();
}

void
MyScene1::render(GfxDriver *driver)
{
	// if the shader program wasn't created, just
	// render the objects to the screen
	if(m_shaderProgram.isNull()) {
		driver->clearBuffers();

		// render scene objects from player's perspective
		for(unsigned int i = 0; i < m_sceneObjects.size(); ++i) {
			driver->setModelViewMatrix(Matrix4().translate(m_sceneObjects[i]->getPosition()) * m_camera.getMatrix());
			m_sceneObjects[i]->render(driver);
		}

		return;
	}

	Vector3 lightPosition[3];
	Matrix4 shadowMatrix[3];

	// render shadow maps
	driver->setCullFace(CULL_FACE_FRONT);
	for(int i = 0; i < 3; ++i) {
		// calculate light position
		float r = m_lightRotation + (float)i * ((M_PI * 2.0f) / 3.0f);
		lightPosition[i] = Vector3(cosf(r), sinf(r)) * 7.5f;
		lightPosition[i].z = 0.0f;

		// create light camera
		Camera lightCamera;
		lightCamera.setPosition(lightPosition[i]);
		lightCamera.lookAt(Vector3(0.0f, 0.0f, 0.0f));
		lightCamera.update();

		// calculate shadow map matrix
		Matrix4 tmp;
		tmp[0] = tmp[5] = tmp[10] = tmp[12] = tmp[13] = tmp[14] = 0.5f;
		shadowMatrix[i] = lightCamera.getMatrix() * driver->getProjectionMatrix() * tmp;

		// render to shadow map framebuffer
		driver->bindFramebuffer(m_lightFramebuffers[i]);
		driver->clearBuffers();

		// render scene objects from light's perspective
		for(unsigned int j = 0; j < m_sceneObjects.size(); ++j) {
			driver->setModelViewMatrix(Matrix4().translate(m_sceneObjects[j]->getPosition()) * lightCamera.getMatrix());
			m_sceneObjects[j]->render(driver);
		}
	}

	// render scene to the default framebuffer
	driver->bindFramebuffer(Framebuffer::none());
	driver->clearBuffers();
	driver->setCullFace(CULL_FACE_BACK);

	// bind shader program and set uniform variables
	driver->bindShaderProgram(m_shaderProgram);
	m_shaderProgram->setUniform("texture", 0);
	m_shaderProgram->setUniform("normalmap", 1);
	m_shaderProgram->setUniform("shadowmap[0]", 2);
	m_shaderProgram->setUniform("shadowmap[1]", 3);
	m_shaderProgram->setUniform("shadowmap[2]", 4);
	m_shaderProgram->setUniform("cameraPosition", m_camera.getPosition());
	m_shaderProgram->setUniform("lightColor", m_lightColor, 3);
	m_shaderProgram->setUniform("lightPosition", lightPosition, 3);
	m_shaderProgram->setUniform("shadowMatrix", shadowMatrix, 3);

	// render scene objects from player's perspective
	driver->setModelViewMatrix(m_camera.getMatrix());
	driver->bindTexture(2, m_lightFramebuffers[0]);
	driver->bindTexture(3, m_lightFramebuffers[1]);
	driver->bindTexture(4, m_lightFramebuffers[2]);
	for(unsigned int i = 0; i < m_sceneObjects.size(); ++i) {
		m_shaderProgram->setUniform("objectPosition", m_sceneObjects[i]->getPosition());
		m_sceneObjects[i]->render(driver);
	}
	driver->bindTexture(2, Texture::none());
	driver->bindTexture(3, Texture::none());
	driver->bindTexture(4, Texture::none());

	// disable shader
	driver->bindShaderProgram(ShaderProgram::none());

	// render light spheres
	for(int i = 0; i < 3; ++i) {
		driver->setModelViewMatrix(Matrix4().translate(lightPosition[i]) * m_camera.getMatrix());
		driver->bindTexture(m_lightTextures[i]);
		m_sphere->render();
	}

	// render GUI elements
	driver->enable2D();
	m_logo->render(driver);
	m_label->render(driver);
	driver->disable2D();
}

void
MyScene1::loadSceneFile(const char *filePath)
{
	// load XML scene file
	XmlDocument doc;
	doc.fromFile(File::getPath(filePath));

	// make sure the root element is named correctly
	const XmlElement *root = doc.getRootElement();
	if(root->getName() != "dromescene")
		throw Exception("MyScene1::loadSceneFile(): Invalid root element name");

	vector < RefPtr <Texture> > textures;

	// loop through each child element
	for(unsigned int i = 0; i < root->getNumChildren(); ++i) {
		const XmlElement *child = root->getChild(i);

		if(child->getName() == "texture") {
			// create texture
			const XmlAttribute *attr = child->getAttribute("filePath");
			if(!attr)
				throw Exception("MyScene1::loadSceneFile(): Texture without filePath");
			textures.push_back(Texture::create(Image::create(attr->getValue())));
		} else if(child->getName() == "block") {
			// parse position
			const XmlAttribute *attr = child->getAttribute("position");
			if(!attr)
				throw Exception("MyScene1::loadSceneFile(): Block without position");
			Vector3 position(attr->getValue());

			// parse bounds
			attr = child->getAttribute("bounds");
			if(!attr)
				throw Exception("MyScene1::loadSceneFile(): Block without bounds");
			Vector3 bounds(attr->getValue());

			// parse texture index
			attr = child->getAttribute("textureIndex");
			unsigned int index = (attr != NULL) ? (unsigned int)String(attr->getValue()).toInt() : 0;
			if(index >= textures.size())
				throw Exception("MyScene1::loadSceneFile(): Block with invalid texture index");

			// parse normalmap index
			RefPtr <Texture> normalmap;
			attr = child->getAttribute("normalmapIndex");
			if(attr != NULL) {
				unsigned int tmp = (unsigned int)String(attr->getValue()).toInt();
				if(tmp >= textures.size())
					throw Exception("MyScene1::loadSceneFile(): Block with invalid normalmap index");
				normalmap = textures[tmp];
			}

			// create block
			m_sceneObjects.push_back(new Block(position, bounds, textures[index], normalmap));
		} else {
			throw Exception("MyScene1::loadSceneFile(): Invalid element name '" + child->getName() + "'");
		}
	}
}
