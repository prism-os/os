#pragma once
#ifndef BLOCKING_H
  #define BLOCKING_H

  #ifndef INTERNAL_CONSTANTS_H
    #include <prism/internal_constants.hpp>
  #endif

  class BlockingClass {
    private:
    int _widgetBlockedAppIds[PRISM_MAX_ALLOWED_INSTALLATIONS];
    int _notificationBlockedAppIds[PRISM_MAX_ALLOWED_INSTALLATIONS];
    int _widgetBlockedAppIdsTop;
    int _notificationBlockedAppIdsTop;

    public:
    BlockingClass(void);
    bool isBlocked(int appId, int category);
    void blockApp(int appId, int category);
    void unblockApp(int appId, int category);
    int* getWidgetBlockedAppIds();
    int* getNotificationBlockedAppIds();
  };

  extern BlockingClass Blocking;

#endif
