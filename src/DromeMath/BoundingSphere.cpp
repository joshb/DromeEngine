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

#include <cmath>
#include <DromeMath/BoundingSphere.h>
#include <DromeMath/BoundingBox.h>

namespace DromeMath {

BoundingSphere::BoundingSphere(const Vector3 &position, float radius)
{
	m_position = position;
	m_radius = radius;
}

float
BoundingSphere::getRadius() const
{
	return m_radius;
}

void
BoundingSphere::setRadius(float value)
{
	m_radius = value;
}

PhysicsObjectType
BoundingSphere::getType() const
{
	return PHYSICS_OBJECT_TYPE_BOUNDING_SPHERE;
}

static bool
pointInSphere(const Vector3 &point,
              const Vector3 &spherePosition, float sphereRadius)
{
	Vector3 tmp = point - spherePosition;
	float dist = tmp.dotProduct(tmp);
	return (dist < (sphereRadius*sphereRadius));
}

CollisionInfo
BoundingSphere::collisionCheck(const BoundingSphere *sphere) const
{
	Vector3 otherPosition = sphere->getPosition();
	float otherRadius = sphere->getRadius();

	Vector3 tmp = m_position - otherPosition;
	float dist = tmp.dotProduct(tmp);
	float radius = (otherRadius + m_radius);
	float radiusSquared = radius * radius;

	if(dist > radiusSquared)
		return CollisionInfo(false);

	return CollisionInfo(true, tmp.normalize(), radius - sqrtf(dist));
}

CollisionInfo
BoundingSphere::collisionCheck(const BoundingBox *box) const
{
	Vector3 boxPosition = box->getPosition();
	Vector3 boxBounds = box->getBounds();
	Vector3 min = boxPosition - boxBounds;
	Vector3 max = boxPosition + boxBounds;

	// clamp the sphere position to the box boundaries
	// to get the point on the box closest to the sphere
	Vector3 position = m_position;
	if(position.x < min.x)
		position.x = min.x;
	else if(position.x > max.x)
		position.x = max.x;
	if(position.y < min.y)
		position.y = min.y;
	else if(position.y > max.y)
		position.y = max.y;
	if(position.z < min.z)
		position.z = min.z;
	else if(position.z > max.z)
		position.z = max.z;

	Vector3 tmp = m_position - position;
	float dist = tmp.dotProduct(tmp);

	return CollisionInfo(dist < (m_radius*m_radius), tmp);
}

CollisionInfo
BoundingSphere::collisionCheck(const PhysicsObject *object) const
{
	PhysicsObjectType otherType = object->getType();
	switch(otherType) {
		default:
			return PhysicsObject::collisionCheck(object);
		case PHYSICS_OBJECT_TYPE_POINT:
			return pointInSphere(object->getPosition(), m_position, m_radius);
		case PHYSICS_OBJECT_TYPE_BOUNDING_SPHERE:
			return collisionCheck((const BoundingSphere *)object);
		case PHYSICS_OBJECT_TYPE_BOUNDING_BOX:
			return collisionCheck((const BoundingBox *)object);
	}
}

} // namespace DromeMath
