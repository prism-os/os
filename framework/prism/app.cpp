#ifndef APP_BASE_CLASS_H
  #include <prism/app.hpp>
#endif

#ifndef GRAPHICS_H
  #include <prism/graphics.hpp>
#endif

#ifndef NOTIFICATION_H
  #include <prism/notification.hpp>
#endif

void App::draw(Graphics graphics) {}

void App::update() {}

Notification App::notify() {
  return Notification::empty();
}

void App::mount() {}

void App::unmount() {}
