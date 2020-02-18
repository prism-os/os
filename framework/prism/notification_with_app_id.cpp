#ifndef NOTIFICATION_WITH_APP_NAME_H
  #include <prism/notification_with_app_id.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef NOTIFICATION_H
  #include <prism/notification.hpp>
#endif

NotificationWithAppId::NotificationWithAppId(Notification notification, int appId) {
  _appId = appId;
  _title = notification.getTitle();
  _body = notification.getBody();
}

NotificationWithAppId::NotificationWithAppId() {
  _appId = 0;
  _title = "";
  _body = "";
}

String NotificationWithAppId::getTitle() {
  return _title;
}
String NotificationWithAppId::getBody() {
  return _body;
}

int NotificationWithAppId::getAppId() {
  return _appId;
}
