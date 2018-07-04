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

#include <DromeMath/BoundingBox.h>
#include <DromeMath/BoundingSphere.h>

namespace DromeMath {

BoundingBox::BoundingBox(const Vector3 &position, const Vector3 &bounds)
{
	m_position = position;
	m_bounds = bounds;
}

Vector3
BoundingBox::getBounds() const
{
	return m_bounds;
}

void
BoundingBox::setBounds(const Vector3 &value)
{
	m_bounds = value;
}

PhysicsObjectType
BoundingBox::getType() const
{
	return PHYSICS_OBJECT_TYPE_BOUNDING_BOX;
}

static bool
pointInBox(const Vector3 &point,
           const Vector3 &boxPosition, const Vector3 &boxBounds)
{
	return (point.x > (boxPosition.x - boxBounds.x)) &&
	       (point.x < (boxPosition.x + boxBounds.x)) &&
	       (point.y > (boxPosition.y - boxBounds.y)) &&
	       (point.y < (boxPosition.y + boxBounds.y)) &&
	       (point.z > (boxPosition.z - boxBounds.z)) &&
	       (point.z < (boxPosition.z + boxBounds.z));
}

CollisionInfo
BoundingBox::collisionCheck(const BoundingBox *box) const
{
	Vector3 otherPosition = box->getPosition();
	Vector3 otherBounds = box->getBounds();

	CollisionInfo info(false);
	bool infoSet = false;
	float dist;

	// +x
	dist = (m_position.x + m_bounds.x) - (otherPosition.x - otherBounds.x);
	if(dist < 0.0f) {
		return info;
	} else {
		info.direction = Vector3(-1.0f, 0.0f, 0.0f);
		info.distance = dist;
		infoSet = true;
	}

	// -x
	dist = (otherPosition.x + otherBounds.x) - (m_position.x - m_bounds.x);
	if(dist < 0.0f) {
		return info;
	} else {
		if(!infoSet || dist < info.distance) {
			info.direction = Vector3(1.0f, 0.0f, 0.0f);
			info.distance = dist;
			infoSet = true;
		}
	}

	// +y
	dist = (m_position.y + m_bounds.y) - (otherPosition.y - otherBounds.y);
	if(dist < 0.0f) {
		return info;
	} else {
		if(!infoSet || dist < info.distance) {
			info.direction = Vector3(0.0f, -1.0f, 0.0f);
			info.distance = dist;
			infoSet = true;
		}
	}

	// -y
	dist = (otherPosition.y + otherBounds.y) - (m_position.y - m_bounds.y);
	if(dist < 0.0f) {
		return info;
	} else {
		if(!infoSet || dist < info.distance) {
			info.direction = Vector3(0.0f, 1.0f, 0.0f);
			info.distance = dist;
			infoSet = true;
		}
	}

	// +z
	dist = (m_position.z + m_bounds.z) - (otherPosition.z - otherBounds.z);
	if(dist < 0.0f) {
		return info;
	} else {
		if(!infoSet || dist < info.distance) {
			info.direction = Vector3(0.0f, 0.0f, -1.0f);
			info.distance = dist;
			infoSet = true;
		}
	}

	// -z
	dist = (otherPosition.z + otherBounds.z) - (m_position.z - m_bounds.z);
	if(dist < 0.0f) {
		return info;
	} else {
		if(!infoSet || dist < info.distance) {
			info.direction = Vector3(0.0f, 0.0f, 1.0f);
			info.distance = dist;
		}
	}

	info.collision = true;
	return info;

#if 0
	return CollisionInfo(
	       ((otherPosition.x - otherBounds.x) < (m_position.x + m_bounds.x)) &&
	       ((otherPosition.x + otherBounds.x) > (m_position.x - m_bounds.x)) &&
	       ((otherPosition.y - otherBounds.y) < (m_position.y + m_bounds.y)) &&
	       ((otherPosition.y + otherBounds.y) > (m_position.y - m_bounds.y)) &&
	       ((otherPosition.z - otherBounds.z) < (m_position.z + m_bounds.z)) &&
	       ((otherPosition.z + otherBounds.z) > (m_position.z - m_bounds.z))
	);
#endif
}

CollisionInfo
BoundingBox::collisionCheck(const BoundingSphere *sphere) const
{
	CollisionInfo info = sphere->collisionCheck(this);
	return CollisionInfo(info.collision, info.direction * -1.0f);
}

CollisionInfo
BoundingBox::collisionCheck(const PhysicsObject *object) const
{
	PhysicsObjectType otherType = object->getType();
	switch(otherType) {
		default:
			return PhysicsObject::collisionCheck(object);
		case PHYSICS_OBJECT_TYPE_POINT:
			return pointInBox(object->getPosition(), m_position, m_bounds);
		case PHYSICS_OBJECT_TYPE_BOUNDING_BOX:
			return collisionCheck((const BoundingBox *)object);
	}
}

} // namespace DromeMath
