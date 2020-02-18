#pragma once
#ifndef TIMER_H
  #define TIMER_H

  class Timer {
    private:
    uint32_t _intervalDelay;
    uint32_t _initialMillis;
    bool _isArmed;

    public:
    Timer();
    bool isOver();
    void start(uint32_t delay);
  };

#endif
