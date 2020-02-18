#include "main.hpp"

sidevesh__weather::sidevesh__weather() {
  strcpy(currentWeather, "XX.X");
  request.writeResponseTo(response);
}

void sidevesh__weather::mount() {
  timer.start(5000);
}

void sidevesh__weather::draw(Graphics graphics) {
  graphics.line(String(currentWeather) + " C", 0, 0, 0 , false, true);
  if(String(currentWeather).toInt() >= 29){
    graphics.line("Sunny", 0, Graphics::LARGE_FONT_HEIGHT + 1);
  } else{
    graphics.line("Cloudy", 0, Graphics::LARGE_FONT_HEIGHT + 1);
  }
}

void sidevesh__weather::update(void) {
  if(timer.isOver()) {
    if(request.getStatus() == HTTP_INTIALIZED || request.isComplete()) {
      request.get("/api/weather");
    }
    timer.start(5000);
  }
  if(request.isComplete()) {
    strncpy(currentWeather, response + 4, 4);
    currentWeather[4] = '\0';
  }
}
