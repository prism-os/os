#ifndef COMPOSER_H
  #include <prism/composer.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif

#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

#ifndef BLOCKING_H
  #include <prism/blocking.hpp>
#endif

#ifndef BUTTONS_H
  #include <prism/buttons.hpp>
#endif

int ComposerClass::getCurrentAppId(void) {
  return _currentAppId;
}

void ComposerClass::setNextAppId(int id) {
  _nextAppId = id;
}

ComposerClass::ComposerClass(void) {
  _currentAppId = PRISM_HOME_APP_ID;
  _nextAppId = PRISM_HOME_APP_ID;
  _leftAppId = 0;
  _rightAppId = -1;
}

void ComposerClass::draw(void) {
  if (canSwitchLeft()) {
    Display.DONTUSEmarkerLeft();
  }
  if (canSwitchRight()) {
    Display.DONTUSEmarkerRight();
  }
  AppWrapper* app = Apps.getAppInstanceFromId(_currentAppId);
  Display.graphics.topBar(app->name);
  app->devApp->draw(Display.graphics);
}

void ComposerClass::update(void) {
  if (Buttons.leftPressed() && canSwitchLeft()) {
    setNextAppId(_leftAppId);
  } else if (Buttons.rightPressed() && canSwitchRight()) {
    setNextAppId(_rightAppId);
  }
  if (_currentAppId != _nextAppId) {
    if (Apps.isExists(_nextAppId)) {
      int prevAppId = _currentAppId;
      _currentAppId = _nextAppId;
      AppWrapper* current = Apps.getAppInstanceFromId(_currentAppId);
      AppWrapper* prev = Apps.getAppInstanceFromId(prevAppId);
      current->devApp->mount();
      prev->devApp->unmount();
    } else {
      _currentAppId = 0;
      _nextAppId = 0;
    }
  } else {
    AppWrapper* current = Apps.getAppInstanceFromId(_currentAppId);
    current->devApp->update();
  }
  _updateSwitchingOptions();
}

bool ComposerClass::canSwitchLeft() {
  return _leftAppId != 0;
}

bool ComposerClass::canSwitchRight() {
  return _rightAppId != -1;
}

void ComposerClass::_updateSwitchingOptions(void) {
  int i;
  int currentIndex = Apps.getSortedIndexFromId(getCurrentAppId());
  int new__leftAppId = 0;
  int new__rightAppId = -1;
  i = 1;
  while (Apps.getAppIdFromSortedIndex(currentIndex + i) != -1) {
    int appId = Apps.getAppIdFromSortedIndex(currentIndex + i);
    int indexInUnorderedList = Apps.getInstallationIndexFromId(appId);
    if (Apps.getAppInstanceFromInstallationIndex(indexInUnorderedList)->widgetAbility && !Blocking.isBlocked(appId, PRISM_WIDGET)) {
      new__rightAppId = appId;
      break;
    }
    i++;
  }
  i = 1;
  while (currentIndex - i != 0) {
    int appId = Apps.getAppIdFromSortedIndex(currentIndex - i);
    int indexInUnorderedList = Apps.getInstallationIndexFromId(appId);
    if (Apps.getAppInstanceFromInstallationIndex(indexInUnorderedList)->widgetAbility && !Blocking.isBlocked(appId, PRISM_WIDGET)) {
      new__leftAppId = appId;
      break;
    }
    i++;
  }
  _leftAppId = new__leftAppId;
  _rightAppId = new__rightAppId;
}
