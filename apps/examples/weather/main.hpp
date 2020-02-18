#include <prism/app.hpp>

#ifndef HTTP_FETCH_H
  #include <prism/http.hpp>
#endif

#ifndef TIMER_H
  #include <prism/timer.hpp>
#endif

class sidevesh__weather : public App {
  private:
  char currentWeather[5];
  char response[10];
  Http request = Http("prismbackend.herokuapp.com");
  Timer timer;

  public:
  sidevesh__weather();
  void mount();
  void draw(Graphics graphics);
  void update(void);
};
