#ifndef DATE_TIME_H
  #include <prism/date_time.hpp>
#endif

#ifndef Arduino_h
  #include <Arduino.h>
#endif

#define LEAP_YEAR(Y) (((1970 + (Y)) > 0) && !((1970 + (Y)) % 4) && ( ((1970 + (Y)) % 100) || !((1970 + (Y)) % 400)))
#define MILLISECONDS_IN_SECONDS 1000
#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24
#define DAYS_IN_WEEK 7
#define OFFSET_SINCE_EPOCH_IS_THURSDAY 4


DateTimeClass::DateTimeClass() : _monthDays({
  31, 28, 31, 30,
  31, 30, 31, 31,
  30, 31, 30, 31
}), _secondsSinceEpochAtSync(0), _millisAtSync(0) {}

void DateTimeClass::initialize(uint32_t secondsSinceEpochAtSync, uint32_t millisAtSync) {
  _secondsSinceEpochAtSync = secondsSinceEpochAtSync;
  _millisAtSync = millisAtSync;
}

uint32_t DateTimeClass::_secondsSinceEpoch() {
  return _secondsSinceEpochAtSync + (millis() - _millisAtSync) / MILLISECONDS_IN_SECONDS;
}

uint32_t DateTimeClass::_minutesSinceEpoch() {
  return _secondsSinceEpoch() / SECONDS_IN_MINUTE;
}

uint32_t DateTimeClass::_hoursSinceEpoch() {
  return _minutesSinceEpoch() / MINUTES_IN_HOUR;
}

uint32_t DateTimeClass::_daysSinceEpoch() {
  return _hoursSinceEpoch() / HOURS_IN_DAY;
}

int DateTimeClass::second() {
  return _secondsSinceEpoch() % SECONDS_IN_MINUTE;
}

int DateTimeClass::minute() {
  return _minutesSinceEpoch() % MINUTES_IN_HOUR;
}

int DateTimeClass::hour() {
  return _hoursSinceEpoch() % HOURS_IN_DAY;
}

int DateTimeClass::weekday() {
  return (_daysSinceEpoch() + OFFSET_SINCE_EPOCH_IS_THURSDAY) % DAYS_IN_WEEK; // Sunday is 0 and so on
}

uint32_t DateTimeClass::_generateDate(bool yearOnly, bool monthOnly, bool dayOnly) {
  uint32_t year = 0;  
  uint32_t days = 0;
  uint32_t calculated__daysSinceEpoch = _daysSinceEpoch();

  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= calculated__daysSinceEpoch) {
    year++;
  }

  if (yearOnly) {
    return year;
  }

  days -= LEAP_YEAR(year) ? 366 : 365;
  calculated__daysSinceEpoch -= days; // now it is days in this year, starting at 0
  
  days = 0;
  uint8_t month = 0;
  uint8_t monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = _monthDays[month];
    }
    
    if (calculated__daysSinceEpoch >= monthLength) {
      calculated__daysSinceEpoch -= monthLength;
    } else {
      break;
    }
  }
  if (monthOnly) {
    return month + 1; // january is month 1
  }
  if (dayOnly) {
    return calculated__daysSinceEpoch + 1; // day of month
  }
}

int DateTimeClass::day() {
  return _generateDate(false, false, true);
}

int DateTimeClass::month() {
  return _generateDate(false, true, false);
}

int DateTimeClass::year() {
  return 1970 + _generateDate(true, false, false);
}
