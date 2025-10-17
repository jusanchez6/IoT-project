#ifndef WIFI_FUNCS_HPP
#define WIFI_FUNCS_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>


#include <types.hpp>



void setup_wifi();

void syncTime();

void sendData(SensorData_t& sensor_data);

void init_communications();

#endif //WIFI_FUNCS_HPP