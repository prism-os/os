#include <prism/1.hpp>

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef DATE_TIME_H
  #include <prism/date_time.hpp>
#endif

#ifndef WiFi_h
  #include <ESP8266WiFi.h>
#endif

#ifndef API_H
  #include <prism/api.hpp>
#endif

HomeApp::HomeApp() {
  is_wifi_connected = false;
  hour = 0;
  minute = 0;
  second = 0;
  day = 0;
  month = 0;
  year = 0;
}

void HomeApp::draw(Graphics graphics) {
  uint8_t x_displacement = (graphics.getWidgetAreaWidth() - (Graphics::LARGE_FONT_WIDTH * 7 + Graphics::NORMAL_FONT_WIDTH)) / 2;
  graphics.line(
    hour < 10 ? ("0" + String(hour)) : String(hour),
    0 + x_displacement,
    0, 0, true, true
  );
  graphics.line(
    ":",
    Graphics::LARGE_FONT_WIDTH * 2 + x_displacement,
    0, 0, true, true
  );
  graphics.line(
    minute < 10 ? ("0" + String(minute)) : String(minute),
    Graphics::LARGE_FONT_WIDTH * 3 + x_displacement,
    0, 0, true, true
  );
  graphics.line(
    ":",
    Graphics::LARGE_FONT_WIDTH * 5 + x_displacement,
    0, 0, true, true
  );
  graphics.line(
    second < 10 ? ("0" + String(second)) : String(second),
    Graphics::LARGE_FONT_WIDTH * 6 + x_displacement,
    0, 0, true, false
  );
  graphics.line(
    day < 10 ? ("0" + String(day)) : String(day),
    0 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT, 0, false, false
  );
  graphics.line(
    "-",
    Graphics::NORMAL_FONT_WIDTH * 2 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT, 0, false, false
  );
  graphics.line(
    month < 10 ? ("0" + String(month)) : String(month),
    Graphics::NORMAL_FONT_WIDTH * 3 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT, 0, false, false
  );
  graphics.line(
    "-",
    Graphics::NORMAL_FONT_WIDTH * 5 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT, 0, false, false
  );
  graphics.line(
    year < 10 ? ("000" + String(year)) : String(year),
    Graphics::NORMAL_FONT_WIDTH * 6 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT, 0, false, false
  );
  #ifdef DEBUG
    graphics.line(
      "DEBUG",
      graphics.getWidgetAreaWidth() - Graphics::NORMAL_FONT_WIDTH * 5,
      Graphics::LARGE_FONT_HEIGHT, 0, false, false
    );
  #endif
  graphics.line(
    is_wifi_connected ? F("Wi-Fi connected") : F("Wi-Fi disconnected"),
    0 + x_displacement,
    Graphics::LARGE_FONT_HEIGHT + Graphics::NORMAL_FONT_HEIGHT, 0, false, false
  );
}

void HomeApp::update(void) {
  is_wifi_connected = WiFi.isConnected();
  hour = DateTime.hour();
  minute = DateTime.minute();
  second = DateTime.second();
  day = DateTime.day();
  month = DateTime.month();
  year = DateTime.year();
}
