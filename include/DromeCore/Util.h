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

#ifndef __DROMECORE_UTIL_H__
#define __DROMECORE_UTIL_H__

namespace DromeCore {

/**
 * @return A random unsigned char.
 */
unsigned char randomUChar();

/**
 * @return A random unsigned integer.
 */
unsigned int randomUInt();

/**
 * @param min The minimum boundary of the range to select the random float from.
 * @param max The maximum boundary of the range to select the random float from.
 * @return A random float within the given range.
 */
float randomFloat(float min = -1.0f, float max = 1.0f);

/**
 * Performs a non-case-sensitive comparison of C-style strings.

 * @param s1 The string to compare to the second.
 * @param s2 The string to compare to the first.
 * @return 0 if the strings are equal, a negative integer if s1 is less than s2, or a positive integer if s1 is greater than s2.
 */
int strCaseCmp(const char *s1, const char *s2);

/**
 * Gets the number of milliseconds that have passed since the first call to the function.
 *
 * @return 0 the first time the function is called, and the number of milliseconds that have passed since the first call on subsequent calls.
 */
long getTicks();

} // namespace DromeCore

#endif /* __DROMECORE_UTIL_H__ */
