#!/bin/bash
set -e

scriptdir=${0%`basename "$0"`}
cd $scriptdir
scriptdir=`pwd`

if [ -d build ]; then
	make -C build VERBOSE=1 -j clean
fi
