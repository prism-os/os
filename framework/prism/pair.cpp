#ifndef PAIR_H
  #include <prism/pair.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef WIFIUDP_H
  #include <WiFiUdp.h>
#endif

#ifndef EEPROM_h
  #include <EEPROM.h>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif

#ifndef GRAPHICS_H
  #include <prism/graphics.hpp>
#endif

#ifndef DATE_TIME_H
  #include <prism/date_time.hpp>
#endif

#ifndef PRISM_OLED_PRE_ENCREMENT_DELAY
  #define PRISM_OLED_PRE_ENCREMENT_DELAY 2
#endif

#ifndef DEVICE_PORT
  #define DEVICE_PORT 4210
#endif

#ifndef SERVER_PORT
  #define SERVER_PORT 4210
#endif

#ifndef BROADCAST_IP
  #define BROADCAST_IP "255.255.255.255"
#endif

#include <ArduinoJson.h>

PairClass::PairClass() {
  _deviceId = String(ESP.getChipId());
  _serverIp[IP_AS_STRING_LENGTH] = 'xxx.xxx.xxx.xxx';
  _serverId[PRISM_SERVER_ID_LENGTH] = '000000000';
}

char* PairClass::_generateExistingHandshakeWaitJson() {
  char _deviceId_to_send[7];
  _deviceId.toCharArray(_deviceId_to_send, PRISM_DEVICE_ID_LENGTH);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& wait_json_obj = jsonBuffer.createObject();
  wait_json_obj["application"] = "system";
  wait_json_obj["sub_module"] = "udp_reconnection";
  JsonObject& wait_json_data_obj = wait_json_obj.createNestedObject("data");
  wait_json_data_obj["action"] = "client_reconn_request";
  wait_json_data_obj["client_id"] = _deviceId_to_send;
  char wait_json[140];
  wait_json_obj.printTo(wait_json, sizeof(wait_json));
  return wait_json;
}

char* PairClass::_generateExistingHandshakeDoneJson() {
  char _deviceId_to_send[7];
  _deviceId.toCharArray(_deviceId_to_send, PRISM_DEVICE_ID_LENGTH);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& done_json_obj = jsonBuffer.createObject();
  done_json_obj["application"] = "system";
  done_json_obj["sub_module"] = "udp_reconnection";
  JsonObject& done_json_data_obj = done_json_obj.createNestedObject("data");
  done_json_data_obj["action"] = "client_reconn_accept";
  done_json_data_obj["client_id"] = _deviceId_to_send;
  char done_json[140];
  done_json_obj.printTo(done_json, sizeof(done_json));
  return done_json;
}

char* PairClass::_generateNewHandshakeSendJson() {
  char _deviceId_to_send[7];
  _deviceId.toCharArray(_deviceId_to_send, PRISM_DEVICE_ID_LENGTH);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& send_json_obj = jsonBuffer.createObject();
  send_json_obj["application"] = "system";
  send_json_obj["sub_module"] = "initial_udp_connection";
  JsonObject& send_json_data_obj = send_json_obj.createNestedObject("data");
  send_json_data_obj["action"] = "client_request";
  send_json_data_obj["client_id"] = _deviceId_to_send;
  char send_json[140];
  send_json_obj.printTo(send_json, sizeof(send_json));
  return send_json;
}

char* PairClass::_generateNewHandshakeDoneJson() {
  char _deviceId_to_send[7];
  _deviceId.toCharArray(_deviceId_to_send, PRISM_DEVICE_ID_LENGTH);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& done_json_obj = jsonBuffer.createObject();
  done_json_obj["application"] = "system";
  done_json_obj["sub_module"] = "initial_udp_connection";
  JsonObject& done_json_data_obj = done_json_obj.createNestedObject("data");
  done_json_data_obj["action"] = "client_accept";
  done_json_data_obj["client_id"] = _deviceId_to_send;
  char done_json[140];
  done_json_obj.printTo(done_json, sizeof(done_json));
  return done_json;
}

uint32_t PairClass::_parseForExistingHandshake(char* incomingPacket) {
  IPAddress ret_ip_obj;
  String ip_string_obj;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& resp_json = jsonBuffer.parseObject(incomingPacket);
  if (resp_json.success()) {
    const char* resp_json_application = resp_json["application"];
    const char* resp_json_sub_module = resp_json["sub_module"];
    const char* resp_json_data_action = resp_json["data"]["action"];
    const char* resp_json_data_passkey = resp_json["data"]["passkey"];
    const char* resp_json_data_server_id = resp_json["data"]["server_id"];
    uint32_t resp_json_data_timestamp = resp_json["data"]["timestamp"];
    if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("udp_reconnection")) && (String(resp_json_data_action) == String("server_reconn_request")) && (String(resp_json_data_server_id) == String(_serverId))) {
      ret_ip_obj = _wiFiUDP.remoteIP();
      ip_string_obj = String(ret_ip_obj[0]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[1]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[2]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[3]);
      ip_string_obj.toCharArray(_serverIp, IP_AS_STRING_LENGTH);
      return resp_json_data_timestamp;
    }
  }
  return 0;
}

// Uses EEPROM to save server id
uint32_t PairClass::_parseForNewHandshake(char* incomingPacket, String rand_pkey) {
  IPAddress ret_ip_obj;
  String ip_string_obj;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& resp_json = jsonBuffer.parseObject(incomingPacket);
  if (resp_json.success()) {
    const char* resp_json_application = resp_json["application"];
    const char* resp_json_sub_module = resp_json["sub_module"];
    const char* resp_json_data_action = resp_json["data"]["action"];
    const char* resp_json_data_passkey = resp_json["data"]["passkey"];
    uint32_t resp_json_data_timestamp = resp_json["data"]["timestamp"];
    if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("initial_udp_connection")) && (String(resp_json_data_action) == String("server_request")) && (String(resp_json_data_passkey) == rand_pkey)) {
      ret_ip_obj = _wiFiUDP.remoteIP();
      ip_string_obj = String(ret_ip_obj[0]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[1]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[2]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[3]);
      ip_string_obj.toCharArray(_serverIp, IP_AS_STRING_LENGTH);
      strcpy(_serverId, resp_json["data"]["server_id"]);
      char ok[2 + 1] = "OK";
      EEPROM.begin(512);
      EEPROM.put(0, ok);
      EEPROM.put(0 + sizeof(ok), _serverId);
      EEPROM.commit();
      EEPROM.end();
      return resp_json_data_timestamp;
    }
  }

  return 0;
}

// Existing handshake flow
// Uses display, _wiFiUDP to call methods on
// Uses PRISM_BUTTON_PIN
// Uses EEPROM to reset when PRISM_BUTTON_PIN pulled high
void PairClass::_existingHandshake() {
  int packetSize;
  int packetLen;
  char incomingPacket[255];
  uint32_t on_udp_config_done_timestamp = 0;
  Display.u8g2.firstPage();
  do {
    Display.DONTUSEdrawStr(F("Reconnecting..."), 0, 0);
    Display.DONTUSEdrawStr(F("To setup a new pair:"), 0, Graphics::NORMAL_FONT_HEIGHT);
    Display.DONTUSEdrawStr(F("Press and hold button"), 0, Graphics::NORMAL_FONT_HEIGHT * 2);
  } while (Display.u8g2.nextPage());
  while (on_udp_config_done_timestamp == 0) {
    if (digitalRead(PRISM_BUTTON_PIN) == HIGH ) {
      delay(PRISM_OLED_PRE_ENCREMENT_DELAY);
      if (digitalRead(PRISM_BUTTON_PIN) == HIGH ) {
        EEPROM.begin(512);
        for (int i = 0; i < 512; i++) {
          EEPROM.write(i, 0);
        }
        EEPROM.end();
        ESP.restart();
      }
    }
    packetSize = _wiFiUDP.parsePacket();
    if (packetSize) {
      packetLen = _wiFiUDP.read(incomingPacket, 255);
      if (packetLen > 0) {
        on_udp_config_done_timestamp = _parseForExistingHandshake(incomingPacket);
      }
    }
    if (on_udp_config_done_timestamp == 0) {
      char wait_json[140];
      strcpy(wait_json, _generateExistingHandshakeWaitJson());
      _wiFiUDP.beginPacket(BROADCAST_IP, SERVER_PORT);
      _wiFiUDP.write(wait_json);
      _wiFiUDP.endPacket();
      delay(500);
    } else {
      char done_json[140];
      strcpy(done_json, _generateExistingHandshakeDoneJson());
      _wiFiUDP.beginPacket(_serverIp, SERVER_PORT);
      _wiFiUDP.write(done_json);
      _wiFiUDP.endPacket();
      DateTime.initialize(on_udp_config_done_timestamp, millis());
    }
  }
  // display.clearDisplay();
}

// New handshake flow
// Uses display and _wiFiUDP to call methods on
void PairClass::_newHandshake() {
  int packetSize;
  int packetLen;
  char incomingPacket[255];
  uint32_t on_udp_config_done_timestamp = 0;
  String rand_pkey = String(random(100000, 999999));
  Display.u8g2.firstPage();
  do {
    Display.DONTUSEdrawStr(F("Enter code:"), 0, 0);
    Display.DONTUSEdrawStr(rand_pkey, 0, Graphics::NORMAL_FONT_HEIGHT);
    Display.DONTUSEdrawStr(F("on Prism app"), 0, Graphics::NORMAL_FONT_HEIGHT * 2);
    Display.DONTUSEdrawStr(F("in your Android or"), 0, Graphics::NORMAL_FONT_HEIGHT * 3);
    Display.DONTUSEdrawStr(F("iOS device"), 0, Graphics::NORMAL_FONT_HEIGHT * 4);
  } while (Display.u8g2.nextPage());
  while (on_udp_config_done_timestamp == 0) {
    packetSize = _wiFiUDP.parsePacket();
    if (packetSize) {
      packetLen = _wiFiUDP.read(incomingPacket, 255);
      if (packetLen > 0) {
        on_udp_config_done_timestamp = _parseForNewHandshake(incomingPacket, rand_pkey);
      }
    }
    if (on_udp_config_done_timestamp == 0) {
      char send_json[140];
      strcpy(send_json, _generateNewHandshakeSendJson());
      _wiFiUDP.beginPacket(BROADCAST_IP, SERVER_PORT);
      _wiFiUDP.write(send_json);
      _wiFiUDP.endPacket();
      delay(500);
    } else {
      char done_json[140];
      strcpy(done_json, _generateNewHandshakeDoneJson());
      _wiFiUDP.beginPacket(_serverIp, SERVER_PORT);
      _wiFiUDP.write(done_json);
      _wiFiUDP.endPacket();
      DateTime.initialize(on_udp_config_done_timestamp, millis());
    }
  }
  // display.clearDisplay();
}

// Check if server id is there in EEPROM, call new or existing flow accordingly
// Read EEPROM
void PairClass::initialize() {
  _wiFiUDP.begin(4210);
  EEPROM.begin(512);
  char ok[2 + 1];
  char gotten_key[PRISM_SERVER_ID_LENGTH];
  EEPROM.get(0, ok);
  EEPROM.get(0 + sizeof(ok), gotten_key);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    _newHandshake();
  } else {
    strcpy(_serverId, gotten_key);
    _existingHandshake();
  }
}

WiFiUDP PairClass::getUDP() {
  return _wiFiUDP;
}

String PairClass::getClientId() {
  return _deviceId;
}

unsigned int PairClass::getLocalPort() {
  return DEVICE_PORT;
}

char* PairClass::getServerIp() {
  return _serverIp;
}

char* PairClass::setServerIp(char* ip) {
  for (int i = 0; i < IP_AS_STRING_LENGTH; i++) {
    _serverIp[i] = ip[i];
  }
}

char* PairClass::getServerId() {
  return _serverId;
}

unsigned int PairClass::getServerPort() {
  return SERVER_PORT;
}
