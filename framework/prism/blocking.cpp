#ifndef BLOCKING_H
  #include <prism/blocking.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef EEPROM_h
  #include <EEPROM.h>
#endif

#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

BlockingClass::BlockingClass() {
  for (int i = 0; i < PRISM_MAX_ALLOWED_INSTALLATIONS; i++) {
    _widgetBlockedAppIds[i] = -1;
    _notificationBlockedAppIds[i] = -1;
  }
  _widgetBlockedAppIdsTop = -1;
  _notificationBlockedAppIdsTop = -1;

  EEPROM.begin(512);
  char ok[2 + 1];
  int gottenArray[PRISM_MAX_ALLOWED_INSTALLATIONS];
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), gottenArray);
  EEPROM.end();

  if (String(ok) == String("OK")) {
    int i = -1;
    for (int j = 0; j < PRISM_MAX_ALLOWED_INSTALLATIONS; j++) {
      if (gottenArray[j] != -1) {
        i++;
      } else {
        break;
      }
    }
    for (int j = 0; j <= i; j++) {
      _notificationBlockedAppIds[j] = gottenArray[j];
    }
    _notificationBlockedAppIdsTop = i;
    bool isFound = false;
    for (int j = 0; j <= _notificationBlockedAppIdsTop; j++) {
      isFound = false;
      for (int k = 0; k <= Apps.getCount(); k++) {
        if (_notificationBlockedAppIds[j] == Apps.getAppIdFromSortedIndex(k)) {
          isFound = true;
        }
      }
      if (!isFound) {
        unblockApp(_notificationBlockedAppIds[j], PRISM_NOTIFICATION);
      }
    }
  }

  EEPROM.begin(512);
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), gottenArray);
  EEPROM.end();

  if (String(ok) == String("OK")) {
    int i = -1;
    for (int j = 0; j < PRISM_MAX_ALLOWED_INSTALLATIONS; j++) {
      if (gottenArray[j] != -1) {
        i++;
      } else {
        break;
      }
    }
    for (int j = 0; j <= i; j++) {
      _widgetBlockedAppIds[j] = gottenArray[j];
    }
    _widgetBlockedAppIdsTop = i;
    bool isFound = false;
    for (int j = 0; j <= _widgetBlockedAppIdsTop; j++) {
      isFound = false;
      for (int k = 0; k <= Apps.getCount(); k++) {
        if (_widgetBlockedAppIds[j] == Apps.getAppIdFromSortedIndex(k)) {
          isFound = true;
        }
      }
      if (!isFound) {
        unblockApp(_widgetBlockedAppIds[j], PRISM_WIDGET);
      }
    }
  }

}

int* BlockingClass::getWidgetBlockedAppIds() {
  return _widgetBlockedAppIds;
}

int* BlockingClass::getNotificationBlockedAppIds() {
  return _notificationBlockedAppIds;
}

bool BlockingClass::isBlocked(int appId, int category) {
  if (category == PRISM_WIDGET) {
    for (int i = 0; i <= _widgetBlockedAppIdsTop; i++) {
      if (_widgetBlockedAppIds[i] == appId) {
        return true;
      }
    }

    return false;
  } else if (category == PRISM_NOTIFICATION) {
    for (int i = 0; i <= _notificationBlockedAppIdsTop; i++) {
      if (_notificationBlockedAppIds[i] == appId) {
        return true;
      }
    }

    return false;
  }
}

void BlockingClass::blockApp(int appId, int category) {
  if (category == PRISM_WIDGET) {
    _widgetBlockedAppIdsTop++;
    _widgetBlockedAppIds[_widgetBlockedAppIdsTop] = appId;
  } else if (category == PRISM_NOTIFICATION) {
    _notificationBlockedAppIdsTop++;
    _notificationBlockedAppIds[_notificationBlockedAppIdsTop] = appId;
  }
}

void BlockingClass::unblockApp(int appId, int category) {
  int itemPos = -1;
  if (category == PRISM_WIDGET) {
    for (int i = 0; i <= _widgetBlockedAppIdsTop; i++) {
      if (_widgetBlockedAppIds[i] == appId) {
        itemPos = i;
        break;
      }
    }
    if (itemPos != -1) {
      for (int i = itemPos; i <= _widgetBlockedAppIdsTop; i++) {
        if (i < PRISM_MAX_ALLOWED_INSTALLATIONS - 1) {
          _widgetBlockedAppIds[i] = _widgetBlockedAppIds[i + 1];
        } else {
          _widgetBlockedAppIds[i] = -1;
        }
      }
    }
  } else if (category == PRISM_NOTIFICATION) {
    for (int i = 0; i <= _notificationBlockedAppIdsTop; i++) {
      if (_notificationBlockedAppIds[i] == appId) {
        itemPos = i;
        break;
      }
    }
    if (itemPos != -1) {
      for (int i = itemPos; i <= _notificationBlockedAppIdsTop; i++) {
        if (i < PRISM_MAX_ALLOWED_INSTALLATIONS - 1) {
          _notificationBlockedAppIds[i] = _notificationBlockedAppIds[i + 1];
        } else {
          _notificationBlockedAppIds[i] = -1;
        }
      }
    }
  }
}
