#pragma once
#ifndef APP_WRAPPER_CLASS_H
  #define APP_WRAPPER_CLASS_H

  #ifndef APP_BASE_CLASS_H
    #include <prism/app.hpp>
  #endif

  class AppWrapper {
    public:
    AppWrapper(const char* name, bool widgetAbility, bool notifyAbility, App* app);
    const bool notifyAbility;
    const bool widgetAbility;
    char name[14];
    App* devApp;
  };

#endif
