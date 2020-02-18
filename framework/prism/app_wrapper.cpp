#ifndef APP_WRAPPER_CLASS_H
  #include <prism/app_wrapper.hpp>
#endif

#ifndef APP_BASE_CLASS_H
  #include <prism/app.hpp>
#endif

AppWrapper::AppWrapper(
  const char* inp_name,
  bool widgetAbility,
  bool notifyAbility,
  App* app
) : notifyAbility(notifyAbility),
  widgetAbility(widgetAbility) {
    strcpy(name, inp_name);
    devApp = app;
  };
