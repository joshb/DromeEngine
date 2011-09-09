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

#ifndef __DROMECORE_ENDIAN_H__
#define __DROMECORE_ENDIAN_H__

#include <stdint.h>

namespace DromeCore {

enum Endianness {
	ENDIANNESS_UNKNOWN = 0,
	ENDIANNESS_LITTLE,
	ENDIANNESS_BIG
};

Endianness getEndianness();

float littleToNativeFloat(float f);
int32_t littleToNativeInt32(int32_t i);
uint32_t littleToNativeUInt32(uint32_t i);
int16_t littleToNativeInt16(int16_t s);
uint16_t littleToNativeUInt16(uint16_t s);

inline float nativeToLittleFloat(float f) { return littleToNativeFloat(f); }
inline int32_t nativeToLittleInt32(int32_t i) { return littleToNativeInt32(i); }
inline uint32_t nativeToLittleUInt32(uint32_t i) { return littleToNativeUInt32(i); }
inline int16_t nativeToLittleInt16(int16_t s) { return littleToNativeInt16(s); }
inline uint16_t nativeToLittleUInt16(uint16_t s) { return littleToNativeUInt16(s); }

float bigToNativeFloat(float f);
int32_t bigToNativeInt32(int32_t i);
uint32_t bigToNativeUInt32(uint32_t i);
int16_t bigToNativeInt16(int16_t s);
uint16_t bigToNativeUInt16(uint16_t s);

inline float nativeToBigFloat(float f) { return bigToNativeFloat(f); }
inline int32_t nativeToBigInt32(int32_t i) { return bigToNativeInt32(i); }
inline uint32_t nativeToBigUInt32(uint32_t i) { return bigToNativeUInt32(i); }
inline int16_t nativeToBigInt16(int16_t s) { return bigToNativeInt16(s); }
inline uint16_t nativeToBigUInt16(uint16_t s) { return bigToNativeUInt16(s); }

} // namespace DromeCore

#endif /* __DROMECORE_ENDIAN_H__ */
