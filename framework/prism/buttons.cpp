#ifndef BUTTONS_H
  #include <prism/buttons.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef Arduino_h
  #include <Arduino.h>
#endif

ButtonsClass::ButtonsClass() {
  _leftPressed = false;
  _rightPressed = false;
  _upPressed = false;
  _downPressed = false;
  _enterPressed = false;
}

void ButtonsClass::initialize() {
  pinMode(PRISM_BUTTON_PIN, INPUT);
}

void ButtonsClass::setEnter(void) {
  _enterPressed = true;
}
void ButtonsClass::setLeft(void) {
  _leftPressed = true;
}
void ButtonsClass::setRight(void) {
  _rightPressed = true;
}
void ButtonsClass::setDown(void) {
  _downPressed = true;
}
void ButtonsClass::setUp(void) {
  _upPressed = true;
}

bool ButtonsClass::leftPressed(void) {
  if (_leftPressed) {
    _leftPressed = false;
    return true;
  }
  return false;
}

bool ButtonsClass::rightPressed(void) {
  if (_rightPressed) {
    _rightPressed = false;
    return true;
  }
  return false;
}

bool ButtonsClass::upPressed(void) {
  if (_upPressed) {
    _upPressed = false;
    return true;
  }
  return false;
}

bool ButtonsClass::downPressed(void) {
  if (_downPressed) {
    _downPressed = false;
    return true;
  }
  return false;
}

bool ButtonsClass::enterPressed(void) {
  if (_enterPressed) {
    _enterPressed = false;
    return true;
  }
  return false;
}

void ButtonsClass::clear(void) {
  _leftPressed = false;
  _rightPressed = false;
  _upPressed = false;
  _downPressed = false;
  _enterPressed = false;
}
