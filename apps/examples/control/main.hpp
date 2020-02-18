#include <prism/app.hpp>
#include <prism/http.hpp>
#include <prism/timer.hpp>

class sidevesh__control : public App {
  private:
  char response[10];
  int ledPin;
  bool isLightOn;
  Http request = Http("prismbackend.herokuapp.com");
  Timer timer;

  public:
  sidevesh__control();
  void mount();
  void draw(Graphics graphics);
  void update(void);
};
