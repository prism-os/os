#pragma once
#ifndef API_H
  #define API_H

  class ApiClass {
    private:
    bool _appsAllFound(int a[], int b[], int cnt);
    char* _generateAppsOrderListJson(void);
    char* _generateHeartbeatJson(void);
    char* _generateVersionResponseJson(void);
    char* _generateOnlineReconnResponseJson(void);

    public:
    ApiClass(void);
    char heartbeatData[20];
    void check(void);
  };

  extern ApiClass Api;

#endif
