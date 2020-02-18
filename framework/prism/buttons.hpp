#pragma once
#ifndef BUTTONS_H
  #define BUTTONS_H

  class ButtonsClass {
    private:
    bool _leftPressed;
    bool _rightPressed;
    bool _upPressed;
    bool _downPressed;
    bool _enterPressed;

    public:
    ButtonsClass(void);
    void initialize(void);
    void setEnter(void);
    void setLeft(void);
    void setRight(void);
    void setDown(void);
    void setUp(void);
    void clear(void);

    bool leftPressed(void);
    bool rightPressed(void);
    bool upPressed(void);
    bool downPressed(void);
    bool enterPressed(void);
  };

  extern ButtonsClass Buttons;

#endif
