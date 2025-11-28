| Supported Targets | ESP32-S3 |
| ----------------- | -------- |
# Firmware BikeTracker — ESP32-S3

## Introducción

El Firmware BikeTracker es un sistema embebido desarrollado sobre un ESP32-S3 DevKit, cuyo propósito es capturar información del entorno mediante sensores GNSS e IMU, procesarla localmente y transmitirla hacia la nube mediante WiFi y MQTT. El proyecto integra tareas concurrentes con FreeRTOS, un portal cautivo para configuración de red, retroalimentación visual con LED RGB, alarmas acústicas y un pipeline completo de generación de telemetría.

Este firmware corresponde al segundo laboratorio de la asignatura de IoT, y busca que el estudiante entienda cómo la capa de percepción interactúa con la capa de red en una arquitectura IoT real.

## Características Principales

- **Adquisición GNSS (GPS):**
  - Latitud, longitud, altitud, velocidad y hora local
  - Indicación de fix y disponibilidad de señal

- **Adquisición IMU (MPU6050):**
  - Aceleración lineal, velocidad angular
  - Cálculo de vibraciones y ángulos (roll y pitch)

- **Comunicación IoT:**
  - Conexión a WiFi en modo estación
  - Portal cautivo para ingresar credenciales
  - Comunicación segura TLS (CA, certificado cliente, llave privada)
  - Cliente MQTT para envío periódico de telemetría

- **FreeRTOS:**
  - Tareas independientes para GNSS, IMU, LED, alarma, envío de datos y monitoreo WiFi
  - Mutex para proteger datos compartidos

- **Actuadores:**
  - LED RGB (indicador de estados WiFi/GNSS/Broker)
  - Alarma sonora por vibraciones o velocidad umbral

## Arquitectura del Sistema

El firmware se organiza en módulos independientes:

- `gnss.hpp` - lectura y decodificación NMEA
- `imu.hpp` - lectura de acelerómetros y giróscopos
- `platform.hpp` - LED RGB y alarma
- `wifi_funcs.hpp` - WiFi & MQTT: conexión, TLS, portal cautivo y envío de datos
- `types.hpp` - Tipos de Datos: estructuras internas de telemetría
- `main.cpp` - punto de entrada y configuración de tareas FreeRTOS

Cada tarea FreeRTOS opera de manera concurrente y accede a datos protegidos mediante un mutex global.

```
┌───────────────────┐         ┌─────────────────────┐
│     taskSensors   │───────▶│      Telemetry_t    │
└───────────────────┘         │   (Data + Mutex)    │
      ▲     ▲                 └─────────────────────┘
      │     │                           ▲
  GNSS/IMU  │                           │
      │     │                           │
      │     │                           │
      ▼     ▼                           ▼
┌────────────────┐          ┌───────────────────────┐
│   taskAlarm    │          │      taskSend         │
└────────────────┘          │   MQTT + mTLS + JSON  │
                            └───────────────────────┘
```

## Objetivos de Aprendizaje

- Comprender la capa de percepción en una arquitectura IoT
- Integrar sensores GNSS e IMU con un ESP32-S3
- Utilizar FreeRTOS para gestionar múltiples tareas concurrentes
- Implementar retroalimentación mediante LED y buzzer
- Establecer comunicaciones seguras (TLS + MQTT)
- Crear una interfaz de configuración mediante portal cautivo

## Tareas Principales (FreeRTOS)

- **taskSensors:** Adquiere datos de GNSS e IMU y actualiza la estructura de telemetría
- **taskAlarm:** Activa o desactiva la alarma según velocidad y vibraciones
- **taskLED:** Indicador de estado basado en: WiFi → GNSS → Broker MQTT
- **taskSend:** Convierte la telemetría a JSON y la envía por MQTT
- **taskWiFiPortal:** Atiende el servidor web y el DNS del portal cautivo
- **taskWifiMonitor:** Supervisa el estado de conexión a WiFi

## Formato de Telemetría

La estructura principal utilizada para el envío de datos es `Telemetry_t` en `types.hpp`

Esta incluye:

- Identificación del dispositivo
- Timestamp y secuencia
- Datos GNSS (posición, fix, velocidad, hora)
- Datos IMU (aceleración, giroscopio, vibración)
- Ángulos roll y pitch

## Flujo de Inicio

1. Inicialización de GNSS, IMU, LED y buzzer
2. Creación de mutex
3. Conexión a WiFi:
   - Si falla, inicia AP + portal cautivo
4. Si WiFi OK → inicializa TLS + MQTT
5. Lanzamiento de las tareas FreeRTOS

## Autor

- **Autor:** Julian Mauricio Sánchez Ceballos
- **Asignatura:** IoT
- **Fecha:** 15/09/2025
- **Versión del Firmware:** 3.0

## Notas Finales

- El proyecto se puede compilar mediante PlatformIO (Arduino Core ESP32-S3)
- El código es de propósito académico y está diseñado para demostrar un pipeline completo de percepción y comunicación IoT
- El archivo principal es: `main.cpp`