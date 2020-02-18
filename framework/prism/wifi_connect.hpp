#pragma once
#ifndef WIFI_CONNECT_H
  #define WIFI_CONNECT_H

  #ifndef WiFiManager_h
    #include <WiFiManager.h>
  #endif

  class WiFiConnectClass {
    private:
    char _APName[11];
    WiFiManager _wifiManager;
    static void _configModeCallback (WiFiManager* wiFiManager);

    public:
    WiFiConnectClass(void);
    void initialize(void);
  };

  extern WiFiConnectClass WiFiConnect;

#endif
