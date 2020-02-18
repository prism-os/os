#ifndef Arduino_h
  #include <Arduino.h>
#endif

#ifndef TIMING_H
  #include <prism/timer.hpp>
#endif

Timer::Timer() : _intervalDelay(0), _initialMillis(0), _isArmed(false) {}

bool Timer::isOver() {
  if (_isArmed && millis() - _initialMillis > _intervalDelay) {
    _isArmed = false;
    return true;
  }

  return false;
}

void Timer::start(uint32_t period) {
  _isArmed = true;
  _initialMillis = millis();
  _intervalDelay = period;
}
