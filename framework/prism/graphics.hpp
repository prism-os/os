#pragma once
#ifndef GRAPHICS_H
  #define GRAPHICS_H

  #ifndef String_class_h
    #include <WString.h>
  #endif

  #ifndef _U8G2LIB_HH
    #include <U8g2lib.h>
  #endif

  class Graphics {
    private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C* _u8g2;
    void _icon(int* iconArray, int x, int y);
    int _BACK_ICON[60];

    public:
    static const int NORMAL_FONT_WIDTH = 6;
    static const int NORMAL_FONT_HEIGHT = 11;
    static const int LARGE_FONT_WIDTH = 11;
    static const int LARGE_FONT_HEIGHT = 21;
    Graphics(U8G2_SSD1306_128X64_NONAME_1_HW_I2C* u8g2);
    int getWidgetAreaWidth(void);
    int getWidgetAreaHeight(void);
    void topBar(String str);
    void backBar(void);
    void markerUp(void);
    void markerEnter(void);
    void markerDown(void);
    String ellipsis(String str, int limit);
    void line(String str, int x, int y, int limit = 0, bool bold = false, bool large = false);
    bool paragraph(String str, int x, int y, int rows = 0, int cols = 0, int offset = 0, bool bold = false);
    void button(String str, int x, int y, bool active = true);
  };

#endif
