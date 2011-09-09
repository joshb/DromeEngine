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

#ifndef __DROMEGFX_PARTICLEEMITTER_H__
#define __DROMEGFX_PARTICLEEMITTER_H__

#include <vector>
#include <DromeCore/Ref.h>
#include "Driver.h"

namespace DromeGfx {

class Particle : public DromeCore::RefClass
{
	protected:
		bool m_done;

		DromeMath::Vector3 m_position;
		DromeMath::Vector3 m_velocity;
		float m_rotation;
		float m_width;
		float m_height;

		Color m_color;

		Particle();
		virtual ~Particle() { }

	public:
		bool isDone() const { return m_done; }

		DromeMath::Vector3 getPosition() const { return m_position; }
		void setPosition(const DromeMath::Vector3 &value) { m_position = value; }

		DromeMath::Vector3 getVelocity() const { return m_velocity; }
		void setVelocity(const DromeMath::Vector3 &value) { m_velocity = value; }

		float getRotation() const { return m_rotation; }
		void setRotation(float value) { m_rotation = value; }

		float getWidth() const { return m_width; }
		void setWidth(float value) { m_width = value; }

		float getHeight() const { return m_height; }
		void setHeight(float value) { m_height = value; }

		Color getColor() const { return m_color; }
		void setColor(const Color &value) { m_color = value; }

		void render(DromeGfx::GfxDriver *driver);
		void cycle(float secondsElapsed);

		static DromeCore::RefPtr <Particle> create();
};

class ParticleEmitter : public DromeCore::RefClass
{
	protected:
		DromeMath::Vector3 m_position;
		float m_width;
		float m_height;

		Color m_color;
		DromeCore::RefPtr <Texture> m_texture;

		std::vector < DromeCore::RefPtr <Particle> > m_particles;
		float m_particlesPerSecond;
		float m_timeRemainder;

		ParticleEmitter();
		virtual ~ParticleEmitter() { }

	public:
		DromeMath::Vector3 getPosition() const { return m_position; }
		void setPosition(const DromeMath::Vector3 &value) { m_position = value; }

		float getWidth() const { return m_width; }
		void setWidth(float value) { m_width = value; }

		float getHeight() const { return m_height; }
		void setHeight(float value) { m_height = value; }

		Color getColor() const { return m_color; }
		void setColor(const Color &value) { m_color = value; }

		DromeCore::RefPtr <Texture> getTexture() const { return m_texture; }
		void setTexture(DromeCore::RefPtr <Texture> value) { m_texture = value; }

		float getParticlesPerSecond() const { return m_particlesPerSecond; }
		void setParticlesPerSecond(float value) { m_particlesPerSecond = value; }

		DromeCore::RefPtr <Particle> createParticle();
		void render(DromeGfx::GfxDriver *driver);
		void cycle(float secondsElapsed);

		static DromeCore::RefPtr <ParticleEmitter> create();
};

} // namespace DromeGfx

#endif /* __DROMEGFX_PARTICLEEMITTER_H__ */
