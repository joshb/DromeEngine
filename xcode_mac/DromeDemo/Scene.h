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

#ifndef __DromeDemo__Scene2__
#define __DromeDemo__Scene2__

#include <DromeCore/EventHandler.h>
#include <DromeGL/Sphere.h>
#include <DromeGL/Texture.h>
#include <DromeGL/ShaderProgram.h>

#define NUM_LIGHTS 3

class Scene : public DromeCore::EventHandler
{
private:
    DromeMath::Matrix4 _projectionMatrix;
    
    DromeCore::RefPtr <DromeGL::Texture> _normalmap;
    DromeGL::Sphere *_sphere;
    
    DromeCore::RefPtr <DromeGL::ShaderProgram> _program;
	GLint _programProjectionMatrixLocation;
	GLint _programModelviewMatrixLocation;
	GLint _programCameraPositionLocation;
	GLint _programLightPositionLocation;
	GLint _programLightColorLocation;
	GLint _programFragmentColorLocation;
    
    float _cameraRotation;
    DromeMath::Vector3 _cameraPosition;
	
    DromeMath::Vector3 _lightPosition[NUM_LIGHTS];
    DromeMath::Color3 _lightColor[NUM_LIGHTS];
	float _lightRotation;
    
public:
    Scene();
    void windowDimensionsChanged(int width, int height);
    bool render();
    void cycle(float secondsElapsed);
};

#endif /* defined(__DromeDemo__Scene2__) */
