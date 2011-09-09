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

#ifndef __DROMEGFX_DRIVER_H__
#define __DROMEGFX_DRIVER_H__

#include <DromeMath/Matrix4.h>
#include <DromeMath/Rect2i.h>
#include "Framebuffer.h"
#include "Image.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Types.h"
#include "VertexBuffer.h"

namespace DromeGfx {

class GfxDriver {
	public:
		GfxDriver() { }
		virtual ~GfxDriver() { }

		virtual void flush() = 0;

		virtual void clearBuffers() = 0;
		virtual void setWireframe(bool enabled) = 0;

		virtual bool getDepthWritesEnabled() const = 0;
		virtual void setDepthWritesEnabled(bool value) = 0;

		virtual DepthMode getDepthMode() const = 0;
		virtual void setDepthMode(DepthMode value) = 0;

		virtual BlendMode getBlendMode() const = 0;
		virtual void setBlendMode(BlendMode value) = 0;

		virtual void setCullFace(CullFace value) = 0;

	/* Matrix functions */
		virtual DromeMath::Matrix4 getProjectionMatrix() const = 0;
		virtual void setProjectionMatrix(const DromeMath::Matrix4 &value) = 0;
		void setProjectionMatrix(int windowWidth, int windowHeight);

		virtual DromeMath::Matrix4 getModelViewMatrix() const = 0;
		virtual void setModelViewMatrix(const DromeMath::Matrix4 &value) = 0;

		virtual DromeMath::Matrix4 getTextureMatrix() const = 0;
		virtual void setTextureMatrix(const DromeMath::Matrix4 &value) = 0;

		virtual void setViewportDimensions(int width, int height) = 0;
		virtual void enable2D() = 0;
		virtual void disable2D() = 0;

		virtual void bindTexture(uint8_t textureUnit, DromeCore::RefPtr <Texture> texture) = 0;
		virtual void bindTexture(DromeCore::RefPtr <Texture> texture);
		virtual void bindFramebuffer(DromeCore::RefPtr <Framebuffer> framebuffer) = 0;
		virtual void bindShaderProgram(DromeCore::RefPtr <ShaderProgram> shaderProgram) = 0;

	/* Rendering functions */
		virtual void drawPic(DromeCore::RefPtr <Texture> texture, Color color, const DromeMath::Rect2i &src, const DromeMath::Rect2i &dest) = 0;

		static GfxDriver *create();
};

} // namespace DromeGfx

#endif /* __DROMEGFX_DRIVER_H__ */
