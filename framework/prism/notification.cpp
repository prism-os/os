#ifndef NOTIFICATION_H
  #include <prism/notification.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

Notification::Notification(String title, String body) {
  _title = title;
  _body = body;
  _blank = title == "";
}

Notification Notification::empty() {
  return Notification("", "");
}

bool Notification::isBlank() {
  return _blank;
}

String Notification::getTitle() {
  return _title;
}
String Notification::getBody() {
  return _body;
}
