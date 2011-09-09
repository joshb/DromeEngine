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

#ifndef __DROMEMATH_PHYSICSOBJECT_H__
#define __DROMEMATH_PHYSICSOBJECT_H__

#include "Vector3.h"

namespace DromeMath {

const float STANDARD_GRAVITY = 9.8f;

enum PhysicsObjectType
{
	PHYSICS_OBJECT_TYPE_POINT = 0,
	PHYSICS_OBJECT_TYPE_BOUNDING_BOX,
	PHYSICS_OBJECT_TYPE_BOUNDING_SPHERE
};

class CollisionInfo
{
	public:
		bool collision;
		Vector3 direction;
		float distance;

		CollisionInfo(bool collisionParam, const Vector3 &directionParam = Vector3(), float distanceParam = 0.0f) { collision = collisionParam; direction = directionParam; distance = distanceParam; }
};

class PhysicsObject
{
	protected:
		Vector3 m_position, m_oldPosition;
		Vector3 m_velocity;
		float m_bounciness;

	public:
		PhysicsObject();

		Vector3 getPosition() const;
		void setPosition(const Vector3 &value);

		Vector3 getVelocity() const;
		void setVelocity(const Vector3 &value);

		float getBounciness() const;
		void setBounciness(float value);

		void cycle(float time, float gravity = STANDARD_GRAVITY);

		virtual PhysicsObjectType getType() const;
		virtual CollisionInfo collisionCheck(const PhysicsObject *object) const;
		virtual CollisionInfo collisionCheck(const PhysicsObject &object) const;
		virtual void collisionResponse(const PhysicsObject *object);
		virtual void collisionResponse(const PhysicsObject &object);
};

} // namespace DromeMath

#endif /* __DROMEMATH_PHYSICSOBJECT_H__ */
