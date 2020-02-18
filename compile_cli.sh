#!/bin/sh
cd cli
pyinstaller ./prismtool.py -n prismtool --onefile --distpath ../bin
cd ..
