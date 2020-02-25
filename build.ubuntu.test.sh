#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> magnet"

cmdX(){
	if ! "$@" ; then
		echo "Error: test"
		exit 1
	fi
}

cmdX magnet --execution-time test/test.0001.js
