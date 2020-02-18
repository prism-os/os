#!/bin/bash
echo "Checking if existing cores exist and removing it..."
[ -d ./platforms ] && rm -rf ./platforms
mkdir platforms
echo "Creating platforms directory..."
cd platforms
echo "Cloning esp8266 core and setting up..."
git clone https://github.com/prism-os/Arduino.git esp8266
cd esp8266
git checkout tags/2.4.2
git submodule update --init
cd tools
python get.py
cd ../..
echo "Cloning esp8266 makefile..."
git clone https://github.com/prism-os/makeEspArduino.git
mv makeEspArduino/makeEspArduino.mk esp8266/make.mk
mv makeEspArduino/LICENSE esp8266/LICENSE-makeEspArduino
rm -rf makeEspArduino
echo "Beginning to clone dependencies..."
cd esp8266/libraries
echo "Cloning dependency (1/4) WiFiManager..."
git clone https://github.com/prism-os/WiFiManager.git
# cd WiFiManager
# git checkout tags/0.14
# cd ..
echo "Cloning dependency (2/4) U8g2..."
git clone https://github.com/prism-os/U8g2_Arduino.git U8g2
cd U8g2
git checkout tags/2.25.10
cd ..
echo "Cloning dependency (3/4) ESPAsyncTCP..."
git clone https://github.com/prism-os/ESPAsyncTCP.git
echo "Cloning dependency (4/4) ArduinoJson..."
git clone https://github.com/prism-os/ArduinoJson.git
cd ArduinoJson
git checkout tags/v5.7.2
rm -rf third-party
rm -rf test
cd ..
cd ../../../..
echo "Completed esp8266 core setup successfully..."
echo "Completed all core setups successfully..."
