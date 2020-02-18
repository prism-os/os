#pragma once
#ifndef HTTP_FETCH_H
  #define HTTP_FETCH_H

  #define HTTP_INTIALIZED 0
  #define HTTP_WAITING_TO_CONNECT -2
  #define HTTP_WAITING_FOR_RESPONSE -3
  #define HTTP_OUT_OF_MEMORY -4
  #define HTTP_CONNECTION_ERROR -5
  #define HTTP_INVALID_RESPONSE -6
  #define HTTP_DISCONNECTED_BEFORE_RESPONSE -7
  #define HTTP_TIMEOUT -8
  #define HTTP_PORT 80

  #ifndef _ASYNCTCP_H_
    #include <ESPAsyncTCP.h>
  #endif

  class Http {
    private:
    AsyncClient *aClient = NULL;
    const char* content_type;
    const char* host;
    int port;
    const char* path;
    const char* method;
    const char* body;
    const char* headers[10];
    int num_headers;
    char* responseString = NULL;
    int status;
    int read_status;
    int get_request_length();
    void get_request_string(char* request_string);
    void request();
    static void onPacket(void *arg, AsyncClient *client, void *b);
    static void onError(void *arg, AsyncClient *client, int error);
    static void onConnect(void *arg, AsyncClient *client);
    static void onDisconnect(void *arg, AsyncClient *client);
    int packet_processed_count = 0;
    bool pre_body_break_found = false;
    bool post_body_break_found = false;

    public:
    Http(const char* host, int port = HTTP_PORT);
    void addHeader(const char* header);
    void setContentType(const char* content_type);
    void writeResponseTo(char* response);
    void get(const char* path);
    void post(const char* path, const char* body = NULL);
    void put(const char* path, const char* body = NULL);
    void del(const char* path, const char* body = NULL);
    bool isComplete();
    int getStatus();
  };

#endif
