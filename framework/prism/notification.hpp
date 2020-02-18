#pragma once
#ifndef NOTIFICATION_H
  #define NOTIFICATION_H

  #ifndef String_class_h
    #include <WString.h>
  #endif

  class Notification {
    private:
    bool _blank;
    String _title;
    String _body;

    public:
    Notification(String title, String body);
    static Notification empty(void);
    bool isBlank(void);
    String getTitle(void);
    String getBody(void);
  };

#endif
