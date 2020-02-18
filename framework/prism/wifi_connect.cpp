#ifndef WIFI_CONNECT_H
  #include <prism/wifi_connect.hpp>
#endif

#ifndef WiFiManager_h
  #include <WiFiManager.h>
#endif

#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif

#ifndef GRAPHICS_H
  #include <prism/graphics.hpp>
#endif

#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

void WiFiConnectClass::_configModeCallback (WiFiManager* wiFiManager) {
  Display.u8g2.firstPage();
  do {
    Display.DONTUSEdrawStr(F("Connect to hotspot:"), 0, 0);
    Display.DONTUSEdrawStr(wiFiManager->getConfigPortalSSID(), 0, Graphics::NORMAL_FONT_HEIGHT);
    Display.DONTUSEdrawStr(F("Open in browser:"), 0, Graphics::NORMAL_FONT_HEIGHT * 2);
    Display.DONTUSEdrawStr(F("192.168.4.1"), 0, Graphics::NORMAL_FONT_HEIGHT * 3);
  } while (Display.u8g2.nextPage());
}

WiFiConnectClass::WiFiConnectClass() {
  sprintf(_APName, "%d", ESP.getChipId());
  strcat(_APName, "AP");
}

void WiFiConnectClass::initialize() {
  _wifiManager.setDebugOutput(false);
  _wifiManager.setAPCallback(_configModeCallback);
  _wifiManager.autoConnect(_APName);
}
