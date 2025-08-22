---
tags:
  - propuesta
  - IoT
  - Proyecto_final
date: 22/08/2025
Integrantes: Julian Sanchez, Danilo, César
---
# Localizador GPS para Vehículos/Bicicletas con ESP32, GPS, IMU, hotspot del celular, LEDs de estado y buzzer por exceso de velocidad

## 1. MOTIVACIÓN

Actualmente en el mundo ciclista se usan una cantidad sin fin de dispositivos para realizar métricas de movimiento (Velocidades y coordenadas) sin embargo estos dispositivos cuentan con limitaciones, en ocasiones impuestas por el mismo fabricante, que hace que algunos dispositivos sean muy básicos, poco personalizables y en muchas ocasiones costosos. 

En este contexto este proyecto apunta a la creación de un localizador GPS para bicicleta y/o vehículos basados en el ESP32 personalizables y barato en comparativa con los demás dispositivos presentes en el mercado. Este dispositivo se conecta a la red mediante hotspot de celular, mostrando datos en una interfaz alojada en la nube. Además de la funcionalidad de alertas por exceso de velocidad segura (definida por el usuario -estática- o por la localización del mismo -dinámica-). El equipo pretende incluir LEDs de estado (Conectividad y GPS) y alimentación por batería recargable con modula de carga USB-C.

---
## 2. OBJETIVOS

**1. Telemetria:** Capturar y almacenar posición (Latitud y Longitud), velocidad y rumbo de 1-5Hz

**2. Conectividad:** Sincronizar datos via WiFi AC conectándose al hotspot del smartphone 

**3. Visualización:** 
- Dashboard interactivo en el celular o pagina web con datos de velocidad por trayecto e historial de ubicaciones.
- Envío a un broker mediante MQTT por túnel TLS para consultas remotas.

**4. Seguridad y sistemas de alertas:** Mediante un buzzer indicando cuando la velocidad supera el limite estimado para la via o por un valor fijado por el usuario de moto estático.

**5. Estados visibles por medio de LEDs:** LEDs de distintos colores indicando si el GPS se encuentra obteniendo datos, si el dispositivo esta conectado a una red WiFi valida.

**6. Energía:** Autonomía objetivo de 10-24 h (modo bici) con batería de polímero de Litio y carga mediante modulo USB-C

---
## 3. ALCANCES

- Hardware prototipo funcional en placa perforada o PCB simple.
- Empaquetado en una estructura impresa en 3D.
- Firmware escrito en ESP-IDF/ARDUINO (C/C++) con cliente WiFi y drivers (GPS/IMU/LEDs/BUZZER)
- Backend con MQTT + Grafana para visualización histórica
- Documentación de ensamblaje, pin-out, esquemas lógicos, guías de uso y pruebas.
---
## 4. REQUISITOS FUNCIONALES

1. **Adquisición:** El sistema debe estar en la capacidad de obtener datos de longitud, latitud, velocidad y angulo de inclinación

2. **Conectividad:** El sistema debe conectarse a WiFi STA a SSID/PASSWORD del hotspot de un smartphone y reconectarse en caso de perder la conexión.

3. **Visualización:** El sistema debe crear un mapa con el historial reciente (Últimos 1-5 Km) y panel numérico con las velocidades y otras estadísticas.

4. **Alertas:** El sistema debe crear una alerta mediante un buzzer cuando la velocidad instantánea sea mayor a la velocidad segura durante cierta cantidad T de segundos.

5. **Estado:** El sistema debe crear alertas de conectividad mediante el uso de diferentes leds: LED Internet (azul): fijo=online, parpadeo=sin Internet; LED GPS (verde): fijo=fix 3D, parpadeo=lento=fix 2D, Apagado=sin fix.

6. **Gestión energía**: El sistema debe contar con una bateria y un modulo de carga, además de contar con modos de deep sleep por inactividad (sin movimiento) con wake por IMU.

---
## 5. ARQUITECTURA DEL PROYECTO:


---
## 6. CRONOGRAMA DE INTEGRACIÓN DEL PROYECTO

### Fase 1 – Preparación y diseño (Semanas 1–3)

- **Semana 1 (22–28 ago):**
    - Definir requisitos detallados del sistema (entradas, salidas, alcance).
    - Establecer diagrama de bloques y arquitectura (hardware + software).
- **Semana 2 (29 ago–4 sep):**
    - Selección y compra de componentes (ESP32, GPS, IMU, LEDs, buzzer, batería, cargador USB-C).
    - Investigación de librerías disponibles (ESP-IDF / Arduino para GPS, Wi-Fi, IMU).
- **Semana 3 (5–11 sep):**
    - Diseño preliminar de firmware (tareas FreeRTOS, drivers básicos).
    - Montaje inicial de módulos en protoboard.

---
### Fase 2 – Integración de hardware y drivers (Semanas 4–6)

- **Semana 4 (12–18 sep):**    
    - Integración ESP32 ↔ GPS (lectura NMEA, velocidad y coordenadas).
- **Semana 5 (19–25 sep):**
    - Integración ESP32 ↔ IMU (lectura básica de acelerómetro/giroscopio).
    - Pruebas de datos combinados GPS + IMU.
- **Semana 6 (26 sep–2 oct):**
    - Integración LEDs (conectividad, estado GPS).
    - Integración buzzer (alertas PWM).

---
### Fase 3 – Conectividad y almacenamiento (Semanas 7–9)

- **Semana 7 (3–9 oct):**
    - Implementar Wi-Fi STA → conexión a hotspot del celular.
    - Rutinas de reconexión automática.
- **Semana 8 (10–16 oct):**
    - Pruebas de envío de datos a la nube (MQTT/HTTP simple).
    - Buffer en memoria/flash cuando no haya Internet.
- **Semana 9 (17–23 oct):**
    - Pruebas de almacenamiento y reenvío de datos tras reconexión.

---
### Fase 4 – Interfaz y visualización (Semanas 10–12)

- **Semana 10 (24–30 oct):**
    - Servidor web local en ESP32.
    - API REST para exponer coordenadas y velocidad.
- **Semana 11 (31 oct–6 nov):**
    - Integración de mapa (Leaflet) en la interfaz web.
    - Visualización de posición en tiempo real.
- **Semana 12 (7–13 nov):**
    - Panel de control web (estado Wi-Fi, umbral de velocidad, batería).
    - Pruebas con datos en vivo.

---
### Fase 5 – Integración total y pruebas (Semanas 13–15)

- **Semana 13 (14–20 nov):**
    - Integración completa del sistema (GPS + IMU + LEDs + buzzer + Wi-Fi + interfaz).
    - Ajustes de consumo y autonomía con batería.
- **Semana 14 (21–27 nov):**
    - Pruebas de campo (bicicleta/vehículo).
    - Verificación de alertas por exceso de velocidad.
- **Semana 15 (28 nov–4 dic):**
    - Corrección de errores y mejoras (reconexión Wi-Fi, precisión GPS).

---
### Fase 6 – Entrega final (Semana 16)

- **Semana 16 (5–11 dic):**    
    - Documentación técnica (manual de usuario + guía de integración).
    - Preparación de presentación final y video demo.
- **13 de diciembre:** **Entrega final del proyecto.**
