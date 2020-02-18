#pragma once
#ifndef NOTIFICATION_STACK_H
  #define NOTIFICATION_STACK_H

  #ifndef NOTIFICATION_WITH_APP_NAME_H
    #include <prism/notification_with_app_id.hpp>
  #endif

  class NotificationStackClass {
    private:
    NotificationWithAppId _stack[10];
    int _top;

    public:
    NotificationStackClass(void);
    int length(void);
    NotificationWithAppId pop(void);
    void push(NotificationWithAppId notificationWithAppId);
    void poll(void);
  };

  extern NotificationStackClass NotificationStack;

#endif
