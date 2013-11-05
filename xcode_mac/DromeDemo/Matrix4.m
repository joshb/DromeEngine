/*
 * Copyright (C) 2013 Josh A. Beam
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

#import "Matrix4.h"

@interface Matrix4()
{
    float _matrix[16];
}

@end

@implementation Matrix4

- (id)init
{
    if((self = [super init])) {
        const float identityData[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        memcpy(_matrix, identityData, sizeof(_matrix));
    }
    
    return self;
}

- (id)initWithData:(const float *)data
{
    if((self = [super init])) {
        memcpy(_matrix, data, sizeof(_matrix));
    }
    
    return self;
}

- (const float *)data
{
    return _matrix;
}

- (float)getValueForIndex:(int)index
{
    if(index < 0 || index > 15) {
        NSLog(@"Invalid matrix element index %d; must be between 0 and 15", index);
        return 0.0f;
    }
    
    return _matrix[index];
}

- (void)setValue:(float)value forIndex:(int)index
{
    if(index < 0 || index > 15) {
        NSLog(@"Invalid matrix element index %d; must be between 0 and 15", index);
    } else {
        _matrix[index] = value;
    }
}

- (Matrix4 *)multiplyWithMatrix:(Matrix4 *)matrix
{
    float m[16];
    const float *m1 = matrix.data;
    const float *m2 = self.data;
    
    m[ 0] = m1[ 0]*m2[ 0] + m1[ 1]*m2[ 4] + m1[ 2]*m2[ 8] + m1[ 3]*m2[12];
    m[ 1] = m1[ 0]*m2[ 1] + m1[ 1]*m2[ 5] + m1[ 2]*m2[ 9] + m1[ 3]*m2[13];
    m[ 2] = m1[ 0]*m2[ 2] + m1[ 1]*m2[ 6] + m1[ 2]*m2[10] + m1[ 3]*m2[14];
    m[ 3] = m1[ 0]*m2[ 3] + m1[ 1]*m2[ 7] + m1[ 2]*m2[11] + m1[ 3]*m2[15];
    m[ 4] = m1[ 4]*m2[ 0] + m1[ 5]*m2[ 4] + m1[ 6]*m2[ 8] + m1[ 7]*m2[12];
    m[ 5] = m1[ 4]*m2[ 1] + m1[ 5]*m2[ 5] + m1[ 6]*m2[ 9] + m1[ 7]*m2[13];
    m[ 6] = m1[ 4]*m2[ 2] + m1[ 5]*m2[ 6] + m1[ 6]*m2[10] + m1[ 7]*m2[14];
    m[ 7] = m1[ 4]*m2[ 3] + m1[ 5]*m2[ 7] + m1[ 6]*m2[11] + m1[ 7]*m2[15];
    m[ 8] = m1[ 8]*m2[ 0] + m1[ 9]*m2[ 4] + m1[10]*m2[ 8] + m1[11]*m2[12];
    m[ 9] = m1[ 8]*m2[ 1] + m1[ 9]*m2[ 5] + m1[10]*m2[ 9] + m1[11]*m2[13];
    m[10] = m1[ 8]*m2[ 2] + m1[ 9]*m2[ 6] + m1[10]*m2[10] + m1[11]*m2[14];
    m[11] = m1[ 8]*m2[ 3] + m1[ 9]*m2[ 7] + m1[10]*m2[11] + m1[11]*m2[15];
    m[12] = m1[12]*m2[ 0] + m1[13]*m2[ 4] + m1[14]*m2[ 8] + m1[15]*m2[12];
    m[13] = m1[12]*m2[ 1] + m1[13]*m2[ 5] + m1[14]*m2[ 9] + m1[15]*m2[13];
    m[14] = m1[12]*m2[ 2] + m1[13]*m2[ 6] + m1[14]*m2[10] + m1[15]*m2[14];
    m[15] = m1[12]*m2[ 3] + m1[13]*m2[ 7] + m1[14]*m2[11] + m1[15]*m2[15];
    
    return [[Matrix4 alloc] initWithData:m];
}

+ (Matrix4 *)perspectiveMatrixWithFieldOfView:(float)fov aspect:(float)aspect near:(float)near far:(float)far
{
    Matrix4 *matrix = [[Matrix4 alloc] init];
    float f = 1.0f / tanf(fov / 2.0f);
    
    [matrix setValue:(f / aspect) forIndex:0];
    [matrix setValue:f forIndex:5];
    [matrix setValue:((far + near) / (near - far)) forIndex:10];
    [matrix setValue:-1.0f forIndex:11];
    [matrix setValue:((2.0f * far * near) / (near - far)) forIndex:14];
    [matrix setValue:0.0f forIndex:15];
    
    return matrix;
}

+ (Matrix4 *)translationMatrixWithX:(float)x y:(float)y z:(float)z
{
    Matrix4 *matrix = [[Matrix4 alloc] init];
    [matrix setValue:x forIndex:12];
    [matrix setValue:y forIndex:13];
    [matrix setValue:z forIndex:14];
    return matrix;
}

+ (Matrix4 *)rotationMatrixWithAngle:(float)angle x:(float)x y:(float)y z:(float)z
{
    Matrix4 *matrix = [[Matrix4 alloc] init];
    
    float c = cosf(angle);
    float ci = 1.0f - c;
    float s = sinf(angle);
    
    float xy = x * y * ci;
    float xz = x * z * ci;
    float yz = y * z * ci;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;
    
    [matrix setValue:(x * x * ci + c) forIndex:0];
    [matrix setValue:(xy + zs) forIndex:1];
    [matrix setValue:(xz - ys) forIndex:2];
    [matrix setValue:(xy - xz) forIndex:4];
    [matrix setValue:(y * y * ci + c) forIndex:5];
    [matrix setValue:(yz + xs) forIndex:6];
    [matrix setValue:(xz + ys) forIndex:8];
    [matrix setValue:(yz - xs) forIndex:9];
    [matrix setValue:(z * z * ci + c) forIndex:10];
 
    return matrix;
}

@end
