---
tags:
  - propuesta
  - IoT
  - Proyecto_final
date: 22/08/2025
Integrantes: Julian Sanchez, Danilo, César Augusto Montoya Ocampo
---

# Localizador GPS para Vehículos/Bicicletas con ESP32, GPS, IMU, hotspot del celular, LEDs de estado y buzzer por exceso de velocidad

## 1. PROBLEMA Y MOTIVACIÓN

En el ámbito del ciclismo y la movilidad personal es común el uso de aplicaciones como Strava, Komoot o Google Fit, así como ciclocomputadores dedicados de marcas como Garmin o Wahoo, los cuales permiten registrar métricas de movimiento como velocidad, distancia y posición. No obstante, estas soluciones presentan limitaciones relevantes. En el caso de las aplicaciones móviles, muchas operan bajo un esquema freemium, lo que restringe funcionalidades avanzadas a planes de pago, además de depender exclusivamente de los sensores del teléfono celular. Esto último puede convertirse en una desventaja para usuarios con dispositivos de gama baja, en los que la calidad del GPS o la estabilidad de los sensores internos no garantiza mediciones precisas y confiables. Por otro lado, los dispositivos especializados suelen ser costosos y poco flexibles en términos de personalización, lo que reduce su accesibilidad y capacidad de adaptación a necesidades particulares.

Ante este panorama, el presente proyecto propone el desarrollo de un localizador GPS basado en la plataforma ESP32, como alternativa económica, abierta y personalizable. Este dispositivo será capaz de conectarse a internet a través del punto de acceso móvil de un teléfono celular, enviando los datos a una interfaz en la nube para su visualización en tiempo real. De este modo, se busca ofrecer una solución que combine la precisión de un hardware dedicado con la flexibilidad de un sistema abierto, evitando las restricciones impuestas por fabricantes o esquemas de suscripción.

Adicionalmente, como valor agregado, se plantea la posibilidad de implementar un modo offline en el que el dispositivo almacene los datos de telemetría en una tarjeta microSD cuando no exista conectividad. Si bien este modo no incluiría todas las funcionalidades en línea, permitiría conservar un registro histórico que podría ser sincronizado posteriormente con la nube, aumentando así la utilidad del sistema en escenarios con conectividad intermitente o nula.

---

## 2. OBJETIVOS

### Objetivo general

Desarrollar un dispositivo de telemetría basado en ESP32, de bajo costo y alta personalización, que permita capturar, transmitir y visualizar en tiempo real datos de posición y velocidad de bicicletas o vehículos, garantizando seguridad, confiabilidad y disponibilidad de la información, además de ofrecer funciones de alertas y monitoreo accesibles mediante interfaces web o móviles.

### Objetivos específicos

- **Captura de telemetría:** Implementar un sistema de adquisición de datos de posición (latitud, longitud), velocidad y rumbo con frecuencias de muestreo entre 1 y 5 Hz, estructurados en mensajes JSON que incluyan metadatos como estampas de tiempo UNIX, identificadores de secuencia y del dispositivo en cuestión, garantizando precisión y trazabilidad en la recolección de información.
- **Conectividad y transmisión:** Establecer la comunicación del dispositivo a través de WiFi, utilizando el punto de acceso del smartphone como enlace hacia internet, y habilitar el envío de datos a un broker MQTT seguro con TLS y QoS configurado para asegurar la entrega confiable de la información.
- **Visualización de la información:** Diseñar un dashboard interactivo accesible desde el celular o una página web, que permita mostrar en tiempo real la velocidad y la ruta recorrida, así como consultar históricos y eventos registrados (excesos de velocidad, desconexiones, fallos de GPS), integrando Grafana como herramienta de análisis y visualización histórica.
- **Sistemas de alertas:** Incorporar un mecanismo de aviso mediante buzzer, capaz de indicar al usuario cuando la velocidad supera un umbral configurable desde la aplicación móvil o web.
- **Indicadores de estado:** Integrar un sistema de LEDs que muestre, mediante diferentes colores, el estado de la conectividad WiFi y la adquisición de datos GPS, actuando como un mecanismo de diagnóstico local cuando no se dispone de la aplicación móvil.
- **Gestión de energía:** Diseñar una solución energética que garantice una autonomía de 10 a 24 horas en modo bicicleta, mediante una batería de polímero de litio con recarga a través de un módulo USB-C, evaluando además modos de bajo consumo (deep sleep) para optimizar la autonomía en distintos escenarios de uso.

---

## 3. ALCANCES

- **Documentación técnica:** Incluirá esquemas lógicos, pinout de conexiones, diagramas de ensamblaje, guías de uso y pruebas, así como la descripción detallada de la arquitectura de software y protocolos de comunicación definidos (estructura de mensajes en JSON).
- **Hardware:** Desarrollo de un prototipo funcional montado en placa perforada o PCB simple, con integración de módulos GPS, IMU, LEDs indicadores, buzzer como actuador de alertas de exceso de velocidad, y batería recargable.
- **Firmware:** Implementación en ESP-IDF/Arduino (C/C++), con cliente WiFi integrado y drivers para la gestión de periféricos. El diseño permitirá seleccionar entre una arquitectura basada en superLoop (baremetal) o el uso de FreeRTOS, según los requerimientos de concurrencia.
- **Conectividad y backend:** Comunicación con un broker MQTT seguro (TLS) para la transmisión de datos en tiempo real. Almacenamiento de series de tiempo y visualización histórica mediante Grafana.
- **Estructura física:** Empaquetado del prototipo en una carcasa diseñada e impresa en 3D para facilitar su portabilidad y resistencia básica en campo.
- **Interfaz de usuario:** Desarrollo de un MVP de aplicación web o móvil que permita visualizar velocidad, trayectorias y eventos relevantes de manera interactiva. Desde esta interfaz se podrán configurar parámetros como el umbral de velocidad para las alertas.

- **OPCIONAL:** Implementación de un modo offline que permita almacenar los datos en una tarjeta microSD cuando no exista conectividad. Esta funcionalidad no contemplará todas las características del sistema en línea, pero preservará los registros históricos para su posterior sincronización con la nube.

---

## 4. REQUISITOS FUNCIONALES

1. **Adquisición de datos**
   - El sistema debe obtener de forma continua la posición geográfica (latitud y longitud), la velocidad y el ángulo de inclinación del dispositivo a partir del GPS e IMU integrados.
   - La frecuencia de adquisición debe estar configurada en el rango de **1 a 5 Hz**, según el balance entre precisión y consumo energético.
2. **Conectividad WiFi**
   - El sistema debe poder conectarse en modo estación (STA) al **hotspot de un smartphone** mediante credenciales SSID/PASSWORD configurables por el usuario.
   - En caso de pérdida de conexión, el sistema debe ejecutar un procedimiento automático de reconexión con intentos periódicos hasta restablecer la comunicación.
3. **Visualización de información**
   - El sistema debe mostrar un **panel numérico** con velocidad instantánea, velocidad promedio y estadísticas básicas del trayecto (distancia recorrida, tiempo activo).
4. **Alertas de seguridad**
   - El sistema debe activar un **buzzer de alerta** cuando la velocidad instantánea supere un umbral de seguridad definido por defecto o configurado por el usuario.
   - La activación del buzzer debe ocurrir únicamente cuando el exceso de velocidad se mantenga durante un intervalo configurable de **T segundos**.
5. **Indicadores de estado**
   - El sistema debe mostrar el estado de conexión y del GPS mediante **LEDs indicadores** de distintos colores:
     - **LED Internet (azul):** fijo = online, parpadeo = sin Internet.
6. **Gestión de energía**
   - El sistema debe incorporar una **batería recargable** con un **módulo de carga USB-C** para facilitar su recarga.
   - OPCIONAL: El sistema debe soportar modos de **deep sleep por inactividad**, detectada a través de la IMU, con reactivación automática al retomar el movimiento.

---

## 5. Arquitectura del proyecto

Buscando cumplir con los estándares de calidad de sistemas de información definidos por las normas **ISO 25010**, se desarrollarán diferentes estructuras que sean desacopladas y modulares en las etapas que constituyen el proyecto.

### 5.1. Modelo de Dominio del sistema

Desde una perspectiva general del sistema, se busca que el usuario final pueda interactuar con el sistema mediante una aplicación de software, tal que se cumpla un modelo como el siguiente:
![[Diagrama del modelo de dominio.png]]

### 5.2. Mockup de la aplicación

Para el desarrollo de la aplicación se realizó el siguiente mockup, que determina el concepto general diseñado para la interfaz de usuario.

![[App mockup.png]]

### 5.3. Patrón de desarrollo

El patrón de desarrollo que se utilizará para la comunicación de los datos corresponde al modelo **Publicador–Subscriptor**, lo cual facilita el intercambio asincrónico de los datos y desacopla la generación de la información respecto a su consumo.

![[PubSubImage.png]]

### 5.4. Arquitectura en capas IoT

La arquitectura del sistema se organiza siguiendo el enfoque clásico de tres capas en sistemas IoT:

1. **Capa de Percepción:**
   En esta capa se concentran los dispositivos encargados de sensar, recolectar datos y activar respuestas inmediatas. El sistema utilizará:

   - **Módulo GPS** para la adquisición de latitud, longitud y velocidad.
   - **IMU (Inertial Measurement Unit)** para la medición del ángulo de inclinación y detección de movimiento (también usada para activar el sistema desde modos de bajo consumo).
   - **Microcontrolador ESP32** como unidad de procesamiento local, encargado de la adquisición, preprocesamiento básico y gestión de energía.
   - **Buzzer** como actuador de alerta, que se activa automáticamente en caso de detección de exceso de velocidad o inclinación peligrosa.
   - **MicroSD (modo opcional)** para el almacenamiento offline de los datos históricos.

2. **Capa de Red:**
   En esta capa se gestiona la transmisión de la información recolectada hacia la nube o hacia otros nodos de la red. El sistema utilizará:

   - **WiFi en modo estación (STA)** para conectividad a través del hotspot de un smartphone.
   - **Broker MQTT** como intermediario en la comunicación, aplicando el patrón **publicador-suscriptor** para desacoplar la adquisición de datos y la visualización.
   - Protocolos de reconexión automática para mitigar pérdidas de conectividad.

   Como ejemplo, el siguiente mensaje JSON representa la información que el ESP32 publicará en el broker MQTT:

   ```json
   {
     "device_id": "esp32_069",
     "timestamp": 1725033930123,
     "sequence_id": 1542,
     "mode": "online",
     "gps": {
       "latitude": 6.2442,
       "longitude": -75.5812,
       "altitude_m": 1495.0,
       "speed_mps": 12.4,
       "fix": "3D",
       "sats": 8,
       "hdop": 0.9
     },
     "imu": {
       "accel_x_mps2": 0.01,
       "accel_y_mps2": 0.12,
       "accel_z_mps2": 9.8,
       "gyro_x_dps": -0.03,
       "gyro_y_dps": 0.04,
       "gyro_z_dps": 0.01,
       "sample_rate_hz": 100
     },
     "angles": {
       "roll_deg": 2.5,
       "pitch_deg": -1.2
     },
     "alerts": {
       "overspeed": false,
       "tilt_warning": false,
     },
     "system": {
       "wifi_status": "connected",
       "battery_level_pct": 78,
       "storage_status": "active"
     }
   }
   ```

3. **Capa de Aplicación:**
   En esta capa se concentran los servicios y la interacción con el usuario final. El sistema contará con:
   - **Servidor en la nube** que almacena los datos recibidos vía MQTT.
   - **Interfaz web** para la visualización de mapas, estadísticas de velocidad, inclinación y alertas.
   - **Módulos de visualización interactiva**, incluyendo un mapa con trayecto histórico y un panel numérico con indicadores en tiempo real.
   - **Sistema de alertas al usuario**, con avisos visuales en la app y sonoros mediante el buzzer.

### 5.5. Interacción máquina a máquina (M2M)

La solución contempla además escenarios de comunicación directa entre máquinas, sin intervención constante del usuario. Por ejemplo:
- El **microcontrolador (ESP32)** comunica periódicamente sus datos al **broker MQTT** sin requerir solicitud directa del usuario.
- El **broker MQTT** reenvía automáticamente la información a múltiples suscriptores (por ejemplo, la aplicación web y otros sistemas de análisis).
- En modo offline, el **ESP32 y la microSD** interactúan de forma autónoma para almacenar datos que luego serán transmitidos de manera automática al servidor en cuanto se restablezca la conectividad.
- De manera inmediata y autónoma, el **ESP32 activa el buzzer** cuando detecta condiciones críticas (exceso de velocidad o inclinación), incluso si no existe conectividad a red.

---

## 6. CRONOGRAMA DE INTEGRACIÓN DEL PROYECTO

### Fase 1 – Preparación y diseño (Semanas 1–3)

- **Semana 1 (22–28 ago):**
  - [x] Definir requisitos detallados del sistema (entradas, salidas, alcance).
  - [ ] Establecer diagrama de bloques y arquitectura (hardware + software).
  - [x] Creación del mockup de la aplicación.
- **Semana 2 (29 ago–4 sep):**
  - [ ] Selección y compra de componentes (ESP32, GPS, IMU, LEDs, buzzer, batería, cargador USB-C).
  - [x] Investigación de librerías disponibles (ESP-IDF / Arduino para GPS, Wi-Fi, IMU).
  - [ ] Análisis de requerimientos de red (capacidad de transmisión, retardo tolerable, ancho de banda estimado por los datos de sensores).
  - [ ] Definición de la arquitectura y los patrones de diseño de la aplicación.
- **Semana 3 (5–11 sep):**
  - [ ] Diseño preliminar de firmware (tareas FreeRTOS, drivers básicos).
  - [ ] Montaje inicial de módulos en protoboard.
  - [ ] Diseño preliminar de la arquitectura de comunicación (modelo Pub/Sub, decisión sobre QoS MQTT, seguridad en la transmisión).
  - [ ] Desarrollo de la aplicación.

---

### Fase 2 – Integración de hardware y drivers (Semanas 4–6)

- **Semana 4 (12–18 sep):**
  - [ ] Integración ESP32 ↔ GPS (lectura NMEA, velocidad y coordenadas).
  - [ ] Desarrollo de la aplicación.
- **Semana 5 (19–25 sep):**
  - [x] Integración ESP32 ↔ IMU (lectura básica de acelerómetro/giroscopio).
  - [ ] Pruebas de datos combinados GPS + IMU.
  - [ ] Desarrollo de la aplicación.
- **Semana 6 (26 sep–2 oct):**
  - [ ] Integración LEDs (conectividad, estado GPS).
  - [ ] Integración buzzer (alertas PWM).
  - [ ] Desarrollo de la aplicación.

---

### Fase 3 – Conectividad y almacenamiento (Semanas 7–9)

- **Semana 7 (3–9 oct):**
  - [ ] Implementar Wi-Fi STA → conexión a hotspot del celular.
  - [ ] Hacer pruebas de estabilidad de conexión para diversos escenarios de uso.
  - [ ] Rutinas de reconexión automática.
  - [ ] Implementar conexión de la aplicación a la nube.
- **Semana 8 (10–16 oct):**
  - [ ] Diseño y prueba de mensajes Pub/Sub para los clientes MQTT, incluyendo estructura de datos (JSON).
  - [ ] Pruebas de envío de datos a la nube (MQTT/HTTP simple).
  - [ ] Buffer en memoria/flash cuando no haya Internet.
  - [ ] Pruebas de intercambio de datos entre el software y la nube.
- **Semana 9 (17–23 oct):**
  - Pruebas de almacenamiento y reenvío de datos tras reconexión.

---

### Fase 4 – Interfaz y visualización (Semanas 10–12)

- **Semana 10 (24–30 oct):**
  - [ ] Servidor web local en ESP32.
  - [ ] API REST para exponer coordenadas y velocidad.
- **Semana 11 (31 oct–6 nov):**
  - [ ] Integración de mapa (Leaflet) en la interfaz web.
  - [ ] Visualización de posición en tiempo real.
  - [ ] Pruebas de recolección de datos enviados por sensores en la aplicación.
- **Semana 12 (7–13 nov):**
  - [ ] Panel de control web (estado Wi-Fi, umbral de velocidad, batería).
  - [ ] Pruebas con datos en vivo.

---

### Fase 5 – Integración total y pruebas (Semanas 13–15)

- **Semana 13 (14–20 nov):**
  - [ ] Integración completa del sistema (GPS + IMU + LEDs + buzzer + Wi-Fi + interfaz).
  - [ ] Ajustes de consumo y autonomía con batería.
- **Semana 14 (21–27 nov):**
  - [ ] Pruebas de campo (bicicleta/vehículo).
  - [ ] Verificación de alertas por exceso de velocidad.
  - [ ] Pruebas de robustez de la comunicación inalámbrica en campo (zonas con baja cobertura Wi-Fi / hotspot, evaluar tolerancia a fallos).
- **Semana 15 (28 nov–4 dic):**
  - [ ] Corrección de errores y mejoras (reconexión Wi-Fi, precisión GPS).

---

### Fase 6 – Entrega final (Semana 16)

- **Semana 16 (5–11 dic):**
  - [ ] Documentación técnica (manual de usuario + guía de integración).
  - [ ] Preparación de presentación final y video demo.
- **13 de diciembre:** **Entrega final del proyecto.**

# 7. LISTA DE MATERIALES Y PRESUPUESTO ESTIMADO.

- **ESP32‑WROOM‑32:** $ 39.000 [ESP32](https://www.mactronica.com.co/tarjeta-de-desarrollo-esp32-s3-n16r8)
  - [x] comprado
- **Módulo Gps Gy Neo6mv2 Eeprom U-blox con antena**: $ 38.000 [GPS](https://articulo.mercadolibre.com.co/MCO-834505267-modulo-gps-gy-neo6mv2-eeprom-u-blox-_JM?matt_tool=19390127)
  - [ ] comprado
- **MPU6050 9 Axis:** $ 11.900 [IMU](https://electronilab.co/tienda/mpu6050-acelerometro-y-giroscopio-i2c/)
  - [x] comprado
- **Cargador USB‑C (TP4056/):** $ 5.000 [CARGADOR](https://electronilab.co/tienda/modulo-cargador-bateria-de-lipo-1a-usb-c-5v-tp4056/)
  - [ ] comprado
- **Batería litio polímero 2000 mAh 3.7V BATLIPO20001S:** $ 23.978,50 [Bateria](https://www.didacticaselectronicas.com/shop/585460-3-7-2000ma-bateria-litio-polimero-2000-mah-3-7v-batlipo20001s-1934#attr=6020,6021,6022,6023,6024,6025,6026)
  - [ ] comprado
- **Misceláneos (LEDs, buzzer, PCB, conectores, caja):** $ 20.000
  - [ ] comprado

# **Total estimado:** $ 138.000
