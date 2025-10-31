#ifndef WIFI_FUNCS_HPP
#define WIFI_FUNCS_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>


#include <types.hpp>



void ask_credentials(String &ssid, String &pass);

bool connect_to_wifi(const String &ssid, const String &pass);

void setup_wifi();

void syncTime();

// void sendData(&SensorData_t);


#endif //WIFI_FUNCS_HPP