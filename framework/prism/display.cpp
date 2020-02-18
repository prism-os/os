#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef GRAPHICS_H
  #include <prism/graphics.hpp>
#endif

#ifndef _U8G2LIB_HH
  #include <U8g2lib.h>
#endif

#ifndef COMPOSER_H
  #include <prism/composer.hpp>
#endif

DisplayClass::DisplayClass() : u8g2(U8G2_R0, U8X8_PIN_NONE), graphics(&u8g2) {}

void DisplayClass::draw(bool receivedScreenShotCommand) {
  u8g2.firstPage();
  if (receivedScreenShotCommand) {
    Serial.print("s:{");
  }
  do {
    Composer.draw();
    if (receivedScreenShotCommand) {
      uint8_t* buff = u8g2.getBufferPtr();
      Serial.write(buff, 128);
      delay(10);
    }
  } while (u8g2.nextPage());
  if (receivedScreenShotCommand) {
    Serial.println("}");
  }
}

void DisplayClass::draw() {
  u8g2.firstPage();
  do {
    Composer.draw();
  } while (u8g2.nextPage());
}

void DisplayClass::initialize() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_11_mf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 28, 2, u8g2.getDisplayWidth() / 4 + 54, 38);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 54, 38, u8g2.getDisplayWidth() / 4 + 37, 46);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 37, 46, u8g2.getDisplayWidth() / 4 + 21, 40);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 21, 40, u8g2.getDisplayWidth() / 4 + 28, 2);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 28, 2, u8g2.getDisplayWidth() / 4 + 7, 36);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 7, 36, u8g2.getDisplayWidth() / 4 + 13, 49);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 13, 49, u8g2.getDisplayWidth() / 4 + 21, 40);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 13, 49, u8g2.getDisplayWidth() / 4 + 37, 62);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 37, 62, u8g2.getDisplayWidth() / 4 + 37, 46);
    u8g2.drawLine(u8g2.getDisplayWidth() / 4 + 37, 62, u8g2.getDisplayWidth() / 4 + 54, 38);
  } while (u8g2.nextPage());
}

void DisplayClass::DONTUSEdrawStr(String str, int x, int y) {
  u8g2.drawStr(x, y, str.c_str());
}

void DisplayClass::DONTUSEmarkerLeft() {
  u8g2.drawPixel(1, (u8g2.getDisplayHeight() / 2) - 1);
  u8g2.drawPixel(0, u8g2.getDisplayHeight() / 2);
  u8g2.drawPixel(1, (u8g2.getDisplayHeight() / 2) + 1);
}

void DisplayClass::DONTUSEmarkerRight() {
  u8g2.drawPixel(u8g2.getDisplayWidth() - 1 - 1, (u8g2.getDisplayHeight() / 2) - 1);
  u8g2.drawPixel(u8g2.getDisplayWidth() - 1, u8g2.getDisplayHeight() / 2);
  u8g2.drawPixel(u8g2.getDisplayWidth() - 1 - 1, (u8g2.getDisplayHeight() / 2) + 1);
}
