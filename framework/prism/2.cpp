#include <prism/2.hpp>

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef BUTTONS_H
  #include <prism/buttons.hpp>
#endif

#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

#ifndef NOTIFICATION_WITH_APP_NAME_H
  #include <prism/notification_with_app_id.hpp>
#endif

#ifndef NOTIFICATION_STACK_H
  #include <prism/notification_stack.hpp>
#endif

NotificationsApp::NotificationsApp() {
  appname_msg = "";
  title_msg = "";
  full_desc_text = "...";
  overview = true;
  full_desc = false;
  appname = "Notifications";

  moreContent = false;
  contentOffset = 0;
}

void NotificationsApp::draw(Graphics graphics) {
  if (overview) {
    graphics.topBar(appname_msg);
    graphics.line(title_msg, 0, 0);
    if (NotificationStack.length() != 0) {
      graphics.markerDown();
    }
  } else if (!overview && !full_desc) {
    graphics.topBar(appname_msg);
    graphics.line(title_msg, 0, 0);
    graphics.markerUp();
    graphics.markerEnter();
    if (NotificationStack.length() != 0) {
      graphics.markerDown();
    }
  } else if (!overview && full_desc) {
    graphics.backBar();
    graphics.markerEnter();
    if (contentOffset > 0) {
      graphics.markerUp();
    }
    if (moreContent) {
      graphics.markerDown();
    }
    moreContent = graphics.paragraph(full_desc_text, 0, 0, 0, 0, contentOffset);
  }
}

void NotificationsApp::update(void) {
  if (overview) {
    if (Buttons.downPressed()) {
      if (NotificationStack.length() != 0) {
        to_show = NotificationStack.pop();
        overview = false;
      }
    }
  } else {
    if (!full_desc) {
      if (Buttons.downPressed()) {
        if (NotificationStack.length() != 0) {
          to_show = NotificationStack.pop();
        }
      }
      if (Buttons.upPressed()) {
        overview = true;
      }
      if (Buttons.enterPressed()) {
        full_desc = true;
      }
    } else {
      if (Buttons.enterPressed()) {
        full_desc = false;
        moreContent = false;
        contentOffset = 0;
      }
      if (Buttons.downPressed()) {
        if (moreContent) {
          contentOffset += 1;
        }
      }
      if (Buttons.upPressed()) {
        if (contentOffset > 0) {
          contentOffset -= 1;
        }
      }
    }
  }
  if (overview) {
    if (NotificationStack.length() != 0) {
      appname_msg = String(NotificationStack.length()) + " " + "notifications";
      title_msg = "Scroll down to see";
    } else {
      appname_msg = "No notifications";
      title_msg = "Enjoy your day";
    }
  } else if (!overview && !full_desc) {
    // If the notification is from Notifications app itself
    if (to_show.getAppId() == PRISM_NOTIFICATIONS_APP_ID) {
      appname_msg = to_show.getTitle();
      title_msg = to_show.getBody();
    } else {
      appname_msg = String(Apps.getAppInstanceFromId(to_show.getAppId())->name);
      title_msg = to_show.getTitle();
    }
  } else if (!overview && full_desc) {
    full_desc_text = to_show.getBody();
  }
}

Notification NotificationsApp::notify(void) {
  return Notification::empty();
}
