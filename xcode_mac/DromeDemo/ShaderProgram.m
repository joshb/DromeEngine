/*
 * Copyright (C) 2011-2013 Josh A. Beam
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

#import <OpenGL/gl3.h>
#import "ShaderProgram.h"

// shader functions defined in shader.c
extern void shaderAttachFromFile(GLuint, GLenum, const char *);

@interface ShaderProgram()
{
    GLuint _program;
}

@end

@implementation ShaderProgram

- (id)init
{
    if((self = [super init])) {
        _program = glCreateProgram();
    }
    
    return self;
}

- (void)attachShaderWithType:(GLenum)type
                    fromFile:(NSString *)filePath
{
	NSString *fullPath = [[[[NSBundle mainBundle] resourcePath]
                           stringByAppendingString:@"/"] stringByAppendingString:filePath];
	shaderAttachFromFile(_program, type, fullPath.UTF8String);
}

- (void)linkProgram
{
    GLint result;
    
    // link the program and make sure that there were no errors
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	if(result == GL_FALSE) {
		GLint length;
		char *log;
        
		// get the program info log
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
		log = malloc(length);
		glGetProgramInfoLog(_program, length, &result, log);
        
		// print an error message with the info log
        NSLog(@"Program linking failed: %s", log);
		free(log);
	}
}

- (GLint)getLocationOfAttributeWithName:(NSString *)name
{
    GLint tmp = glGetAttribLocation(_program, name.UTF8String);
    if(tmp == -1)
        NSLog(@"No attribute named '%@' exists in the program", name);
    return tmp;
}

- (GLint)getLocationOfUniformWithName:(NSString *)name
{
    GLint tmp = glGetUniformLocation(_program, name.UTF8String);
    if(tmp == -1)
        NSLog(@"No uniform named '%@' exists in the program", name);
    return tmp;
}

- (void)useProgram
{
    glUseProgram(_program);
}

- (void)dealloc
{
    glDeleteProgram(_program);
}

@end
