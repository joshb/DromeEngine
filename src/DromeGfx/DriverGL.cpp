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

#include <DromeCore/Exception.h>
#include <DromeGfx/OpenGL.h>
#include <DromeGfx/DriverGL.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGfx {

/*
 * OpenGL Extensions
 */
#if 0
// ARB_multitexture
static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

// ARB_vertex_buffer_object
static PFNGLBINDBUFFERARBPROC glBindBufferARB;
static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
static PFNGLGENBUFFERSARBPROC glGenBuffersARB;
static PFNGLBUFFERDATAARBPROC glBufferDataARB;

// ARB_shader_objects
static PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
static PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
static PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
static PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
static PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
static PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
static PFNGLUNIFORM1IARBPROC glUniform1iARB;
static PFNGLUNIFORM3FARBPROC glUniform3fARB;
static PFNGLUNIFORM3FVARBPROC glUniform3fvARB;

typedef void (*PFNGLPROGRAMPARAMETERIEXT)(GLuint, GLenum, GLint);
static PFNGLPROGRAMPARAMETERIEXT glProgramParameteriEXT;
#endif

#define SET_FUNC_POINTER(FUNCNAME, FUNCTYPE) \
	do { \
		FUNCNAME = (FUNCTYPE)io_context->getProcAddress(#FUNCNAME); \
		if(!FUNCNAME) \
			throw Exception("GfxDriverGL: " #FUNCNAME " failed"); \
	} while(0)

static void
loadExtensions()
{
#if 0
	// ARB_multitexture
	SET_FUNC_POINTER(glActiveTextureARB, PFNGLACTIVETEXTUREARBPROC);
	SET_FUNC_POINTER(glClientActiveTextureARB, PFNGLCLIENTACTIVETEXTUREARBPROC);

	// ARB_vertex_buffer_object
	SET_FUNC_POINTER(glBindBufferARB, PFNGLBINDBUFFERARBPROC);
	SET_FUNC_POINTER(glDeleteBuffersARB, PFNGLDELETEBUFFERSARBPROC);
	SET_FUNC_POINTER(glGenBuffersARB, PFNGLGENBUFFERSARBPROC);
	SET_FUNC_POINTER(glBufferDataARB, PFNGLBUFFERDATAARBPROC);

	// ARB_shader_objects
	SET_FUNC_POINTER(glDeleteObjectARB, PFNGLDELETEOBJECTARBPROC);
	SET_FUNC_POINTER(glCreateShaderObjectARB, PFNGLCREATESHADEROBJECTARBPROC);
	SET_FUNC_POINTER(glShaderSourceARB, PFNGLSHADERSOURCEARBPROC);
	SET_FUNC_POINTER(glCompileShaderARB, PFNGLCOMPILESHADERARBPROC);
	SET_FUNC_POINTER(glCreateProgramObjectARB, PFNGLCREATEPROGRAMOBJECTARBPROC);
	SET_FUNC_POINTER(glAttachObjectARB, PFNGLATTACHOBJECTARBPROC);
	SET_FUNC_POINTER(glLinkProgramARB, PFNGLLINKPROGRAMARBPROC);
	SET_FUNC_POINTER(glUseProgramObjectARB, PFNGLUSEPROGRAMOBJECTARBPROC);
	SET_FUNC_POINTER(glGetObjectParameterivARB, PFNGLGETOBJECTPARAMETERIVARBPROC);
	SET_FUNC_POINTER(glGetInfoLogARB, PFNGLGETINFOLOGARBPROC);
	SET_FUNC_POINTER(glGetUniformLocationARB, PFNGLGETUNIFORMLOCATIONARBPROC);
	SET_FUNC_POINTER(glUniform1iARB, PFNGLUNIFORM1IARBPROC);
	SET_FUNC_POINTER(glUniform3fARB, PFNGLUNIFORM3FARBPROC);
	SET_FUNC_POINTER(glUniform3fvARB, PFNGLUNIFORM3FVARBPROC);

	SET_FUNC_POINTER(glProgramParameteriEXT, PFNGLPROGRAMPARAMETERIEXT);
#endif
}

/*
 * GfxDriverGL
 */
GfxDriverGL::GfxDriverGL()
{
	loadExtensions();

	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef GLES
	glClearDepthf(1.0f);
#else
	glClearDepth(1.0f);
#endif /* GLES */
	clearBuffers();

	// set default blending and depth modes
	glEnable(GL_BLEND);
	setBlendMode(BLEND_MODE_ALPHA);
	setDepthWritesEnabled(true);
	setDepthMode(DEPTH_MODE_LESS_OR_EQUAL);

	// enable backface culling
	setCullFace(CULL_FACE_BACK);
	glFrontFace(GL_CCW);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	flush();
}

GfxDriverGL::~GfxDriverGL()
{
}

void
GfxDriverGL::flush()
{
	glFinish();
}

void
GfxDriverGL::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
GfxDriverGL::setWireframe(bool enabled)
{
#ifdef GLES
	throw Exception("GfxDriverGL::setWireframe(): Function not supported on this platform");
#else
	glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
#endif /* GLES */
}

bool
GfxDriverGL::getDepthWritesEnabled() const
{
	return m_depthWritesEnabled;
}

void
GfxDriverGL::setDepthWritesEnabled(bool value)
{
	glDepthMask(value);
	m_depthWritesEnabled = value;
}

DepthMode
GfxDriverGL::getDepthMode() const
{
	return m_depthMode;
}

void
GfxDriverGL::setDepthMode(DepthMode value)
{
	switch(value) {
		default:
			throw Exception("Unsupported depth mode");
			break;
		case DEPTH_MODE_NONE:
			glDisable(GL_DEPTH_TEST);
			break;
		case DEPTH_MODE_LESS:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			break;
		case DEPTH_MODE_LESS_OR_EQUAL:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			break;
	}

	m_depthMode = value;
}

BlendMode
GfxDriverGL::getBlendMode() const
{
	return m_blendMode;
}

void
GfxDriverGL::setBlendMode(BlendMode value)
{
	switch(value) {
		default:
			throw Exception("Unsupported depth mode");
			break;
		case BLEND_MODE_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_MODE_ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
	}

	m_blendMode = value;
}

void
GfxDriverGL::setCullFace(CullFace value)
{
	switch(value) {
		default:
			glDisable(GL_CULL_FACE);
			break;
		case CULL_FACE_BACK:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case CULL_FACE_FRONT:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
	}
}

/*
 * Matrix functions
 */
Matrix4
GfxDriverGL::getProjectionMatrix() const
{
	float m[16];
	glGetFloatv(GL_PROJECTION_MATRIX, m);

	return Matrix4(m);
}

void
GfxDriverGL::setProjectionMatrix(const Matrix4 &value)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value.getData());
	glMatrixMode(GL_MODELVIEW);
}

Matrix4
GfxDriverGL::getModelViewMatrix() const
{
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	return Matrix4(m);
}

void
GfxDriverGL::setModelViewMatrix(const Matrix4 &value)
{
	glLoadMatrixf(value.getData());
}

Matrix4
GfxDriverGL::getTextureMatrix() const
{
	float m[16];
	glGetFloatv(GL_TEXTURE_MATRIX, m);

	return Matrix4(m);
}

void
GfxDriverGL::setTextureMatrix(const Matrix4 &value)
{
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(value.getData());
	glMatrixMode(GL_MODELVIEW);
}

void
GfxDriverGL::setViewportDimensions(int width, int height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(0, 0, width, height);
}

void
GfxDriverGL::enable2D()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(Matrix4::orthographic(0.0f, m_viewportWidth, m_viewportHeight, 0.0f).getData());
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void
GfxDriverGL::disable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void
GfxDriverGL::bindTexture(uint8_t textureUnit, RefPtr <Texture> texture)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);

	if(!texture) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, texture->getId());
}

void
GfxDriverGL::bindFramebuffer(RefPtr <Framebuffer> framebuffer)
{
#ifdef GLES
	throw Exception("GfxDriverGL::bindFramebuffer(): Function not supported on this platform");
#else
	if(!framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFramebufferId());
		glViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight());
	}
#endif /* GLES */
}

void
GfxDriverGL::bindShaderProgram(RefPtr <ShaderProgram> shaderProgram)
{
#ifdef GLES
	throw Exception("GfxDriverGL::bindShaderProgram(): Function not supported on this platform");
#else
	if(!shaderProgram) {
		glUseProgram(0);
		return;
	}

	glUseProgram(shaderProgram->getId());
#endif /* GLES */
}

/*
 * Rendering functions
 */
void
GfxDriverGL::drawPic(RefPtr <Texture> texture, Color color,
                     const Rect2i &src, const Rect2i &dest)
{
	GfxDriver::bindTexture(texture);

	glColor4ub(color.r, color.g, color.b, color.a);

	short verts[] = {
		dest.min.x, dest.min.y,
		dest.min.x, dest.max.y,
		dest.max.x, dest.min.y,
		dest.max.x, dest.max.y,
	};

	if(!texture) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_SHORT, 0, verts);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glVertexPointer(2, GL_SHORT, 0, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	} else {
		float texCoords[] = {
			(float)src.min.x / (float)texture->getWidth(), (float)src.min.y / (float)texture->getHeight(),
			(float)src.min.x / (float)texture->getWidth(), (float)src.max.y / (float)texture->getHeight(),
			(float)src.max.x / (float)texture->getWidth(), (float)src.min.y / (float)texture->getHeight(),
			(float)src.max.x / (float)texture->getWidth(), (float)src.max.y / (float)texture->getHeight()
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_SHORT, 0, verts);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_SHORT, 0, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

} // namespace DromeGfx
