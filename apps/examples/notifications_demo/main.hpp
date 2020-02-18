#include <prism/app.hpp>

class sidevesh__notifications_demo : public App {
  private:
  int counter;

  public:
  sidevesh__notifications_demo();
  void draw(Graphics graphics);
  Notification notify(void);
};
