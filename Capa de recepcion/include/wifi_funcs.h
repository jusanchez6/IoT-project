
#ifndef WIFI_FUNCS_H
#define WIFI_FUNCS_H

#include <WiFi.h>

void ask_wifi_credentials(String &ssid, String &password);
void connect_to_wifi(const String &ssid, const String &password);


#endif  // WIFI_FUNCS_H