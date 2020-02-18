#ifndef DISPLAY_H
  #include <prism/graphics.hpp>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#ifndef _U8G2LIB_HH
  #include <U8g2lib.h>
#endif

#ifndef TOPBAR_SPACE
  #define TOPBAR_SPACE 11 + 1
#endif

#ifndef MARKER_LEFT_SPACE
  #define MARKER_LEFT_SPACE 2
#endif

#ifndef MARKER_RIGHT_SPACE
  #define MARKER_RIGHT_SPACE 2
#endif

Graphics::Graphics(U8G2_SSD1306_128X64_NONAME_1_HW_I2C* u8g2) : _BACK_ICON({
  0, 0, 1, 0, 0, 0,
  0, 1, 1, 0, 0, 0,
  1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1,
  0, 1, 1, 0, 1, 1,
  0, 0, 1, 0, 1, 1,
  0, 0, 0, 0, 1, 1,
  0, 0, 0, 0, 1, 1,
  0, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 0
}), _u8g2(u8g2) {}

// Private method so no need to check if the x, y supplied is correct,
// or to adjust it to keep within app drawable area
void Graphics::_icon(int* iconArray, int x, int y) {
  for (int i = 0; i < Graphics::NORMAL_FONT_WIDTH; i++) {
    for (int j = 0; j < Graphics::NORMAL_FONT_HEIGHT; j++) {
      if (iconArray[(j * 6) + i] == 1) {
        _u8g2->drawPixel(x + i, x + j);
      }
    }
  }
}

int Graphics::getWidgetAreaWidth(void) {
  return _u8g2->getDisplayWidth() - TOPBAR_SPACE;
}

int Graphics::getWidgetAreaHeight(void) {
  return _u8g2->getDisplayHeight() - (MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE);
}

void Graphics::topBar(String str) {
  _u8g2->setDrawColor(0);
  _u8g2->drawBox(0, 0, _u8g2->getDisplayWidth(), TOPBAR_SPACE);
  _u8g2->setDrawColor(1);
  int limit = (_u8g2->getDisplayWidth() - 1) / Graphics::NORMAL_FONT_WIDTH;
  _u8g2->setFont(u8g2_font_t0_11b_mf);
  if (str.length() > limit) {
    _u8g2->drawStr(1, 0, str.substring(0, limit).c_str());
  } else {
    _u8g2->drawStr(1, 0, str.c_str());
  }
  _u8g2->setFont(u8g2_font_t0_11_mf);
}

void Graphics::backBar() {
  topBar("");
  _icon(_BACK_ICON, 0, 0);
}

void Graphics::markerUp() {
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 1, 1);
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 2, 0);
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 3, 1);
}

void Graphics::markerEnter() {
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 2, 4);
}

void Graphics::markerDown() {
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 1, 7);
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 2, 8);
  _u8g2->drawPixel(_u8g2->getDisplayWidth() - 3, 7);
}

String Graphics::ellipsis(String str, int limit) {
  if (str.length() > limit) {
    return str.substring(0, limit - 2) + "..";
  }

  return str;
}

void Graphics::line(String str, int x, int y, int limit, bool bold, bool large) {
  if (x < 0 || y < 0 || x + MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE >= _u8g2->getDisplayWidth() || y + Graphics::NORMAL_FONT_HEIGHT + 1 >= _u8g2->getDisplayHeight()) {
    return;
  }
  int font_width = large ? Graphics::LARGE_FONT_WIDTH : Graphics::NORMAL_FONT_WIDTH;
  int finalLimit = (_u8g2->getDisplayWidth() - (x + MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE)) / font_width;
  if (limit != 0 && limit < finalLimit) {
    finalLimit = limit;
  }
  if (large && bold) {
    _u8g2->setFont(u8g2_font_t0_22b_mf);
  } else if (large && !bold) {
    _u8g2->setFont(u8g2_font_t0_22_mf);
  } else if (!large && bold) {
    _u8g2->setFont(u8g2_font_t0_11b_mf);
  }
  _u8g2->drawStr(x + MARKER_LEFT_SPACE, y + TOPBAR_SPACE, ellipsis(str, finalLimit).c_str());
  if (large || bold) {
    _u8g2->setFont(u8g2_font_t0_11_mf);
  }
}

bool Graphics::paragraph(String str, int x, int y, int inp_rows, int inp_cols, int offset, bool bold) {
  int rows = 0;
  int cols = 0;
  int curRow = 0;
  int curCol = 0;
  char currentCharString[2];
  if (inp_rows == 0 || inp_cols == 0) {
    rows = (_u8g2->getDisplayHeight() - (y + TOPBAR_SPACE)) / Graphics::NORMAL_FONT_HEIGHT;
    cols = (_u8g2->getDisplayWidth() - (x + MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE)) / Graphics::NORMAL_FONT_WIDTH;
  } else {
    rows = inp_rows;
    cols = inp_cols;
  }
  if (str.length() < offset * cols || rows < 1 || cols < 1) {
    return false;
  }
  for (int i = 0; i < str.length(); i++) {
    if (i < offset * cols) {
      continue;
    }
    if (curCol >= cols) {
      curRow += 1;
      curCol = 0;
    };
    if (curRow >= rows) {
      return true;
    }
    currentCharString[0] = str.charAt(i);
    currentCharString[1] = '\0';
    if (bold) {
      _u8g2->setFont(u8g2_font_t0_11b_mf);
    }
    _u8g2->drawStr((curCol * Graphics::NORMAL_FONT_WIDTH) + x + MARKER_LEFT_SPACE, (curRow * Graphics::NORMAL_FONT_HEIGHT) + y + TOPBAR_SPACE, currentCharString);
    if (bold) {
      _u8g2->setFont(u8g2_font_t0_11_mf);
    }
    curCol += 1;
  }

  return false;
}

void Graphics::button(String str, int x, int y, bool active) {
  if (x < 0 || y < 0 || x + MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE >= _u8g2->getDisplayWidth() || y + TOPBAR_SPACE >= _u8g2->getDisplayHeight()) {
    return;
  }
  int limit = (_u8g2->getDisplayWidth() - (x + MARKER_LEFT_SPACE + MARKER_RIGHT_SPACE + 3 * 2)) / Graphics::NORMAL_FONT_WIDTH;
  if (str.length() > limit) {
    if (active) {
      _u8g2->drawRBox(x + MARKER_LEFT_SPACE, y + TOPBAR_SPACE, Graphics::NORMAL_FONT_WIDTH * str.substring(0, limit).length() + 3 * 2, Graphics::NORMAL_FONT_HEIGHT + 3 * 2, 2);
      _u8g2->setDrawColor(0);
      _u8g2->drawStr(x + MARKER_LEFT_SPACE + 3, y + TOPBAR_SPACE + 3, str.substring(0, limit).c_str());
      _u8g2->setDrawColor(1);
    } else {
      _u8g2->drawRFrame(x + MARKER_LEFT_SPACE, y + TOPBAR_SPACE, Graphics::NORMAL_FONT_WIDTH * str.substring(0, limit).length() + 3 * 2, Graphics::NORMAL_FONT_HEIGHT + 3 * 2, 2);
      _u8g2->drawStr(x + MARKER_LEFT_SPACE + 3, y + TOPBAR_SPACE + 3, str.substring(0, limit).c_str());
    }
  } else {
    if (active) {
      _u8g2->drawRBox(x + MARKER_LEFT_SPACE, y + TOPBAR_SPACE, Graphics::NORMAL_FONT_WIDTH * str.length() + 3 * 2, Graphics::NORMAL_FONT_HEIGHT + 3 * 2, 2);
      _u8g2->setDrawColor(0);
      _u8g2->drawStr(x + MARKER_LEFT_SPACE + 3, y + TOPBAR_SPACE + 3, str.c_str());
      _u8g2->setDrawColor(1);
    } else {
      _u8g2->drawRFrame(x + MARKER_LEFT_SPACE, y + TOPBAR_SPACE, Graphics::NORMAL_FONT_WIDTH * str.length() + 3 * 2, Graphics::NORMAL_FONT_HEIGHT + 3 * 2, 2);
      _u8g2->drawStr(x + MARKER_LEFT_SPACE + 3, y + TOPBAR_SPACE + 3, str.c_str());
    }
  }
}
