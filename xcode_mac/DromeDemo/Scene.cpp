/*
 * Copyright (C) 2013 Josh A. Beam
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

#include <cmath>
#include <DromeGL/Sphere.h>
#include "Scene.h"

using namespace DromeCore;
using namespace DromeGL;
using namespace DromeMath;

Scene::Scene()
{
	// load normalmap texture
    _normalmap = Texture::create(Image::create("normalmap.png"));
    
	// create the program, attach shaders, and link the program */
    _program = ShaderProgram::create();
    _program->attachVertexShaderFromFile("shader.vs");
    _program->attachFragmentShaderFromFile("shader.fs");
    _program->linkShaders();
    
	// get uniform locations
	_programProjectionMatrixLocation = _program->getUniformLocation("projectionMatrix");
	_programModelviewMatrixLocation = _program->getUniformLocation("modelviewMatrix");
	_programCameraPositionLocation = _program->getUniformLocation("cameraPosition");
	_programLightPositionLocation = _program->getUniformLocation("lightPosition");
	_programLightColorLocation = _program->getUniformLocation("lightColor");
    
	// set up red/green/blue lights
    _lightColor[0] = Color3(1.0f, 0.0f, 0.0f);
    _lightColor[1] = Color3(0.0f, 1.0f, 0.0f);
    _lightColor[2] = Color3(0.0f, 0.0f, 1.0f);
    
	// create the mesh
    _mesh = new Sphere(_program);
    
	// do the first cycle to initialize positions
	_lightRotation = 0.0f;
    _cameraRotation = 0.0f;
}

void
Scene::windowDimensionsChanged(int width, int height)
{
    _projectionMatrix = DromeMath::Matrix4::perspective((float)M_PI / 4.0f, (float)width / (float)height, 0.1f, 200.0f);
}

bool
Scene::render()
{
    DromeMath::Matrix4 translationMatrix = DromeMath::Matrix4::translation(_cameraPosition * -1.0f);
    DromeMath::Matrix4 rotationMatrix = DromeMath::Matrix4::rotation(_cameraRotation, Vector3(0.0f, -1.0f, 0.0f));
    DromeMath::Matrix4 modelviewMatrix = rotationMatrix * translationMatrix;
    
	// enable the program and set uniform variables
    glBindTexture(GL_TEXTURE_2D, _normalmap->getId());
    glUseProgram(_program->getId());
    _program->setUniform("projectionMatrix", _projectionMatrix);
    _program->setUniform("modelviewMatrix", modelviewMatrix);
    _program->setUniform("cameraPosition", _cameraPosition);
    _program->setUniform("lightPosition", _lightPosition, NUM_LIGHTS);
    _program->setUniform("lightColor", _lightColor, NUM_LIGHTS);

    _mesh->render();
    
	// disable the program
	glUseProgram(0);
    return true;
}

void
Scene::cycle(float secondsElapsed)
{
	// update the light positions
	_lightRotation += (M_PI / 4.0f) * secondsElapsed;
	for(int i = 0; i < NUM_LIGHTS; ++i) {
		const float radius = 1.75f;
		float r = (((M_PI * 2.0f) / (float)NUM_LIGHTS) * (float)i) + _lightRotation;
        
		_lightPosition[i].x = cosf(r) * radius;
		_lightPosition[i].y = cosf(r) * sinf(r);
		_lightPosition[i].z = sinf(r) * radius;
	}
    
    _cameraRotation -= (M_PI / 16.0f) * secondsElapsed;
    _cameraPosition.x = sinf(_cameraRotation) * 4.0f;
    _cameraPosition.y = 0.0f;
    _cameraPosition.z = cosf(_cameraRotation) * 4.0f;
}