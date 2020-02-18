#ifndef APP_BASE_CLASS_H
  #include <prism/app.hpp>
#endif

class HomeApp : public App {
  private:
  bool is_wifi_connected;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint16_t year;

  public:
  HomeApp();
  void draw(Graphics graphics);
  void update(void);
};
