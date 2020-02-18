#ifndef APP_BASE_CLASS_H
  #include <prism/app.hpp>
#endif

#ifndef NOTIFICATION_WITH_APP_NAME_H
  #include <prism/notification_with_app_id.hpp>
#endif

class NotificationsApp : public App {
  private:
  String appname_msg;
  String title_msg;
  String full_desc_text;
  bool overview;
  bool full_desc;
  NotificationWithAppId to_show;
  char* appname;

  bool moreContent;
  int contentOffset;

  public:
  NotificationsApp();
  void draw(Graphics graphics);
  void update(void);
  Notification notify(void);
};
