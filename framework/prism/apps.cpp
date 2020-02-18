#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef APP_WRAPPER_CLASS_H
  #include <prism/app_wrapper.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef EEPROM_h
  #include <EEPROM.h>
#endif

#include "./0.hpp"
#include "./1.hpp"
#include "./2.hpp"
NoneApp NoneApp_object;
HomeApp HomeApp_object;
NotificationsApp NotificationsApp_object;
AppWrapper NoneApp_object_wrapper("None", true, false, &NoneApp_object);
AppWrapper HomeApp_object_wrapper("Home", true, false, &HomeApp_object);
AppWrapper NotificationsApp_object_wrapper("Notifications", true, true, &NotificationsApp_object);

//returns size of ret_arr
int AppsClass::_mergeNewAndOld(int oldArray[PRISM_MAX_ALLOWED_INSTALLATIONS], int oldArrayCount, int newArray[PRISM_MAX_ALLOWED_INSTALLATIONS], int newArrayCount, int mergedArray[PRISM_MAX_ALLOWED_INSTALLATIONS]) {
  int mergedArrayCount = -1;
  for (int i = 0; i <= oldArrayCount; i++) {
    for (int j = 0; j <= newArrayCount; j++) {
      if (newArray[j] == oldArray[i]) {
        mergedArrayCount++;
        mergedArray[mergedArrayCount] = newArray[j];
        break;
      }
    }
  }
  bool found = false;
  for (int i = 0; i <= newArrayCount; i++) {
    found = false;
    for (int j = 0; j <= oldArrayCount; j++) {
      if (newArray[i] == oldArray[j]) {
        found = true;
      }
    }
    if (!found) {
      mergedArrayCount++;
      mergedArray[mergedArrayCount] = newArray[i];
    }
  }
  return mergedArrayCount;
}

void AppsClass::_insertSystemAppCaller(void) {
  _insertApp(0, &NoneApp_object_wrapper);
  _insertApp(1, &HomeApp_object_wrapper);
  _insertApp(2, &NotificationsApp_object_wrapper);
}

void AppsClass::_insertApp(int appId, AppWrapper* appInstance) {
  count++;
  orderedList[count] = appId;
  _unorderedList[count] = appId;
  _appInstances[count] = appInstance;
}

AppsClass::AppsClass() {
  for (int i = 0; i < PRISM_MAX_ALLOWED_INSTALLATIONS; i++) {
    _unorderedList[i] = -1;
    orderedList[i] = -1;
  }
  count = -1;
  _insertSystemAppCaller();
  _insertAppCaller();
  // restore apps list from EEPROM
  EEPROM.begin(512);
  char ok[2 + 1];
  int gotten_array[PRISM_MAX_ALLOWED_INSTALLATIONS];
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)), ok);
  EEPROM.get(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok), gotten_array);
  EEPROM.end();
  if (String(ok) == String("OK")) {
    int i = 0;
    for (int j = 0; j < PRISM_MAX_ALLOWED_INSTALLATIONS; j++) {
      if (gotten_array[j] != -1) {
        i++;
      } else {
        break;
      }
    }
    i--;
    int mergedArray[PRISM_MAX_ALLOWED_INSTALLATIONS];
    for (int j = 0; j < PRISM_MAX_ALLOWED_INSTALLATIONS; j++) {
      mergedArray[j] = -1;
    }
    int mergedArrayCount = _mergeNewAndOld(gotten_array, i, orderedList, count, mergedArray);
    for (int i = 1; i <= mergedArrayCount; i++) {
      orderedList[i] = mergedArray[i];
    }
    count = mergedArrayCount;
  }
}

int AppsClass::getCount() {
  return count;
}

bool AppsClass::isExists(int appId) {
  for (int i = 1; i <= count; i++) {
    if (_unorderedList[i] == appId) {
      return true;
    }
  }

  return false;
}

int AppsClass::getAppIdFromInstallationIndex(int index) {
  return _unorderedList[index];
}

int AppsClass::getAppIdFromSortedIndex(int index) {
  return orderedList[index];
}

int AppsClass::getInstallationIndexFromId(int appId) {
  for (int i = 0; i <= count; i++) {
    if (_unorderedList[i] == appId) {
      return i;
    }
 }
}

int AppsClass::getSortedIndexFromId(int appId) {
  for (int i = 0; i <= count; i++) {
    if (orderedList[i] == appId) {
      return i;
    }
 }
}

AppWrapper* AppsClass::getAppInstanceFromInstallationIndex(int index) {
  return _appInstances[index];
}

AppWrapper* AppsClass::getAppInstanceFromId(int appId) {
  for (int i = 0; i <= count; i++) {
    if (_unorderedList[i] == appId) {
      return getAppInstanceFromInstallationIndex(i);
    }
  }
}

AppWrapper* AppsClass::getAppInstanceFromSortedIndex(int index) {
  return getAppInstanceFromId(getAppIdFromSortedIndex(index));
}
