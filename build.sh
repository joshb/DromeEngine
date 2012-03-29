#!/bin/sh

cd `dirname $0`

# remove/create the build directory if necessary
if [ "$1" = "clean" ]; then
	rm -rf build 2> /dev/null
fi
mkdir build 2> /dev/null

# build
cd build
cmake .. && make
