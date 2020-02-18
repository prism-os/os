#pragma once
#ifndef PAIR_H
  #define PAIR_H

  #ifndef String_class_h
    #include <WString.h>
  #endif

  #ifndef WIFIUDP_H
    #include <WiFiUdp.h>
  #endif

  #ifndef INTERNAL_CONSTANTS_H
    #include <prism/internal_constants.hpp>
  #endif

  class PairClass {
    public:
    PairClass(void);

    private:
    WiFiUDP _wiFiUDP;
    String _deviceId;
    char _serverIp[IP_AS_STRING_LENGTH];
    char _serverId[PRISM_SERVER_ID_LENGTH];
    char* _generateExistingHandshakeWaitJson(void);
    char* _generateExistingHandshakeDoneJson(void);
    char* _generateNewHandshakeSendJson(void);
    char* _generateNewHandshakeDoneJson(void);
    uint32_t _parseForExistingHandshake(char* incomingPacket);
    // Uses EEPROM to save server id
    uint32_t _parseForNewHandshake(char* incomingPacket, String rand_pkey);
    // Existing handshake flow
    // Uses display, _wiFiUDP to call methods on
    // Uses PRISM_BUTTON_PIN
    // Uses EEPROM to reset when PRISM_BUTTON_PIN pulled high
    void _existingHandshake(void);
    // New handshake flow
    // Uses display and _wiFiUDP to call methods on
    void _newHandshake(void);

    public:
    // Check if server id is there in EEPROM, call new or existing flow accordingly
    // Read EEPROM
    void initialize(void);
    WiFiUDP getUDP(void);
    String getClientId(void);
    unsigned int getLocalPort(void);
    char* getServerIp(void);
    char* setServerIp(char* ip);
    char* getServerId(void);
    unsigned int getServerPort(void);
  };

  extern PairClass Pair;

#endif
