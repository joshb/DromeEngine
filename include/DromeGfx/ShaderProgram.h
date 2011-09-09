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

#ifndef __DROMEGFX_SHADERPROGRAM_H__
#define __DROMEGFX_SHADERPROGRAM_H__

#include <DromeCore/Ref.h>
#include <DromeMath/Matrix4.h>
#include <DromeMath/Vector3.h>
#include "Types.h"

namespace DromeGfx {

class ShaderProgram : public DromeCore::RefClass
{
	protected:
		unsigned int m_id;

		ShaderProgram();
		virtual ~ShaderProgram();

	public:
		unsigned int getId() const;

		void attachGeometryShader(PrimitiveType inputPrimitiveType, PrimitiveType outputPrimitiveType, int maxOutputVertices, const char *shader);
		void attachGeometryShaderFromFile(PrimitiveType inputPrimitiveType, PrimitiveType outputPrimitiveType, int maxOutputVertices, const char *shaderPath);

		void attachVertexShader(const char *shader);
		void attachVertexShaderFromFile(const char *shaderPath);

		void attachFragmentShader(const char *shader);
		void attachFragmentShaderFromFile(const char *shaderPath);

		void linkShaders();

		int getUniformVariableLocation(const char *name) const;
		void setUniform(const char *name, int value);
		void setUniform(const char *name, const DromeMath::Vector3 *values, int numValues);
		void setUniform(const char *name, const DromeMath::Vector3 &value);
		void setUniform(const char *name, const DromeMath::Matrix4 *values, int numValues);
		void setUniform(const char *name, const DromeMath::Matrix4 &value);

		static DromeCore::RefPtr <ShaderProgram> none();
		static DromeCore::RefPtr <ShaderProgram> create();
};

} // namespace DromeGfx

#endif /* __DROMEGFX_SHADERPROGRAM_H__ */
