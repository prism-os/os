#pragma once
#ifndef APP_BASE_CLASS_H
  #define APP_BASE_CLASS_H

  #ifndef GRAPHICS_H
    #include <prism/graphics.hpp>
  #endif

  #ifndef NOTIFICATION_H
    #include <prism/notification.hpp>
  #endif

  class App {
    public:
    virtual void draw(Graphics graphics);
    virtual void update(void);
    virtual Notification notify(void);
    virtual void mount(void);
    virtual void unmount(void);
  };

#endif
