#ifndef DEBUGGING_BRIDGE_H
  #include <prism/debugging_bridge.hpp>
#endif

#ifndef HardwareSerial_h
  #include <HardwareSerial.h>
#endif

#ifndef ESP_H
  #include <Esp.h>
#endif

DebuggingBridgeClass::DebuggingBridgeClass() {
  _serialInputChar = '0';
}

void DebuggingBridgeClass::reset() {
  _serialInputChar = '0';
}

void DebuggingBridgeClass::check() {
  if (Serial.available()) {
    _serialInputChar = (char)Serial.read();
  }
}

void DebuggingBridgeClass::respond() {
  if (_serialInputChar == 'm') {
    Serial.print(F("m:{"));
    Serial.print(ESP.getFreeHeap());
    Serial.println(F("}"));
  } else if (_serialInputChar == 'r') {
    Serial.print(F("r:{"));
    Serial.print(ESP.getResetReason());
    Serial.println(F("}"));
  }
}

bool DebuggingBridgeClass::receivedScreenShotCommand() {
  return _serialInputChar == 's';
}

char DebuggingBridgeClass::getReceivedCommand() {
  return _serialInputChar;
}
