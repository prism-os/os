#pragma once
#ifndef NOTIFICATION_WITH_APP_NAME_H
  #define NOTIFICATION_WITH_APP_NAME_H

  #ifndef String_class_h
    #include <WString.h>
  #endif

  #ifndef NOTIFICATION_H
    #include <prism/notification.hpp>
  #endif

  class NotificationWithAppId {
    private:
    int _appId;
    String _title;
    String _body;

    public:
    NotificationWithAppId(Notification notification, int appId);
    NotificationWithAppId(void);
    int getAppId(void);
    String getTitle(void);
    String getBody(void);
  };

#endif
