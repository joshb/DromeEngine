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

#include <DromeCore/Util.h>
#include <DromeGfx/OpenGL.h>
#include <DromeGfx/ParticleEmitter.h>

using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

/*
 * Particle class
 */
Particle::Particle()
{
	m_done = false;

	m_rotation = 0.0f;
	m_width = 1.0f;
	m_height = 1.0f;
}

void
Particle::render(GfxDriver *driver)
{
	// set modelview matrix for billboarding
	glPushMatrix();
	Matrix4 m = driver->getModelViewMatrix().translate(m_position);
	m = m.removeTranslation().transpose() * m;
	driver->setModelViewMatrix(m);
	glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);

	float verts[] = {
		-m_width, m_height, 0.0f,
		-m_width, -m_height, 0.0f,
		m_width, m_height, 0.0f,
		m_width, -m_height, 0.0f
	};

	float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	glColor4ub(m_color.r, m_color.g, m_color.b, m_color.a);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void
Particle::cycle(float secondsElapsed)
{
	m_position += m_velocity * secondsElapsed;
	m_rotation += 30.0f * secondsElapsed;
	m_width -= 0.25f * secondsElapsed;
	m_height -= 0.25f * secondsElapsed;

	float a = m_color.getFloatA() - (0.75f * secondsElapsed);
	if(a < 0.0f)
		m_done = true;
	else
		m_color.setA(a);
}

RefPtr <Particle>
Particle::create()
{
	return RefPtr <Particle> (new Particle());
}

/*
 * ParticleEmitter class
 */
ParticleEmitter::ParticleEmitter()
{
	m_width = 1.0f;
	m_height = 1.0f;

	m_particlesPerSecond = 1.0f;
	m_timeRemainder = 0.0f;
}

RefPtr <Particle>
ParticleEmitter::createParticle()
{
	RefPtr <Particle> p = Particle::create();

	p->setPosition(m_position);
	p->setWidth(m_width);
	p->setHeight(m_height);
	p->setColor(m_color);

	// set random rotation
	p->setRotation(randomFloat(0.0f, 360.0f));

	// set random velocity
	p->setVelocity(Vector3(randomFloat(), randomFloat(), randomFloat()).normalize());

	// set random size
	p->setWidth(randomFloat(0.0f, 1.0f));
	p->setHeight(p->getWidth());

	return p;
}

void
ParticleEmitter::render(GfxDriver *driver)
{
	bool depthWritesEnabled = driver->getDepthWritesEnabled();
	BlendMode blendMode = driver->getBlendMode();

	driver->setDepthWritesEnabled(false);
	driver->setBlendMode(BLEND_MODE_ADD);

	// render each particle
	glBindTexture(GL_TEXTURE_2D, m_texture->getId());
	for(unsigned int i = 0; i < m_particles.size(); i++)
		m_particles[i]->render(driver);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	driver->setBlendMode(blendMode);
	driver->setDepthWritesEnabled(depthWritesEnabled);
}

void
ParticleEmitter::cycle(float secondsElapsed)
{
	// cycle each particle
	for(unsigned int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->cycle(secondsElapsed);
		if(m_particles[i]->isDone()) {
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}
	
	// create new particles if necessary
	for(secondsElapsed += m_timeRemainder; secondsElapsed > 0.0f; secondsElapsed -= 1.0f / m_particlesPerSecond) {
		// create particle
		m_particles.insert(m_particles.end(), createParticle());
	}
	m_timeRemainder = secondsElapsed;
}

RefPtr <ParticleEmitter>
ParticleEmitter::create()
{
	return RefPtr <ParticleEmitter> (new ParticleEmitter());
}

} // namespace DromeGfx
