#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
#endif
#include <ESPAsyncTCP.h>

#include <prism/main.hpp>

void setup(void) {
  Prism::setup();
}

void loop(void) {
  Prism::loop();
}
