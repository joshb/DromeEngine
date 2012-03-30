#!/bin/sh

cd `dirname $0`
./build.sh

# execute DromeDemo if the build was successful
if [ $? -eq 0 ]; then
	cd demo
	../build/demo/DromeDemo
fi
