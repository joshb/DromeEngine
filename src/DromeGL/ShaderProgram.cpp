/*
 * Copyright (C) 2010-2012 Josh A. Beam
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

#include <cstdio>
#include <cstring>
#include <DromeCore/Exception.h>
#include <DromeCore/File.h>
#include <DromeGL/ShaderProgram.h>

using namespace std;
using namespace DromeCore;
using namespace DromeMath;

namespace DromeGL {

static string
loadShaderFromFile(const char *filename)
{
	string source;
	char tmp[512];
	size_t length;
	FILE *fp;

	// open shader
	fp = fopen(File::getPath(filename).c_str(), "r");
	if(!fp)
		throw Exception(string("loadShaderFromFile(): Couldn't open ") + filename);

	// read shader into string
	while((length = fread(tmp, 1, sizeof(tmp) - 1, fp)) > 0) {
		tmp[length] = '\0';
		source += tmp;
	}

	fclose(fp);

	return source;
}

ShaderProgram::ShaderProgram()
{
	m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

GLuint
ShaderProgram::getId() const
{
	return m_id;
}

static GLuint
compileShader(GLenum type, const char *shaderSource)
{
	GLuint shader = glCreateShader(type);

	// set shader source
	GLint length = (GLint)strlen(shaderSource);
	glShaderSource(shader, 1, &shaderSource, &length);

	// compile the shader
	glCompileShader(shader);

	// make sure the compilation was successful
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		char *log = new char[length];
		glGetShaderInfoLog(shader, length, &length, log);

		throw Exception(string("compileShader(): Shader compilation failed: ") + log);
		delete [] log;
	}

	return shader;
}

static void
attachShader(GLuint programId, GLenum type, const char *shaderSource)
{
	// compile the shader
	GLuint shader = compileShader(type, shaderSource);

	// attach the shader to the program
	glAttachShader(programId, shader);

	// call delete on the shader so that it will
	// be deleted when detached from the program
	glDeleteShader(shader);
}

void
ShaderProgram::attachVertexShader(const char *shader)
{
	attachShader(m_id, GL_VERTEX_SHADER_ARB, shader);
}

void
ShaderProgram::attachVertexShaderFromFile(const char *shaderPath)
{
	string source = loadShaderFromFile(shaderPath);
	attachVertexShader(source.c_str());
}

void
ShaderProgram::attachFragmentShader(const char *shader)
{
	attachShader(m_id, GL_FRAGMENT_SHADER_ARB, shader);
}

void
ShaderProgram::attachFragmentShaderFromFile(const char *shaderPath)
{
	string source = loadShaderFromFile(shaderPath);
	attachFragmentShader(source.c_str());
}

void
ShaderProgram::linkShaders()
{
	// link the program
	glLinkProgram(m_id);

	// make sure the linking was successful
	GLint result;
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if(result == GL_FALSE) {
		int length;

		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

		char *log = new char[length];
		glGetProgramInfoLog(m_id, length, &length, log);

		throw Exception(string("ShaderProgram::linkShaders(): Program linking failed: ") + log);
		delete [] log;
	}
}

GLint
ShaderProgram::getUniformLocation(const char *name) const
{
	GLint location = glGetUniformLocation(m_id, name);
	if(location == -1)
		throw Exception(string("ShaderProgram::getUniformLocation(): Uniform variable '") + name + "' does not exist");

	return location;
}

void
ShaderProgram::setUniform(const char *name, int value)
{
	glUniform1iARB(getUniformLocation(name), value);
}

void
ShaderProgram::setUniform(const char *name, const Vector3 *values,
                          int numValues)
{
	glUniform3fvARB(getUniformLocation(name), numValues, (float *)values);
}

void
ShaderProgram::setUniform(const char *name, const Vector3 &value)
{
	setUniform(name, &value, 1);
}

void
ShaderProgram::setUniform(const char *name, const Matrix4 *values,
                          int numValues)
{
	glUniformMatrix4fvARB(getUniformLocation(name), numValues, GL_FALSE, (float *)values);
}

void
ShaderProgram::setUniform(const char *name, const Matrix4 &value)
{
	setUniform(name, &value, 1);
}

GLint
ShaderProgram::getAttribLocation(const char *name) const
{
	GLint location = glGetAttribLocation(m_id, name);
	if(location == -1)
		throw Exception(string("ShaderProgram::getAttribLocation(): Uniform variable '") + name + "' does not exist");

	return location;
}

RefPtr <ShaderProgram>
ShaderProgram::none()
{
	return RefPtr <ShaderProgram> ();
}

RefPtr <ShaderProgram>
ShaderProgram::create()
{
	return RefPtr <ShaderProgram> (new ShaderProgram());
}

} // namespace DromeGfx
