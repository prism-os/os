#ifndef NOTIFICATION_STACK_H
  #include <prism/notification_stack.hpp>
#endif

#ifndef NOTIFICATION_WITH_APP_NAME_H
  #include <prism/notification_with_app_id.hpp>
#endif

#ifndef NOTIFICATION_H
  #include <prism/notification.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

#ifndef BLOCKING_H
  #include <prism/blocking.hpp>
#endif

#ifndef STACK_DEPTH
  #define STACK_DEPTH 10
#endif

NotificationStackClass::NotificationStackClass() {
  _top = -1;
};

int NotificationStackClass::length() {
  return _top + 1;
}

NotificationWithAppId NotificationStackClass::pop(void) {
  NotificationWithAppId notification = _stack[_top];
  _top--;
  return notification;
}

void NotificationStackClass::push(NotificationWithAppId notificationWithAppId) {
  if (_top >= STACK_DEPTH - 1) {
    _top = -1;
  }
  _top++;
  _stack[_top] = notificationWithAppId;
}

void NotificationStackClass::poll() {
  for (int i = 1; i <= Apps.getCount(); i++) {
    int id = Apps.getAppIdFromInstallationIndex(i);
    AppWrapper* app = Apps.getAppInstanceFromInstallationIndex(i);
    if (app->notifyAbility && !Blocking.isBlocked(id, PRISM_NOTIFICATION)) {
      Notification polledNotification = app->devApp->notify();
      if (!polledNotification.isBlank()) {
        push(NotificationWithAppId(polledNotification, id));
      }
    }
  }
}
