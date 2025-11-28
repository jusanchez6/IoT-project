/**
 * @file wifi_funcs.hpp
 * @brief Funciones para la gestión de WiFi, AP cautivo, sincronización NTP y comunicación segura (MQTT/HTTPS).
 *
 * Este archivo contiene las funciones necesarias para configurar un Access Point,
 * solicitar credenciales, conectarse a WiFi, sincronizar la hora, comunicarse con
 * un broker seguro y preparar mensajes JSON de telemetría.
 * @author Julián Sánchez
 * @date 15/09/2025
 * @version 3.0
 */

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

#define CLOUD_SERVER 1      ///< Indicador para habilitar comunicación con la nube.
#define DNS_PORT 53         ///< Puerto usado por el DNS del portal cautivo.

// --- Variables globales externas ---

extern Preferences prefs;   ///< Preferencias de almacenamiento (NVS).
extern WebServer server;    ///< Servidor web para portal cautivo.
extern DNSServer dns;       ///< Servidor DNS para redirigir a la página de configuración.

/**
 * @brief Solicita credenciales WiFi al usuario mediante el portal cautivo.
 *
 * Esta función se usa dentro del AP cuando el usuario accede a la página
 * de configuración. Devuelve SSID y contraseña por referencia.
 *
 * @param ssid  Referencia donde se almacenará el SSID ingresado.
 * @param pass  Referencia donde se almacenará la contraseña ingresada.
 */
void ask_credentials(String &ssid, String &pass);

/**
 * @brief Sincroniza el tiempo del sistema usando servidores NTP.
 *
 * Debe ejecutarse antes de establecer conexiones TLS o enviar telemetría con timestamp.
 */
void syncTime();

/**
 * @brief Intenta reconectarse al broker MQTT.
 *
 * Reutiliza certificados ya configurados y reintenta conexión automática.
 *
 * @return true si la reconexión fue exitosa, false en caso contrario.
 */
bool reconnect();

/**
 * @brief Inicializa la comunicación segura (TLS + MQTT) y sincroniza el tiempo.
 *
 * Configura certificados, sincroniza el tiempo, configura el cliente MQTT
 * y llama a reconnect() para establecer la conexión.
 */
void init_communications();

/**
 * @brief Convierte la telemetría estructurada en un mensaje JSON.
 *
 * Genera un string JSON con todos los campos necesarios para enviar al servidor.
 *
 * @param data Estructura de telemetría (GPS, IMU, ángulos, metadatos).
 * @return Cadena JSON lista para enviar.
 */
String msgToJson(const Telemetry_t &data);

/**
 * @brief Envía un mensaje de telemetría al servidor o broker configurado.
 *
 * Puede usar MQTT o HTTPS según la configuración.
 *
 * @param data Cadena JSON generada por msgToJson().
 * @return true si el envío fue exitoso, false en caso de error.
 */
bool sendData(const String &data);

/**
 * @brief Genera la página HTML principal del portal cautivo.
 *
 * Contiene formulario para que el usuario ingrese nuevo SSID y contraseña.
 *
 * @param currentSSID SSID actual guardado.
 * @param currentName Nombre del dispositivo o usuario.
 * @return Página HTML lista para servir.
 */
String htmlPage(const String& currentSSID, const String& currentName);

/**
 * @brief Configura las rutas y redirecciones del portal cautivo.
 *
 * Se encarga de capturar cualquier URL y redirigirla a la página de credenciales.
 */
void setupCaptiveRedirects();

/**
 * @brief Handler para guardar credenciales enviadas por el formulario.
 *
 * Guarda SSID y contraseña en NVS mediante Preferences.
 */
void handleSanve();

/**
 * @brief Inicia el Access Point del dispositivo con portal cautivo.
 *
 * Crea red WiFi, inicia DNS, servidor web y activa las redirecciones.
 */
void startAP();

/**
 * @brief Intenta conectar al dispositivo a una red WiFi usando credenciales guardadas.
 *
 * Si falla, se sugiere iniciar el AP y pedir nuevas credenciales.
 *
 * @return true si se conectó correctamente, false si no.
 */
bool connectToWiFi();

#endif // WIFI_FUNCS_HPP
