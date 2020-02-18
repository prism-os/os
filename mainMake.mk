DEBUG ?= 0

BOARD = nodemcuv2
ESP_ROOT = platforms/esp8266
UPLOAD_PORT = /dev/ttyUSB0
BUILD_THREADS = 1
ifeq ($(DEBUG), 0)
  CPP_EXTRA = -I ./framework
else
  CPP_EXTRA = -I ./framework -D DEBUG
endif
include platforms/esp8266/make.mk

SRC_DIR := $(PACKAGE_SRC_DIR_PATH)
OBJ_DIR := $(PACKAGE_SRC_DIR_PATH)
SRC_CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_HPP_FILES := $(wildcard $(SRC_DIR)/*.hpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP_FILES))

SRC_MANIFEST_FILE_PATH := $(SRC_DIR)/manifest.json
PACKAGE_OUT_FILE_PATH := $(PACKAGE_OUT_DIR_PATH)/main.apkg

ifeq ($(DEBUG), 0)
  SRC_ARCHIVE_FILE_PATH := $(SRC_DIR)/main.a
else
  SRC_ARCHIVE_FILE_PATH := $(SRC_DIR)/main.debug.a
endif

-include $(OBJ_FILES:.o=.d)

.PHONY: compile
compile: $(OBJ_FILES)
	echo "Done building objects..."
	echo "Building archive..."
	$(LIB_COM) cru $(SRC_ARCHIVE_FILE_PATH) $^
	rm -f $(PACKAGE_OUT_FILE_PATH)
	zip -j $(PACKAGE_OUT_FILE_PATH) $(SRC_HPP_FILES) $(SRC_ARCHIVE_FILE_PATH) $(SRC_MANIFEST_FILE_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP_COM) $(CPP_EXTRA) -x c++ -include $(CORE_DIR)/Arduino.h $< -o $@
