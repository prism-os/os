#pragma once
#ifndef DATE_TIME_H
  #define DATE_TIME_H

  #ifndef Arduino_h
    #include <Arduino.h>
  #endif

  class DateTimeClass {
    private:
    uint32_t _secondsSinceEpochAtSync;
    uint32_t _millisAtSync;
    uint8_t _monthDays[12];
    uint32_t _secondsSinceEpoch(void);
    uint32_t _minutesSinceEpoch(void);
    uint32_t _hoursSinceEpoch(void);
    uint32_t _daysSinceEpoch(void);
    uint32_t _generateDate(bool yearOnly, bool monthOnly, bool dayOnly);

    public:
    DateTimeClass();
    void initialize(uint32_t secondsSinceEpochAtSync, uint32_t millisAtSync);
    int second();
    int minute();
    int hour();
    int weekday();
    int day();
    int month();
    int year();
  };

  extern DateTimeClass DateTime;

#endif
