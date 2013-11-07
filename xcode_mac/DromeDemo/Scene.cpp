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
    _program->attachVertexShaderFromFile("shader.vp");
    _program->attachFragmentShaderFromFile("shader.fp");
    _program->linkShaders();
    
	// get uniform locations
	_programProjectionMatrixLocation = _program->getUniformLocation("projectionMatrix");
	_programModelviewMatrixLocation = _program->getUniformLocation("modelviewMatrix");
	_programCameraPositionLocation = _program->getUniformLocation("cameraPosition");
	_programLightPositionLocation = _program->getUniformLocation("lightPosition");
	_programLightColorLocation = _program->getUniformLocation("lightColor");
    
	// set up red/green/blue lights
	_lightColor[0] = 1.0f; _lightColor[1] = 0.0f; _lightColor[2] = 0.0f;
	_lightColor[3] = 0.0f; _lightColor[4] = 1.0f; _lightColor[5] = 0.0f;
	_lightColor[6] = 0.0f; _lightColor[7] = 0.0f; _lightColor[8] = 1.0f;
    
	// create the sphere
    _sphere = new Sphere(_program);
    
	// do the first cycle to initialize positions
	_lightRotation = 0.0f;
    _cameraRotation = 0.0f;
}

void
Scene::setProjectionMatrix(const Matrix4 &projectionMatrix)
{
    _projectionMatrix = projectionMatrix;
}

bool
Scene::render()
{
    DromeMath::Matrix4 translationMatrix = DromeMath::Matrix4::translation(Vector3(-_cameraPosition[0], -_cameraPosition[1], -_cameraPosition[2]));
    DromeMath::Matrix4 rotationMatrix = DromeMath::Matrix4::rotation(_cameraRotation, Vector3(0.0f, -1.0f, 0.0f));
    DromeMath::Matrix4 modelviewMatrix = rotationMatrix * translationMatrix;
    
	// enable the program and set uniform variables
    glBindTexture(GL_TEXTURE_2D, _normalmap->getId());
    glUseProgram(_program->getId());
	glUniformMatrix4fv(_programProjectionMatrixLocation, 1, GL_FALSE, _projectionMatrix.getData());
	glUniformMatrix4fv(_programModelviewMatrixLocation, 1, GL_FALSE, modelviewMatrix.getData());
	glUniform3fv(_programCameraPositionLocation, 1, _cameraPosition);
	glUniform3fv(_programLightPositionLocation, NUM_LIGHTS, _lightPosition);
	glUniform3fv(_programLightColorLocation, NUM_LIGHTS, _lightColor);
    
	// render the sphere
    _sphere->render();
    
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
        
		_lightPosition[i * 3 + 0] = cosf(r) * radius;
		_lightPosition[i * 3 + 1] = cosf(r) * sinf(r);
		_lightPosition[i * 3 + 2] = sinf(r) * radius;
	}
    
    _cameraRotation -= (M_PI / 16.0f) * secondsElapsed;
    _cameraPosition[0] = sinf(_cameraRotation) * 4.0f;
    _cameraPosition[1] = 0.0f;
    _cameraPosition[2] = cosf(_cameraRotation) * 4.0f;
}