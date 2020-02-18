#include "main.hpp"

sidevesh__notifications_demo::sidevesh__notifications_demo() {
  counter = 2;
}

void sidevesh__notifications_demo::draw(Graphics graphics) {
  graphics.line("Notifications Enabled", 0, 0);
}

Notification sidevesh__notifications_demo::notify(void) {
  if(counter == 2) {
    Notification notification("test 1", "test body");
    counter = 1;
    return notification;
  } else if(counter == 1) {
    Notification notification("test 2", "test body");
    counter = 0;
    return notification;
  }

  return Notification::empty();
}
