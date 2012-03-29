#!/bin/sh

cd `dirname $0`
./build.sh

# execute DromeDemo if the build was successful
if [ $? -eq 0 ]; then
	if [ -d demo/DromeDemo.app ]; then
		./demo/DromeDemo.app/Contents/MacOS/DromeDemo
	else
		./demo/DromeDemo
	fi
fi
