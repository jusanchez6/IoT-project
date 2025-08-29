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

En este contexto este proyecto apunta a la creación de un localizador GPS para bicicleta y/o vehículos basados en el ESP32 personalizables y barato en comparativa con los demás dispositivos presentes en el mercado. Este dispositivo se conecta a la red mediante hotspot de celular, mostrando datos en una interfaz alojada en la nube. Además de la funcionalidad de alertas por exceso de velocidad segura (definida por el usuario -estática- o por la localización del mismo -dinámica-). El equipo pretende incluir LEDs de estado (Conectividad y GPS) y alimentación por batería recargable con modula de carga USB-C, y configuración por este mismo al conectarse al teléfono mediante una aplicación.

---
## 2. OBJETIVOS

**1. Telemetria:** Capturar y almacenar posición (Latitud y Longitud), velocidad y rumbo de 1-5Hz

**2. Conectividad:** Sincronizar datos via WiFi AC conectándose al hotspot del smartphone 

**3. Visualización:** 
- Dashboard interactivo en el celular o pagina web con datos de velocidad por trayecto e historial de ubicaciones.
- Envío a un broker mediante MQTT por túnel TLS para consultas remotas.
- Registro de eventos en el celular o pagina web (excesos de velocidad, fallos de conectividad, etc.)

**4. Seguridad y sistemas de alertas:** Mediante un buzzer indicando cuando la velocidad supera el limite estimado para la via o por un valor fijado por el usuario desde la configuración por la aplicación móvil.

**5. Estados visibles por medio de LEDs:** LEDs de distintos colores indicando si el GPS se encuentra obteniendo datos, si el dispositivo esta conectado a una red WiFi valida.

**6. Energía:** Autonomía objetivo de 10-24 h (modo bici) con batería de polímero de Litio y carga mediante modulo USB-C.

---
## 3. ALCANCES

- Hardware prototipo funcional en placa perforada o PCB simple.
- Empaquetado en una estructura impresa en 3D.
- Firmware escrito en ESP-IDF/ARDUINO (C/C++) con cliente WiFi y drivers (GPS/IMU/LEDs/BUZZER)
- Capacidad de elección entre código enfocado un superLoop (Baremetal) o RTOS (FreeRTOS)
- Backend con MQTT + Grafana para visualización histórica
- MVP del Software de usuario (aplicación web o aplicación móvil).
- Documentación de ensamblaje, pin-out, esquemas lógicos, guías de uso y pruebas.
- Documentación de la estructura y arquitectura de las aplicaciones web o aplicación móvil desarrollada.
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

Buscando cumplir con los estándares de calidad de sistemas de información definidos por las normas ISO 25010, se desarrollarán diferentes estructuras que sean desacopladas y modulares en las etapas que constituyen el proyecto.
### 5.1. Modelo de Domino del sistema
Desde una perspectiva general del sistema, se busca que el usuario final pueda interactuar con el sistema mediante una aplicación de software, tal que se cumpla un modelo como el siguiente:
![[Diagrama del modelo de dominio.png]]

### 5.2. Mockup de la aplicación
Para el desarrollo de la aplicación se realizó el siguiente mockup, que determina el concepto general diseñado para la interfaz de usuario.

![[App mockup.png]]

### 5.3. Patrón de desarrollo 
El patrón de desarrollo que se utilizará para la comunicación de los datos corresponde al modelo Publicador-Subscriptor, tal que se facilitará el intercambio asincrónico de los datos.

![[PubSubImage.png]]


---
## 6. CRONOGRAMA DE INTEGRACIÓN DEL PROYECTO

### Fase 1 – Preparación y diseño (Semanas 1–3)

- **Semana 1 (22–28 ago):**
    - [x] Definir requisitos detallados del sistema (entradas, salidas, alcance).
    - [ ] Establecer diagrama de bloques y arquitectura (hardware + software).
    - [x]  Creación del mockup de la aplicación.
- **Semana 2 (29 ago–4 sep):**
    - [ ] Selección y compra de componentes (ESP32, GPS, IMU, LEDs, buzzer, batería, cargador USB-C).
    - [x] Investigación de librerías disponibles (ESP-IDF / Arduino para GPS, Wi-Fi, IMU).
    - [ ] Definición de la arquitectura y los patrones de diseño de la aplicación.
- **Semana 3 (5–11 sep):**
    - [ ] Diseño preliminar de firmware (tareas FreeRTOS, drivers básicos).
    - [ ] Montaje inicial de módulos en protoboard.
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
	
**Total estimado:** $ 138.000
=======
