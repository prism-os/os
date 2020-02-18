#pragma once
#ifndef DISPLAY_H
  #define DISPLAY_H

  #ifndef String_class_h
    #include <WString.h>
  #endif

  #ifndef GRAPHICS_H
    #include <prism/graphics.hpp>
  #endif

  #ifndef _U8G2LIB_HH
    #include <U8g2lib.h>
  #endif

  class DisplayClass {
    public:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;
    Graphics graphics;
    DisplayClass(void);
    void draw(bool receivedScreenShotCommand);
    void draw(void);
    void initialize(void);
    void DONTUSEdrawStr(String str, int x, int y);
    void DONTUSEmarkerLeft(void);
    void DONTUSEmarkerRight(void);
  };

  extern DisplayClass Display;

#endif
