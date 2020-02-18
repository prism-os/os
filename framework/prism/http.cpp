#ifndef Arduino_h
  #include <Arduino.h>
#endif

#ifndef HTTP_FETCH_H
  #include <prism/http.hpp>
#endif

Http::Http(const char* req_host, int req_port) {
  host = req_host;
  port = req_port;
  content_type = "application/x-www-form-urlencoded";
  num_headers = 0;
  read_status = 0;
  status = HTTP_INTIALIZED;
}

void Http::addHeader(const char* header) {
  headers[num_headers] = header;
  num_headers++;
}

void Http::setContentType(const char* req_content_type) {
  content_type = req_content_type;
}

int Http::getStatus() {
  return status;
}

bool Http::isComplete() {
  /* Error cases are also considered onComplete at which point user can check for error by getting status */
  return status != HTTP_INTIALIZED && status != HTTP_WAITING_TO_CONNECT && status != HTTP_WAITING_FOR_RESPONSE;
}

void Http::writeResponseTo(char* response) {
  responseString = response;
}

void Http::get(const char* req_path) {
  path = req_path;
  method = "GET";
  body = NULL;
  request();
}
void Http::post(const char* req_path, const char* req_body) {
  path = req_path;
  method = "POST";
  body = req_body;
  request();
}

void Http::put(const char* req_path, const char* req_body) {
  path = req_path;
  method = "PUT";
  body = req_body;
  request();
}

void Http::del(const char* req_path, const char* req_body) {
  path = req_path;
  method = "DELETE";
  body = req_body;
  request();
}

int Http::get_request_length() {
  int method_path_version_length = strlen(method) + strlen(" ") + strlen(path) + strlen(" HTTP/1.1\r\n");
  int header_length = 0;
  for (int i = 0; i < num_headers; i++) {
    header_length += strlen(headers[i]);
    header_length += strlen("\r\n");
  }
  int host_connection_close_length = strlen("Host: ") + strlen(host) + strlen("\r\n") + strlen("Connection: close\r\n");
  int body_section_length = 0;
  if (body != NULL) {
    String content_length_string = String(strlen(body));
    int content_length_length = strlen("Content-Length: ") + content_length_string.length() + strlen("\r\n");
    int content_type_length = strlen("Content-Type: ") + strlen(content_type) + strlen("\r\n");
    int body_length = strlen(body) + strlen("\r\n") + strlen("\r\n");
    body_section_length = content_length_length + content_type_length + body_length;
  }
  int extra_break_at_end_length = strlen("\r\n");
  return method_path_version_length + header_length + host_connection_close_length + body_section_length + extra_break_at_end_length;
}

void Http::get_request_string(char* request_string) {
  request_string[0] = '\0';
  strcat(request_string, method);
  strcat(request_string, " ");
  strcat(request_string, path);
  strcat(request_string, " HTTP/1.1\r\n");
  for (int i = 0; i < num_headers; i++) {
    strcat(request_string, headers[i]);
    strcat(request_string, "\r\n");
  }
  strcat(request_string, "Host: ");
  strcat(request_string, host);
  strcat(request_string, "\r\n");
  strcat(request_string, "Connection: close\r\n");
  if (body != NULL) {
    char contentLength[30];
    sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
    strcat(request_string, contentLength);
	strcat(request_string, "Content-Type: ");
	strcat(request_string, content_type);
	strcat(request_string, "\r\n");
  }
  strcat(request_string, "\r\n");
  if (body != NULL) {
    strcat(request_string, body);
    strcat(request_string, "\r\n");
    strcat(request_string, "\r\n");
  }
}

void Http::onError(void *arg, AsyncClient *client, int error) {
  #ifdef HTTP_DEBUG
    Serial.println("ERROR, error code:");
    Serial.println(error);
  #endif
  Http *self = (Http*) arg;
  self->aClient = NULL;
  self->status = HTTP_CONNECTION_ERROR;
  self->packet_processed_count = 0;
  self->pre_body_break_found = false;
  self->post_body_break_found = false;
  self->read_status = 0;
  client->onDisconnect(NULL, NULL);
  client->onPacket(NULL, NULL);
	delete client;
}

void Http::onDisconnect(void *arg, AsyncClient *client) {
  #ifdef HTTP_DEBUG
    Serial.println("DISCONNECTED");
  #endif
  Http *self = (Http*) arg;
  self->aClient = NULL;
  if (self->status == HTTP_WAITING_FOR_RESPONSE) {
    self->status = HTTP_DISCONNECTED_BEFORE_RESPONSE;
  }
  self->packet_processed_count = 0;
  self->pre_body_break_found = false;
  self->post_body_break_found = false;
  self->read_status = 0;
  client->onDisconnect(NULL, NULL);
  client->onPacket(NULL, NULL);
	delete client;
}

void Http::onPacket(void *arg, AsyncClient *client, void *b) {
  Http *self = (Http*) arg;
  pbuf* buf = (pbuf*)b;
  #ifdef HTTP_DEBUG
    Serial.println("PACKET no:");
    Serial.println(self->packet_processed_count);
    Serial.println("PACKET length:");
    Serial.println(buf->len);
    Serial.println("PACKET contents:");
    Serial.println("===================================================");
    Serial.write((uint8_t*)buf->payload, buf->len);
    Serial.println("===================================================");
  #endif
  if (self->packet_processed_count == 0) {
    if (strncmp((char*)buf->payload, "HTTP/1.1", 8) == 0) {
      self->read_status = atoi((char*)buf->payload + 9);
      #ifdef HTTP_DEBUG
        Serial.println("HTTP/1.1, code:");
        Serial.println(self->read_status);
      #endif
    } else {
      #ifdef HTTP_DEBUG
        Serial.println("NOT HTTP/1.1");
      #endif
      Http *self = (Http*) arg;
      self->aClient = NULL;
      self->status = HTTP_INVALID_RESPONSE;
      self->packet_processed_count = 0;
      self->pre_body_break_found = false;
      self->post_body_break_found = false;
      self->read_status = 0;
      client->onDisconnect(NULL, NULL);
      client->onPacket(NULL, NULL);
      delete client;
      return;
    }
  }
  if (!self->pre_body_break_found && !self->post_body_break_found) {
    #ifdef HTTP_DEBUG
      Serial.println("PACKET nothing found till far");
    #endif
    if (self->responseString != NULL) {
      #ifdef HTTP_DEBUG
        Serial.println("PACKET has body");
      #endif
      char* body_start = strstr((char*)buf->payload, "\r\n\r\n");
      if (body_start != NULL) {
        #ifdef HTTP_DEBUG
          Serial.println("PACKET body start found");
        #endif
        self->pre_body_break_found = true;
        body_start += 4;
        char* body_end = body_start - (char*)buf->payload >= buf->len ? NULL : strstr(body_start, "\r\n\r\n");
        if (body_end != NULL) {
          #ifdef HTTP_DEBUG
            Serial.println("PACKET body end found");
          #endif
          self->post_body_break_found = true;
          // WRITE BODY: body and whole http message starts and ends in this packet itself, write to responseString
          if (self->responseString != NULL) {
            strncpy(self->responseString, body_start, body_end - body_start);
            self->responseString[body_end - body_start] = '\0';
            #ifdef HTTP_DEBUG
              Serial.println("PACKET body written till far:");
              Serial.println("===================================================");
              Serial.println(self->responseString);
              Serial.println("===================================================");
            #endif
          }
        } else {
          #ifdef HTTP_DEBUG
            Serial.println("PACKET body end not found");
          #endif
          // WRITE BODY: body ends in other packet, write to responseString and then once other packet is received, write again
          if (self->responseString != NULL) {
            if (body_start - (char*)buf->payload >= buf->len) {
              #ifdef HTTP_DEBUG
                Serial.println("Case of start break on first packet right at end, overflowed position and packet length:");
                Serial.println(body_start - (char*)buf->payload);
                Serial.println(buf->len);
              #endif
              self->responseString[0] = '\0';
            } else {
              strncpy(self->responseString, body_start, buf->len - (body_start - (char*)buf->payload));
              self->responseString[buf->len - (body_start - (char*)buf->payload)] = '\0';
            }
            #ifdef HTTP_DEBUG
              Serial.println("PACKET body written till far:");
              Serial.println("===================================================");
              Serial.println(self->responseString);
              Serial.println("===================================================");
            #endif
          }
        }
      }
    } else {
      #ifdef HTTP_DEBUG
        Serial.println("PACKET has no body");
      #endif
      char* packet_end = strstr((char*)buf->payload, "\r\n\r\n");
      if (packet_end != NULL) {
        #ifdef HTTP_DEBUG
          Serial.println("PACKET end found");
        #endif
        self->pre_body_break_found = true;
        self->post_body_break_found = true;
      }
    }
  } else if (self->pre_body_break_found && !self->post_body_break_found) {
    #ifdef HTTP_DEBUG
      Serial.println("PACKET body start found but end not found");
    #endif
    char* packet_end = strstr((char*)buf->payload, "\r\n\r\n");
    if (packet_end != NULL) {
      #ifdef HTTP_DEBUG
        Serial.println("PACKET body end found");
      #endif
      self->post_body_break_found = true;
      // WRITE BODY: write till this double break
      if (self->responseString != NULL) {
        strncat(self->responseString, (char*)buf->payload, packet_end - (char*)buf->payload);
        #ifdef HTTP_DEBUG
          Serial.println("PACKET body written till far:");
          Serial.println("===================================================");
          Serial.println(self->responseString);
          Serial.println("===================================================");
        #endif
      }
    } else {
      #ifdef HTTP_DEBUG
        Serial.println("PACKET body end not found again");
      #endif
      // WRITE BODY: more data but still no double break, write till end and then write more in next iteration
      if (self->responseString != NULL) {
        strncat(self->responseString, (char*)buf->payload, buf->len);
        #ifdef HTTP_DEBUG
          Serial.println("PACKET body written till far:");
          Serial.println("===================================================");
          Serial.println(self->responseString);
          Serial.println("===================================================");
        #endif
      }
    }
  }
  if (self->pre_body_break_found && self->post_body_break_found) {
    #ifdef HTTP_DEBUG
      Serial.println("COMPLETED");
    #endif
    self->aClient = NULL;
    self->packet_processed_count = 0;
    self->pre_body_break_found = false;
    self->post_body_break_found = false;
    self->status = self->read_status;
    self->read_status = 0;
    client->onDisconnect(NULL, NULL);
    client->onPacket(NULL, NULL);
    delete client;
  }
  self->packet_processed_count += 1;
}

void Http::onConnect(void *arg, AsyncClient *client) {
  Http *self = (Http*) arg;
  self->status = HTTP_WAITING_FOR_RESPONSE;
  self->aClient->onError(NULL, NULL);
  self->aClient->onConnect(NULL, NULL);
	client->onDisconnect(self->onDisconnect, self);
  client->onPacket(self->onPacket, self);
  int request_length = self->get_request_length() + 10;
  char request_string[request_length];
  self->get_request_string(request_string);
  #ifdef HTTP_DEBUG
    Serial.println("CONNECTED, request:");
    Serial.println(request_string);
  #endif
	client->write(request_string);
}

void Http::request() {
  if (aClient) {
    return;
  }
  packet_processed_count = 0;
  pre_body_break_found = false;
  post_body_break_found = false;
  read_status = 0;
  aClient = new AsyncClient();
	if (!aClient) {
    status = HTTP_OUT_OF_MEMORY;
		return;
  }
  status = HTTP_WAITING_TO_CONNECT;
	aClient->onError(onError, this);
	aClient->onConnect(onConnect, this);
  aClient->connect(host, port);
}
