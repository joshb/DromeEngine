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

#include <cstring>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <stdlib.h>
	#include <sys/time.h>
#endif /* _WIN32 */
#include <DromeCore/Util.h>

namespace DromeCore {

unsigned char
randomUChar()
{
	return randomUInt() & 0xff;
}

unsigned int
randomUInt()
{
#ifndef _WIN32
	return random();
#else
	return rand();
#endif /* _WIN32 */
}

float
randomFloat(float min, float max)
{
	return min + (max - min) * ((float)randomUInt() / (float)RAND_MAX);
}

int
strCaseCmp(const char *s1, const char *s2)
{
#ifdef _WIN32
	return strcmpi(s1, s2);
#else
	return strcasecmp(s1, s2);
#endif /* _WIN32 */
}

int
getTicks()
{
	static bool first_run = true;

#ifndef _WIN32
	static struct timeval start;
	struct timeval t;

	gettimeofday(&t, NULL);
	if(first_run) {
		start.tv_sec = t.tv_sec;
		start.tv_usec = t.tv_usec;
		first_run = false;
	}

	return ((t.tv_sec - start.tv_sec) * 1000) + (t.tv_usec / 1000);
#else
	int start;
	int t = GetTickCount();

	if(first_run) {
		start = t;
		first_run = false;
	}

	return t - start;
#endif /* _WIN32 */
}

} // namespace DromeCore
