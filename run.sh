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

# execute DromeDemo if the build was successful
if [ $? -eq 0 ]; then
	if [ -d demo/DromeDemo.app ]; then
		./demo/DromeDemo.app/Contents/MacOS/DromeDemo
	else
		./demo/DromeDemo
	fi
fi
