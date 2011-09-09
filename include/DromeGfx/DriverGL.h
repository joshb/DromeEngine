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

#ifndef __DROMEGFX_DRIVER_GL_H__
#define __DROMEGFX_DRIVER_GL_H__

#include "Driver.h"

namespace DromeGfx {

class GfxDriverGL : public GfxDriver {
	protected:
		int m_viewportWidth, m_viewportHeight;
		bool m_depthWritesEnabled;
		DepthMode m_depthMode;
		BlendMode m_blendMode;

	public:
		GfxDriverGL();
		virtual ~GfxDriverGL();

		void flush();

		void clearBuffers();
		void setWireframe(bool enabled);

		bool getDepthWritesEnabled() const;
		void setDepthWritesEnabled(bool value);

		DepthMode getDepthMode() const;
		void setDepthMode(DepthMode value);

		BlendMode getBlendMode() const;
		void setBlendMode(BlendMode value);

		void setCullFace(CullFace value);

	/* Matrix functions */
		DromeMath::Matrix4 getProjectionMatrix() const;
		void setProjectionMatrix(const DromeMath::Matrix4 &m);

		DromeMath::Matrix4 getModelViewMatrix() const;
		void setModelViewMatrix(const DromeMath::Matrix4 &m);

		DromeMath::Matrix4 getTextureMatrix() const;
		void setTextureMatrix(const DromeMath::Matrix4 &m);

		void setViewportDimensions(int width, int height);
		void enable2D();
		void disable2D();

		void bindTexture(uint8_t textureUnit, DromeCore::RefPtr <Texture> texture);
		void bindFramebuffer(DromeCore::RefPtr <Framebuffer> framebuffer);
		void bindShaderProgram(DromeCore::RefPtr <ShaderProgram> shaderProgram);

	/* Rendering functions */
		void drawPic(DromeCore::RefPtr <Texture> texture, Color color, const DromeMath::Rect2i &src, const DromeMath::Rect2i &dest);
};

} // namespace DromeGfx

#endif /* __DROMEGFX_DRIVER_GL_H__ */
