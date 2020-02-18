#pragma once
#ifndef COMPOSER_H
  #define COMPOSER_H

  class ComposerClass {
    private:
    int _currentAppId;
    int _nextAppId;
    int _leftAppId;
    int _rightAppId;
    void _updateSwitchingOptions(void);

    public:
    ComposerClass(void);
    bool canSwitchLeft(void);
    bool canSwitchRight(void);
    int getCurrentAppId(void);
    void setNextAppId(int id);
    void draw(void);
    void update(void);
  };

  extern ComposerClass Composer;

#endif
