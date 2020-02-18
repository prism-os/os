#ifndef PRISM_H
  #include <prism/main.hpp>
#endif

#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif
#ifndef BUTTONS_H
  #include <prism/buttons.hpp>
#endif
#ifndef PAIR_H
  #include <prism/pair.hpp>
#endif
#ifndef WIFI_CONNECT_H
  #include <prism/wifi_connect.hpp>
#endif
#ifndef NOTIFICATION_STACK_H
  #include <prism/notification_stack.hpp>
#endif
#ifndef API_H
  #include <prism/api.hpp>
#endif
#ifndef COMPOSER_H
  #include <prism/composer.hpp>
#endif

// The below declarations are not needed in this file,
// but need to be declared for Prism and apps to use
#ifndef APPS_H
  #include <prism/apps.hpp>
#endif
#ifndef BLOCKING_H
  #include <prism/blocking.hpp>
#endif
#ifndef DATE_TIME_H
  #include <prism/date_time.hpp>
#endif  
AppsClass Apps;
BlockingClass Blocking;
DateTimeClass DateTime;

#ifdef DEBUG
  #ifndef DEBUGGING_BRIDGE_H
    #include <prism/debugging_bridge.hpp>
  #endif
  DebuggingBridgeClass DebuggingBridge;
#endif

DisplayClass Display;
ButtonsClass Buttons;
PairClass Pair;
WiFiConnectClass WiFiConnect;
NotificationStackClass NotificationStack;
ApiClass Api;
ComposerClass Composer;

void Prism::setup() {
  Display.initialize();
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  Buttons.initialize();
  WiFiConnect.initialize();
  Pair.initialize();
}

void Prism::loop() {
  #ifdef DEBUG
    DebuggingBridge.check();
    DebuggingBridge.respond();
    Display.draw(DebuggingBridge.receivedScreenShotCommand());
  #else
    Display.draw();
  #endif
  Api.check();
  NotificationStack.poll();
  Composer.update();
  Buttons.clear();
  #ifdef DEBUG
    DebuggingBridge.reset();
  #endif
  delay(PRISM_OLED_REFRESH_RATE);
}
