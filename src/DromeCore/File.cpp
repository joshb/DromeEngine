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

#include <cstdio>
#include <cstring>
#include <vector>
#include <DromeCore/File.h>
#ifdef _WIN32
	#include <windows.h>
	#define DIR_SEPARATOR "\\"
#else
	#define DIR_SEPARATOR "/"
#endif /* _WIN32 */

using namespace std;

namespace DromeCore {

#ifdef APPLE
extern string getAppleResourcesPath();
#endif /* APPLE */

static vector <string> searchPaths;

void
File::init(int argc, const char **argv)
{
	if(argc > 0) {
		// add directory containing executable to searchPaths
		string path = argv[0];
		size_t tmp = path.find_last_of(DIR_SEPARATOR);
		if(tmp != string::npos) {
			string dir = path.substr(0, tmp);
			addSearchPath(dir);
		}
	}

#ifdef APPLE
	// add application bundle Resources directory
	addSearchPath(getAppleResourcesPath());
#endif /* APPLE */
}

bool
File::exists(const string &filename)
{
	return exists(filename.c_str());
}

bool
File::exists(const char *filename)
{
	// attempt to open file for reading to see if it exists
	FILE *fp = fopen(filename, "r");
	if(fp) {
		fclose(fp);
		return true;
	}

	return false;
}

void
File::addSearchPath(const string &path)
{
	searchPaths.insert(searchPaths.end(), path);
}

void
File::addSearchPath(const char *path)
{
	addSearchPath(string(path));
}

string
File::getPath(const string &filename)
{
#if 0
	// add the default search path if necessary
	static bool firstRun = true;
	if(firstRun) {
#ifdef _WIN32
		// get executable path
		char tmp[4096];
		if(GetModuleFileName(NULL, tmp, sizeof(tmp) - 1) != 0) {
			// remove executable file name from the end of the path
			for(unsigned int i = strlen(tmp) - 1; i > 0; i--) {
				if(tmp[i] == '\\') {
					tmp[i] = '\0';
					break;
				}
			}

			searchPaths.insert(searchPaths.begin(), string(tmp) + string("\\..\\") + string(SHARE_DIR));
		}
#else
		searchPaths.insert(searchPaths.begin(), string(PREFIX) + string(DIR_SEPARATOR) + string(SHARE_DIR));
#endif /* _WIN32 */

		firstRun = false;
	}
#endif

	// search for given filename under all search paths
	for(int i = (int)searchPaths.size() - 1; i >= 0; i--) {
		string path = searchPaths[i] + string(DIR_SEPARATOR) + filename;
		if(exists(path))
			return path;
	}

	// file not found in any of the search paths,
	// so just return the given filename in case
	// it's under the current directory
	return filename;
}

string
File::getPath(const char *filename)
{
	return getPath(string(filename));
}

} // namespace DromeCore
