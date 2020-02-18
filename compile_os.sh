#!/bin/sh
rm -rf /tmp/prism-osMakeMake/*
make -f mainMake.mk lib SKETCH=framework/prism/entry.ino BUILD_ROOT=/tmp/prism-osMakeMake DEBUG=1
cp /tmp/prism-osMakeMake/entry_nodemcuv2/entry.a ./framework/main.debug.a
rm -rf /tmp/prism-osMakeMake/*
make -f mainMake.mk lib SKETCH=framework/prism/entry.ino BUILD_ROOT=/tmp/prism-osMakeMake
cp /tmp/prism-osMakeMake/entry_nodemcuv2/entry.a ./framework/main.a
rm -rf /tmp/prism-osMakeMake/*
