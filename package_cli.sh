#!/bin/sh
echo "Compiling os binaries..."
./compile_os.sh
echo "Compiling cli binary..."
./compile_cli.sh
echo "Create temp directory to package cli files..."
[ -d /tmp/cliPkgPkg ] && rm -rf /tmp/cliPkgPkg
mkdir /tmp/cliPkgPkg
echo "Copying files..."

cp -R apps /tmp/cliPkgPkg/apps

mkdir /tmp/cliPkgPkg/bin
cp bin/prismtool /tmp/cliPkgPkg/bin/prismtool

mkdir /tmp/cliPkgPkg/framework
cp framework/main.a /tmp/cliPkgPkg/framework/main.a
cp framework/main.debug.a /tmp/cliPkgPkg/framework/main.debug.a
mkdir /tmp/cliPkgPkg/framework/prism
#!--  Copying whitelisted source headers
cp framework/prism/app.hpp /tmp/cliPkgPkg/framework/prism/app.hpp
cp framework/prism/buttons.hpp /tmp/cliPkgPkg/framework/prism/buttons.hpp
cp framework/prism/date_time.hpp /tmp/cliPkgPkg/framework/prism/date_time.hpp
cp framework/prism/graphics.hpp /tmp/cliPkgPkg/framework/prism/graphics.hpp
cp framework/prism/http.hpp /tmp/cliPkgPkg/framework/prism/http.hpp
cp framework/prism/notification.hpp /tmp/cliPkgPkg/framework/prism/notification.hpp
cp framework/prism/timer.hpp /tmp/cliPkgPkg/framework/prism/timer.hpp

cp framework/prism/app_wrapper.hpp /tmp/cliPkgPkg/framework/prism/app_wrapper.hpp
cp framework/prism/apps.hpp /tmp/cliPkgPkg/framework/prism/apps.hpp
cp framework/prism/entry.ino /tmp/cliPkgPkg/framework/prism/entry.ino
cp framework/prism/main.hpp /tmp/cliPkgPkg/framework/prism/main.hpp
#--!

cp mainMake.mk /tmp/cliPkgPkg/mainMake.mk
cp setup_platforms.sh /tmp/cliPkgPkg/setup_platforms.sh
cp version.txt /tmp/cliPkgPkg/version.txt

# Move to temp directory before executing zip command
# before that save the current directory to return back to after zip command is executed
cwd=$(pwd)
cd /tmp/cliPkgPkg
zip -r cli.zip .
# Move back to the working directory before moving to temp directory
cd $(echo $cwd)
cp /tmp/cliPkgPkg/cli.zip dist/cli.zip
