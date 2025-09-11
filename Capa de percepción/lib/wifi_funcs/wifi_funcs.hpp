#ifndef WIFI_FUNCS_HPP
#define WIFI_FUNCS_HPP

#include <Arduino.h>
#include <WiFi.h>


void ask_credentials(String &ssid, String &pass);

bool connect_to_wifi(const String &ssid, const String &pass);





#endif //WIFI_FUNCS_HPP