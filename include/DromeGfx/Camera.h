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

#ifndef __DROMEGFX_CAMERA_H__
#define __DROMEGFX_CAMERA_H__

#include <DromeMath/DromeMath>

namespace DromeGfx {

/**
 * The Camera class generates matrices and view frustums based on its position and orientation.
 */
class Camera {
	protected:
		DromeMath::Quaternion m_orientation;
		DromeMath::Matrix4 m_matrix;
		DromeMath::Vector3 m_position;
		DromeMath::Vector3 m_translation;
		float m_yaw, m_pitch, m_roll;

	public:
		Camera();

		/**
		 * Resets Camera's position/orientation, matrix, and view frustum to the defaults.
		 */
		void clear();

		/**
		 * Performs a yaw rotation.
		 *
		 * @param theta Number of degrees to rotate.
		 */
		void rotateYaw(float theta);

		/**
		 * Performs a roll rotation.
		 *
		 * @param theta Number of degrees to rotate.
		 */
		void rotateRoll(float theta);

		/**
		 * Performs a pitch rotation.
		 *
		 * @param theta Number of degrees to rotate.
		 */
		void rotatePitch(float theta);

		/**
		 * Moves the Camera.
		 *
		 * @param v Vector to add to Camera's position.
		 */
		void move(const DromeMath::Vector3 &v);

		/**
		 * Sets the Camera to look in the given direction.
		 *
		 * @param direction The direction to set the Camera to look in.
		 * @param roll The roll rotation angle (in degrees) around the direction vector.
		 */
		void lookInDirection(const DromeMath::Vector3 &direction, float roll = 0.0f);

		/**
		 * Sets the Camera to look at the given point.
		 *
		 * @param point The point to set the Camera to look at.
		 * @param roll The roll rotation angle (in degrees) around the direction vector.
		 */
		void lookAt(const DromeMath::Vector3 &point, float roll = 0.0f);

		/**
		 * Updates the Camera's view matrix.
		 */
		void update();

		/**
		 * @return Reference to Camera's view matrix.
		 */
		inline const DromeMath::Matrix4 &getMatrix() const { return m_matrix; }

		/**
		 * Sets the Camera's translation Vector, which is added to the Camera's position Vector during Matrix generation. This is handy if, for example, the Camera view should be a bit above the ground if its Z position is 0.
		 *
		 * @param v Translation Vector.
		 */
		inline void setTranslation(const DromeMath::Vector3 &v) { m_translation = v; }
		/**
		 * @return Reference to Camera's translation vector.
		 */
		inline const DromeMath::Vector3 &getTranslation() const { return m_translation; }

		/**
		 * Sets the Camera's position Vector.
		 *
		 * @param v Position Vector.
		 */
		inline void setPosition(const DromeMath::Vector3 &v) { m_position = v; }
		/**
		 * @return Reference to Camera's position Vector.
		 */
		inline const DromeMath::Vector3 &getPosition() const { return m_position; }

		/**
		 * Sets the Camera's yaw rotation.
		 *
		 * @param f Yaw rotation, in degrees.
		 */
		inline void setYaw(float f) { m_yaw = f; }
		/**
		 * @return Camera's yaw rotation, in degrees.
		 */
		inline float getYaw() const { return m_yaw; }

		/**
		 * Sets the Camera's pitch rotation.
		 *
		 * @param f Pitch rotation, in degrees.
		 */
		inline void setPitch(float f) { m_pitch = f; }
		/**
		 * @return Camera's pitch rotation, in degrees.
		 */
		inline float getPitch() const { return m_pitch; }

		/**
		 * Sets the Camera's roll rotation.
		 *
		 * @param f Roll rotation, in degrees.
		 */
		inline void setRoll(float f) { m_roll = f; }
		/**
		 * @return Camera's roll rotation, in degrees.
		 */
		inline float getRoll() const { return m_roll; }
};

} // namespace DromeGfx

#endif /* __DROMEGFX_CAMERA_H__ */
