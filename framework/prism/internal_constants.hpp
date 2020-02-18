#pragma once
#ifndef INTERNAL_CONSTANTS_H

  #ifndef IP_AS_STRING_LENGTH
    #define IP_AS_STRING_LENGTH 15 + 1
  #endif

  #ifndef PRISM_OLED_REFRESH_RATE
    #define PRISM_OLED_REFRESH_RATE 10
  #endif

  #ifndef PRISM_HOME_APP_ID
    #define PRISM_HOME_APP_ID 1
  #endif

  #ifndef PRISM_NOTIFICATIONS_APP_ID
    #define PRISM_NOTIFICATIONS_APP_ID 2
  #endif

  // Also defined seperately in apps.hpp
  // this file is not included there,
  // as apps.hpp is kept in distributable,
  // so included this file in it will let anyone,
  // with the dist to see and edit the source of this file
  #ifndef PRISM_MAX_ALLOWED_INSTALLATIONS
    #define PRISM_MAX_ALLOWED_INSTALLATIONS 9
  #endif

  #ifndef PRISM_EEPROM_OFFSET
    #define PRISM_EEPROM_OFFSET 98
  #endif

  #ifndef PRISM_BUTTON_PIN
    #define PRISM_BUTTON_PIN 12
  #endif

  #ifndef PRISM_DEVICE_ID_LENGTH
    #define PRISM_DEVICE_ID_LENGTH 6 + 1
  #endif

  #ifndef PRISM_SERVER_ID_LENGTH
    #define PRISM_SERVER_ID_LENGTH 9 + 1
  #endif

  #ifndef PRISM_WIDGET
    #define PRISM_WIDGET 1
  #endif

  #ifndef PRISM_NOTIFICATION
    #define PRISM_NOTIFICATION 2
  #endif

#endif
