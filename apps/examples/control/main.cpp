#include "main.hpp"
#include <prism/buttons.hpp>

sidevesh__control::sidevesh__control() {
  ledPin = 14;
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  isLightOn = digitalRead(ledPin) == 1;
  request.writeResponseTo(response);
}

void sidevesh__control::mount() {
  timer.start(5000);
}

void sidevesh__control::draw(Graphics graphics) {
  graphics.markerEnter();
  if(isLightOn) {
    graphics.line("ON", 0, 0, 0, true, false);
    graphics.line("tap to turn off", 0, Graphics::NORMAL_FONT_HEIGHT + 1);
  } else {
    graphics.line("OFF", 0, 0, 0, true, false);
    graphics.line("tap to turn on", 0, Graphics::NORMAL_FONT_HEIGHT + 1);
  }
  if(!request.getStatus() == HTTP_INTIALIZED && !request.isComplete()) {
    graphics.line("syncing...", 0, (Graphics::NORMAL_FONT_HEIGHT + 1) * 2);
  }
}

void sidevesh__control::update(void) {
  if(request.getStatus() == HTTP_INTIALIZED || request.isComplete()) {
    if(Buttons.enterPressed()) {
      if(isLightOn) {
        request.get("/api/lighting/1/status/off");
      } else {
        request.get("/api/lighting/1/status/on");
      }
    }
  }
  if(timer.isOver()) {
    if(request.getStatus() == HTTP_INTIALIZED || request.isComplete()) {
      request.get("/api/lighting/1/status");
    }
    timer.start(5000);
  }
  if(request.isComplete()) {
    if(response[3] == 't' && !isLightOn) {
      digitalWrite(ledPin, HIGH);
      isLightOn = true;
    } else if(response[3] == 'f' && isLightOn) {
      digitalWrite(ledPin, LOW);
      isLightOn = false;
    }
  }
}
