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

#include <DromeGfx/Camera.h>

using namespace DromeMath;

namespace DromeGfx {

Camera::Camera()
{
	clear();
}

void
Camera::clear()
{
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;

	update();
}

void
Camera::rotateYaw(float theta)
{
	m_yaw += theta;
}

void
Camera::rotateRoll(float theta)
{
	m_roll += theta;
}

void
Camera::rotatePitch(float theta)
{
	m_pitch += theta;

	// clamp between -90 and 90 degrees
	if(m_pitch > 90.0f)
		m_pitch = 90.0f;
	else if(m_pitch < -90.0f)
		m_pitch = -90.0f;
}

void
Camera::move(const Vector3 &v)
{
	m_position += v;
}

void
Camera::lookInDirection(const Vector3 &direction, float roll)
{
	const Vector3 standardDir = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 dir = direction.normalize();
	Vector3 diff = dir - standardDir;

	// set yaw/pitch/roll
	Vector3 vyaw = Vector3(diff.x, diff.y, 0.0f).normalize();
	m_yaw = -radToDeg(asinf(vyaw.x));
	if(vyaw.y < 0.0f)
		m_yaw = 180.0f - m_yaw;
	m_pitch = -radToDeg(asinf(standardDir.dotProduct(dir)));
	m_roll = roll;
}

void
Camera::lookAt(const Vector3 &point, float roll)
{
	lookInDirection(point - m_position, roll);
}

void
Camera::update()
{
	// create pitch rotation matrix
	Matrix4 pitch;
	float c = cosf(degToRad(m_pitch));
	float s = sinf(degToRad(m_pitch));
	pitch[5] = c;
	pitch[6] = -s;
	pitch[9] = s;
	pitch[10] = c;

	// create translation matrix
	Vector3 tmp = m_translation - m_position;
	Matrix4 translation;
	translation = translation.translate(tmp);

	// update quaternion and matrix
	m_orientation = Quaternion::fromRotation(Vector3(90.0f, m_yaw, m_roll));
	m_matrix = pitch * m_orientation.toMatrix4() * translation;
}

} // namespace DromeGfx
