#ifndef WIFI_FUNCS_HPP
#define WIFI_FUNCS_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>


#include <types.hpp>

#define CLOUD_SERVER 1
#define DNS_PORT 53


extern Preferences prefs;
extern WebServer server;
extern DNSServer dns;

void ask_credentials(String &ssid, String &pass);

void syncTime();

bool reconnect();

void init_communications();

String msgToJson(const Telemetry_t &data);

bool sendData(const String &data);


String htmlPage(const String& currentSSID, const String& currentName);

void setupCaptiveRedirects();

void handleSanve();

void startAP();

bool connectToWiFi();

#endif //WIFI_FUNCS_HPP