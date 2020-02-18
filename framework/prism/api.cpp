#ifndef API_H
  #include <prism/api.hpp>
#endif

#ifndef EEPROM_h
  #include <EEPROM.h>
#endif

#ifndef String_class_h
  #include <WString.h>
#endif

#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>

#ifndef VERSION_H
  #include <prism/version.hpp>
#endif

#ifndef INTERNAL_CONSTANTS_H
  #include <prism/internal_constants.hpp>
#endif

#ifndef BUTTONS_H
  #include <prism/buttons.hpp>
#endif

#ifndef BLOCKING_H
  #include <prism/blocking.hpp>
#endif

#ifndef PAIR_H
  #include <prism/pair.hpp>
#endif

#ifndef COMPOSER_H
  #include <prism/composer.hpp>
#endif

#ifndef APPS_H
  #include <prism/apps.hpp>
#endif

#ifndef DISPLAY_H
  #include <prism/display.hpp>
#endif

ApiClass::ApiClass() : heartbeatData({0}) {}

bool ApiClass::_appsAllFound(int a[], int b[], int cnt) {
  bool cmp_found = false;
  bool cmp_fail = false;
  for (int j = 1; j <= cnt; j++) {
    cmp_found = false;
    for (int k = 1; k <= cnt; k++) {
      if (a[j] == b[k]) {
        cmp_found = true;
        break;
      }
    }
    if (!cmp_found) {
      cmp_fail = true;
      break;
    }
  }
  return !cmp_fail;
}

char* ApiClass::_generateAppsOrderListJson() {
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& send_json_obj = jsonBuffer.createObject();
  send_json_obj["application"] = "system";
  send_json_obj["sub_module"] = "apps_order";
  JsonObject& send_json_data_obj = send_json_obj.createNestedObject("data");
  send_json_data_obj["action"] = "list_payload";

  JsonArray& send_json_data_array_obj = send_json_data_obj.createNestedArray("array");
  char* array_item;
  for (int i = 1; i <= Apps.getCount(); i++) {
    //String widgetAbility_flag = ability_call(Apps.getAppIdFromSortedIndex(i), PRISM_WIDGET) ? "w1" : "w0";
    array_item = strdup(
                        (
                          String(Apps.getAppIdFromSortedIndex(i))
                          + "|"
                          + Apps.getAppInstanceFromSortedIndex(i)->name
                          + "|w"
                          + (!Apps.getAppInstanceFromSortedIndex(i)->widgetAbility ? "X" : Blocking.isBlocked(Apps.getAppIdFromSortedIndex(i), PRISM_WIDGET) ? "B" : "U" )
                          + "|n"
                          + (!Apps.getAppInstanceFromSortedIndex(i)->notifyAbility ? "X" : Blocking.isBlocked(Apps.getAppIdFromSortedIndex(i), PRISM_NOTIFICATION) ? "B" : "U" )
                        ).c_str()
                      );
    send_json_data_array_obj.add(array_item);
  }

  //String apps_list_string = "";
  //for (int i = 1; i <= Apps.getCount(); i++) {
  //  apps_list_string += Apps.getAppIdFromSortedIndex(i);
  //  if (i != Apps.getCount()) {
  //    apps_list_string += ',';
  //  }
  //}
  //send_json_data_obj["array"] = apps_list_string.c_str();

  char send_json[300];
  send_json_obj.printTo(send_json, sizeof(send_json));
  return send_json;
}

char* ApiClass::_generateHeartbeatJson() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& send_json_obj = jsonBuffer.createObject();
  send_json_obj["application"] = "system";
  send_json_obj["sub_module"] = "heartbeat";
  JsonObject& send_json_data_obj = send_json_obj.createNestedObject("data");
  send_json_data_obj["action"] = "client_response";
  char send_json[200];
  send_json_obj.printTo(send_json, sizeof(send_json));
  return send_json;
}

char* ApiClass::_generateVersionResponseJson() {
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& send_json_obj = jsonBuffer.createObject();
  send_json_obj["application"] = "system";
  send_json_obj["sub_module"] = "version_info";
  JsonObject& send_json_data_obj = send_json_obj.createNestedObject("data");
  send_json_data_obj["action"] = "info_request_response";
  send_json_data_obj["user_account_id"] = USER_ACCOUNT_ID;
  send_json_data_obj["user_image_id"] = USER_IMAGE_ID;
  send_json_data_obj["system_image_id"] = SYSTEM_IMAGE_ID;
  char send_json[250];
  send_json_obj.printTo(send_json, sizeof(send_json));
  return send_json;
}

char* ApiClass::_generateOnlineReconnResponseJson() {
  char _deviceId_to_send[7];
  Pair.getClientId().toCharArray(_deviceId_to_send, PRISM_DEVICE_ID_LENGTH);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& done_json_obj = jsonBuffer.createObject();
  done_json_obj["application"] = "system";
  done_json_obj["sub_module"] = "online_udp_reconnection";
  JsonObject& done_json_data_obj = done_json_obj.createNestedObject("data");
  done_json_data_obj["action"] = "client_online_reconn_accept";
  done_json_data_obj["client_id"] = _deviceId_to_send;
  char done_json[200];
  done_json_obj.printTo(done_json, sizeof(done_json));
  return done_json;
}


void ApiClass::check(void) {
  int packetSize;
  int packetLen;
  char incomingPacket[255];
  StaticJsonBuffer<300> jsonBuffer;
  IPAddress ret_ip_obj;
  String ip_string_obj;
  char ret_ip_char_obj[16] = "xxx.xxx.xxx.xxx";
  packetSize = Pair.getUDP().parsePacket();
  if (packetSize) {
    packetLen = Pair.getUDP().read(incomingPacket, 255);
    if (packetLen > 0) {
      ret_ip_obj = Pair.getUDP().remoteIP();
      ip_string_obj = String(ret_ip_obj[0]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[1]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[2]);
      ip_string_obj = ip_string_obj + ".";
      ip_string_obj = ip_string_obj + String(ret_ip_obj[3]);
      ip_string_obj.toCharArray(ret_ip_char_obj, IP_AS_STRING_LENGTH);
      if (String(ret_ip_char_obj) == String(Pair.getServerIp())) {
        JsonObject& resp_json = jsonBuffer.parseObject(incomingPacket);
        if (resp_json.success()) {
          const char* resp_json_application = resp_json["application"];
          const char* resp_json_sub_module = resp_json["sub_module"];
          if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("heartbeat"))) {
            strcpy(heartbeatData, resp_json["data"]["timestamp"]);
            char send_json[200];
            strcpy(send_json, _generateHeartbeatJson());
            Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
            Pair.getUDP().write(send_json);
            Pair.getUDP().endPacket();
          } else if ((String(resp_json_application) == String("system") ) && (String(resp_json_sub_module) == String("version_info"))) {
            char send_json[250];
            strcpy(send_json, _generateVersionResponseJson());
            Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
            Pair.getUDP().write(send_json);
            Pair.getUDP().endPacket();
          } else if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("ota_update"))) {
            const char* resp_json_data_url = resp_json["data"]["url"];
            bool resp_json_data_format = resp_json["data"]["format"];
            if (resp_json_data_format) {
              EEPROM.begin(512);
              for (int i = 0; i < 512; i++) {
                EEPROM.write(i, 0);
              }
              EEPROM.end();
            }
            Display.u8g2.firstPage();
            do {
              Display.DONTUSEdrawStr("Updating...", 0, 0);
            } while (Display.u8g2.nextPage());
            ESPhttpUpdate.update(resp_json_data_url);
          } else if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("online_udp_reconnection"))) {
            const char* resp_json_data_action = resp_json["data"]["action"];
            const char* resp_json_data_server_id = resp_json["data"]["server_id"];
            if ((String(resp_json_data_action) == String("server_online_reconn_request")) && (String(resp_json_data_server_id) == Pair.getServerId())) {
              Pair.setServerIp(ret_ip_char_obj);
              char send_json[200];
              strcpy(send_json, _generateOnlineReconnResponseJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            }
          } else if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("buttons_control"))) {
            const char* resp_json_data_action = resp_json["data"]["action"];
            if (String(resp_json_data_action) == String("enter")) {
              Buttons.setEnter();
            } else if (String(resp_json_data_action) == String("left")) {
              Buttons.setLeft();
            } else if (String(resp_json_data_action) == String("right")) {
              Buttons.setRight();
            } else if (String(resp_json_data_action) == String("down")) {
              Buttons.setDown();
            } else if (String(resp_json_data_action) == String("up")) {
              Buttons.setUp();
            }
          } else if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("apps_order"))) {
            const char* resp_json_data_action = resp_json["data"]["action"];
            if (String(resp_json_data_action) == String("list_request")) {
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            } else if (String(resp_json_data_action) == String("list_update")) {
              const char* resp_json_data_array = resp_json["data"]["array"];
              char* resp_json_data_array_for_tok = strdup(resp_json_data_array);
              char delim[2] = ",";
              char* token;
              token = strtok(resp_json_data_array_for_tok, delim);
              int i = 1;
              int new_apps_list[PRISM_MAX_ALLOWED_INSTALLATIONS];
              new_apps_list[0] = 0;
              for (int j = 1; j < PRISM_MAX_ALLOWED_INSTALLATIONS; j++) {
                new_apps_list[j] = -1;
              }
              while (token != NULL) {
                new_apps_list[i] = String(token).toInt();
                i++;
                token = strtok(NULL, delim);
              }
              i--;
              if (i == Apps.getCount()) {
                if ((_appsAllFound(new_apps_list, Apps.orderedList, i)) && _appsAllFound(Apps.orderedList, new_apps_list, i)) {
                  for (int j = 1; j <= i; j++) {
                    Apps.orderedList[j] = new_apps_list[j];
                  }
                  Apps.count = i;
                  char ok[2 + 1] = "OK";
                  EEPROM.begin(512);
                  EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)), ok);
                  EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok), Apps.orderedList);
                  EEPROM.commit();
                  EEPROM.end();
                }
              }
              /*
              while (token != NULL) {
                Apps.orderedList[i] = String(token);
                i++;
                token = strtok(NULL, delim);
              }
              Apps.count = i - 1;
              */
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            }
          } else if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("apps_blocking"))) {
            const char* resp_json_data_action = resp_json["data"]["action"];
            int resp_json_data_app_id = resp_json["data"]["app_id"];
            if (String(resp_json_data_action) == String("widget_block")) {
              if ((Apps.isExists(resp_json_data_app_id)) && (resp_json_data_app_id != PRISM_HOME_APP_ID) && (Apps.getAppInstanceFromId(resp_json_data_app_id)->widgetAbility) && (!Blocking.isBlocked(resp_json_data_app_id, PRISM_WIDGET))) {
                Blocking.blockApp(int(resp_json_data_app_id), PRISM_WIDGET);
                char ok[2 + 1] = "OK";
                EEPROM.begin(512);
                EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
                EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), Blocking.getWidgetBlockedAppIds());
                EEPROM.commit();
                EEPROM.end();
                if (Composer.getCurrentAppId() == int(resp_json_data_app_id)) {
                  Composer.setNextAppId(1);
                }
              }
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            } else if (String(resp_json_data_action) == String("widget_unblock")) {
              Blocking.unblockApp(resp_json_data_app_id, PRISM_WIDGET);
              char ok[2 + 1] = "OK";
              EEPROM.begin(512);
              EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
              EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), Blocking.getWidgetBlockedAppIds());
              EEPROM.commit();
              EEPROM.end();
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            } else if (String(resp_json_data_action) == String("notification_block")) {
              if ((Apps.isExists(resp_json_data_app_id)) && (Apps.getAppInstanceFromId(resp_json_data_app_id)->notifyAbility) && (!Blocking.isBlocked(resp_json_data_app_id, PRISM_NOTIFICATION))) {
                Blocking.blockApp(resp_json_data_app_id, PRISM_NOTIFICATION);
                char ok[2 + 1] = "OK";
                EEPROM.begin(512);
                EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
                EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), Blocking.getNotificationBlockedAppIds());
                EEPROM.commit();
                EEPROM.end();
              }
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            } else if (String(resp_json_data_action) == String("notification_unblock")) {
              Blocking.unblockApp(resp_json_data_app_id, PRISM_NOTIFICATION);
              char ok[2 + 1] = "OK";
              EEPROM.begin(512);
              EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)), ok);
              EEPROM.put(0 + sizeof(ok) + (10 * sizeof(char)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok) + (PRISM_MAX_ALLOWED_INSTALLATIONS * sizeof(int)) + sizeof(ok), Blocking.getNotificationBlockedAppIds());
              EEPROM.commit();
              EEPROM.end();
              char send_json[300];
              strcpy(send_json, _generateAppsOrderListJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            }
          }
        }
      } else {
        JsonObject& resp_json = jsonBuffer.parseObject(incomingPacket);
        if (resp_json.success()) {
          const char* resp_json_application = resp_json["application"];
          const char* resp_json_sub_module = resp_json["sub_module"];
          if ((String(resp_json_application) == String("system")) && (String(resp_json_sub_module) == String("online_udp_reconnection"))) {
            const char* resp_json_data_action = resp_json["data"]["action"];
            const char* resp_json_data_server_id = resp_json["data"]["server_id"];
            if ((String(resp_json_data_action) == String("server_online_reconn_request")) && (String(resp_json_data_server_id) == Pair.getServerId())) {
              Pair.setServerIp(ret_ip_char_obj);
              char send_json[200];
              strcpy(send_json, _generateOnlineReconnResponseJson());
              Pair.getUDP().beginPacket(Pair.getServerIp(), Pair.getServerPort());
              Pair.getUDP().write(send_json);
              Pair.getUDP().endPacket();
            }
          }
        }
      }
    }
  }
}
