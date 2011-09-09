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

#include <DromeMath/PhysicsObject.h>

namespace DromeMath {

PhysicsObject::PhysicsObject()
{
	setBounciness(0.5f);
}

Vector3
PhysicsObject::getPosition() const
{
	return m_position;
}

void
PhysicsObject::setPosition(const Vector3 &value)
{
	m_position = value;
}

Vector3
PhysicsObject::getVelocity() const
{
	return m_velocity;
}

void
PhysicsObject::setVelocity(const Vector3 &value)
{
	m_velocity = value;
}

float
PhysicsObject::getBounciness() const
{
	return m_bounciness;
}

void
PhysicsObject::setBounciness(float value)
{
	m_bounciness = value;
}

void
PhysicsObject::cycle(float time, float gravity)
{
	m_oldPosition = m_position;
	m_position += m_velocity * time;
	m_velocity.z -= gravity * time;
}

PhysicsObjectType
PhysicsObject::getType() const
{
	return PHYSICS_OBJECT_TYPE_POINT;
}

CollisionInfo
PhysicsObject::collisionCheck(const PhysicsObject * /*object*/) const
{
	return CollisionInfo(false);
}

CollisionInfo
PhysicsObject::collisionCheck(const PhysicsObject &object) const
{
	return collisionCheck(&object);
}

void
PhysicsObject::collisionResponse(const PhysicsObject *object)
{
	CollisionInfo info = collisionCheck(object);
	if(info.collision) {
	//	m_position = m_oldPosition;
		m_position += info.direction * info.distance;

		Vector3 normal = info.direction.normalize();
		m_velocity += (normal * (m_velocity * -1.0f).dotProduct(normal) * (1.0f + m_bounciness));
	}
}

void
PhysicsObject::collisionResponse(const PhysicsObject &object)
{
	collisionResponse(&object);
}

} // namespace DromeMath
