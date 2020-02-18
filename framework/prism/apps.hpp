#pragma once
#ifndef APPS_H
  #define APPS_H

  #ifndef PRISM_MAX_ALLOWED_INSTALLATIONS
    #define PRISM_MAX_ALLOWED_INSTALLATIONS 9
  #endif

  #ifndef APP_WRAPPER_CLASS_H
    #include <prism/app_wrapper.hpp>
  #endif

  class AppsClass {
    private:
    int _unorderedList[PRISM_MAX_ALLOWED_INSTALLATIONS];
    void _insertApp(int appId, AppWrapper* appInstance);
    void _insertSystemAppCaller(void);
    void _insertAppCaller(void);
    int _mergeNewAndOld(int oldArray[PRISM_MAX_ALLOWED_INSTALLATIONS], int oldArrayCount, int newArray[PRISM_MAX_ALLOWED_INSTALLATIONS], int newArrayCount, int mergedArray[PRISM_MAX_ALLOWED_INSTALLATIONS]);
    AppWrapper* _appInstances[PRISM_MAX_ALLOWED_INSTALLATIONS];

    public:
    // Need to be public for api to set the orderedList on reordering
    int count;
    int orderedList[PRISM_MAX_ALLOWED_INSTALLATIONS];

    AppsClass(void);
    int getCount(void);
    bool isExists(int appId);
    int getAppIdFromInstallationIndex(int index);
    int getAppIdFromSortedIndex(int index);
    int getInstallationIndexFromId(int appId);
    int getSortedIndexFromId(int appId);
    AppWrapper* getAppInstanceFromInstallationIndex(int index);
    AppWrapper* getAppInstanceFromId(int appId);
    AppWrapper* getAppInstanceFromSortedIndex(int index);
  };

  extern AppsClass Apps;

#endif
