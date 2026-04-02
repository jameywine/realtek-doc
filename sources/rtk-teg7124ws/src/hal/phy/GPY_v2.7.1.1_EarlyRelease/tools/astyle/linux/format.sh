#!/bin/sh

ASTYLE_HOME=`pwd`

cd ../../../src

find . -path ./inc -prune -o -name *.c -print -o -name *.h -print | xargs $ASTYLE_HOME/astyle --options=$ASTYLE_HOME/linux.ini
