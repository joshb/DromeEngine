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
#include <DromeCore/Exception.h>
#include <DromeCore/IOContext.h>
#include <DromeCore/Util.h>

using namespace std;

namespace DromeCore {

/*
 * IOContext class
 */
IOContext::IOContext()
{
	m_handler = NULL;
	m_ticks = getTicks();
}

IOContext::~IOContext()
{
}

const EventHandler *
IOContext::getEventHandler() const
{
	return m_handler;
}

void
IOContext::setEventHandler(EventHandler *value)
{
	m_handler = value;
}

int
IOContext::getWindowWidth() const
{
	throw Exception("IOContext::getWindowWidth(): Function not supported on this platform");
}

int
IOContext::getWindowHeight() const
{
	throw Exception("IOContext::getWindowHeight(): Function not supported on this platform");
}

void
IOContext::setWindowDimensions(int /*width*/, int /*height*/)
{
	throw Exception("IOContext::setWindowDimensions(): Function not supported on this platform");
}

bool
IOContext::getFullScreen() const
{
	throw Exception("IOContext::getFullScreen(): Function not supported on this platform");
}

void
IOContext::setFullScreen(bool /*value*/)
{
	throw Exception("IOContext::setFullScreen(): Function not supported on this platform");
}

string
IOContext::getWindowTitle() const
{
	throw Exception("IOContext::getWindowTitle(): Function not supported on this platform");
}

void
IOContext::setWindowTitle(const string & /*value*/)
{
	throw Exception("IOContext::setWindowTitle(): Function not supported on this platform");
}

void
IOContext::cycle()
{
	const float maxSecondsElapsed = 0.1f;

	if(m_handler == NULL)
		return;

	int ticks = getTicks();
	float secondsElapsed = (ticks - m_ticks) / 1000.0f;
	m_ticks = ticks;

	while(secondsElapsed > maxSecondsElapsed) {
		m_handler->cycle(maxSecondsElapsed);
		secondsElapsed -= maxSecondsElapsed;
	}
	m_handler->cycle(secondsElapsed);
}

extern IOContext *io_cocoa_new_context();
extern IOContext *io_sdl_new_context();

IOContext *
IOContext::create(const char *type)
{
#ifdef APPLE
	if(!type)
		type = "cocoa";
	if(strcmp(type, "cocoa") == 0)
		return io_cocoa_new_context();
#endif /* APPLE */
#ifdef SDL_FOUND
	if(!type)
		type = "sdl";
	if(strcmp(type, "sdl") == 0)
		return io_sdl_new_context();
#endif /* SDL_FOUND */

	return NULL;
}

} // namespace DromeCore
