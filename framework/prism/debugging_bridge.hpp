#pragma once
#ifndef DEBUGGING_BRIDGE_H
  #define DEBUGGING_BRIDGE_H

  class DebuggingBridgeClass {
    private:
    char _serialInputChar;

    public:
    DebuggingBridgeClass(void);
    void check(void);
    void respond(void);
    void reset(void);
    bool receivedScreenShotCommand(void);
    char getReceivedCommand(void);
  };

  extern DebuggingBridgeClass DebuggingBridge;

#endif
