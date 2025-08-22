
# Propuesta de Proyecto

## Localizador GPS para Vehículos/Bicicletas con ESP32, GPS, IMU, hotspot del celular, LEDs de estado y buzzer por exceso de velocidad

---

### 1) Resumen ejecutivo

Desarrollar un dispositivo compacto basado en **ESP32** que registre **coordenadas GPS** y **velocidad**, se conecte a Internet mediante el **hotspot del celular**, muestre los datos en una **interfaz web** local y/o en la nube, y emita alertas por **buzzer** cuando la velocidad exceda la velocidad segura (límite de vía obtenido de mapas). El equipo incluirá **LEDs de estado** (conectividad e GPS) y alimentación por **batería recargable** con **módulo de carga USB‑C**.

---

### 2) Objetivos

- **O1. Telemetría**: Capturar y almacenar posición (lat, lon), velocidad y rumbo a 1–5 Hz.
    
- **O2. Conectividad**: Sincronizar datos vía **Wi‑Fi STA** conectándose al **hotspot del smartphone** (portal de configuración Wi‑Fi y fallback a AP propio).
    
- **O3. Visualización**:
    
    - **Dashboard web** servido por el ESP32 (cuando no hay Internet) con mapa y datos en vivo.
        
    - **Opción nube**: envío a un broker **MQTT/HTTP** para consulta remota.
        
- **O4. Seguridad vial**: **Buzzer** cuando la velocidad medida supere el **límite de velocidad** estimado/obtenido para la vía.
    
- **O5. Estados visibles**: **LED 1**: Internet OK / fallo. **LED 2**: GPS fix 3D / buscando.
    
- **O6. Energía**: Autonomía objetivo 10–24 h (modo bici) con **Li‑ion 18650** o pouch y **carga por USB‑C**.
    

---

### 3) Alcance

- Hardware prototipo funcional en placa perforada o PCB simple.
    
- Firmware ESP-IDF (C/C++) con tareas FreeRTOS, servidor web, cliente Wi‑Fi y drivers (GPS/IMU/LEDs/buzzer).
    
- Interfaz web ligera (HTML/JS) con mapa embebido (Leaflet) y datos en vivo vía WebSocket o EventSource.
    
- Opción de backend ligero (Docker opcional) con **MQTT + InfluxDB + Grafana** o **HTTP + SQLite** para histórico.
    
- Documentación de ensamblaje, pinout, esquemas lógicos, guía de uso, y pruebas.
    

> Fuera de alcance (MVP): app móvil nativa; carcasa IP67 personalizada; certificaciones.

---

### 4) Requerimientos funcionales

1. **Adquisición**: lat/lon/alt, velocidad, rumbo; aceleraciones de IMU para detección de movimiento/caídas.
    
2. **Registro**: buffer circular en flash (NVS/partición dedicada) cuando no hay Internet; envío batch al reconectar.
    
3. **Conectividad**: Wi‑Fi STA a SSID/clave del hotspot; prueba de conectividad (DNS/ping) y reconexión.
    
4. **Visualización**: mapa centrado en posición actual, historial reciente (últimos 1–5 km), y panel numérico.
    
5. **Alertas**: buzzer activo cuando `v_instantánea > v_segura` (límite de vía o umbral usuario) por más de T segundos.
    
6. **Estados**: LED Internet (azul): fijo=online, parpadeo=sin Internet; LED GPS (verde): fijo=fix 3D, parpadeo=lento=fix 2D, rápido=sin fix.
    
7. **Gestión energía**: deep sleep por inactividad (sin movimiento) con wake por IMU.
    

---

### 5) Arquitectura del sistema

```
[Satélites GNSS] → [Módulo GPS] ┐
                               ├→ ESP32 (FreeRTOS)
[IMU 6/9 ejes] → [I2C/SPI]     │     ├→ Wi‑Fi STA → Hotspot teléfono → (MQTT/HTTP opcional)
                               │     ├→ Servidor Web local (Leaflet + API REST)
[Batería Li‑ion] → [PMIC/Charger USB‑C]     ├→ LED Internet / LED GPS
                                            └→ Buzzer
```

---

### 6) Diseño de Hardware (BoM sugerida)

- **MCU/Comms**: ESP32‑WROOM‑32 o ESP32‑S3 (Wi‑Fi 2.4 GHz, suficiente RAM/flash).
    
- **GNSS**: u‑blox NEO‑6M/7/8, u‑blox M8N, o **L76K** (GPS/GLONASS/BeiDou). Antena patch + LNA.
    
- **IMU**: MPU‑6050/9250 o ICM‑20948 (acel/gyro; opcional magnetómetro).
    
- **Alimentación**:
    
    - Batería **Li‑ion 18650** (2–3 Ah) o pouch 2000–3000 mAh.
        
    - Cargador **USB‑C**: TP4056 con módulo USB‑C (5V sink) + protección (DW01 + MOSFET) o módulo integrado **IP5306/MH‑CD42** con USB‑C.
        
    - **Step‑up/Step‑down** según sea necesario (ej. MT3608 o MP1584) para 5V si algún módulo lo requiere.
        
- **Indicadores**: 2 × LED (azul/verde) con resistencias (1–2 kΩ).
    
- **Buzzer**: activo 3–5 V o pasivo + PWM del ESP32 (canal LEDC).
    
- **Botón**: multiuso (power/función) + switch hard‑off opcional.
    
- **Conectores**: JST‑PH para batería; USB‑C receptáculo; header para GPS/IMU.
    
- **Caja**: impresa 3D; fijación a bici (brackets) o sujeción oculta en vehículo.
    

> Nota: si el GPS requiere 3.3 V/5 V, ajustar reguladores y niveles lógicos.

---

### 7) Diseño de Firmware (ESP‑IDF)

**Tareas FreeRTOS**

- `task_gps`: parseo NMEA/UBX (UART), cálculo de velocidad y precisión (HDOP), watchdog de fix.
    
- `task_imu`: lectura periódica (I2C), detección de movimiento y caídas (umbral aceleración).
    
- `task_net`: gestión Wi‑Fi STA → hotspot; test de Internet; reconexión exponencial; NTP.
    
- `task_store`: buffer en RAM → flush a flash (cuando sin Internet) y reenvío en reconexión.
    
- `task_web`: servidor HTTP + WebSocket (ESP32); API REST JSON; panel SPA con Leaflet.
    
- `task_alert`: lógica buzzer y LEDs basada en estados y `v_instantánea` vs `v_segura`.
    

**APIs/Drivers**

- UART GNSS, I2C IMU, LEDC PWM buzzer, GPIO LEDs, NVS para credenciales y parámetros.
    

**Actualización OTA (opcional)**

- OTA por Wi‑Fi cuando haya Internet.
    

---

### 8) Fuentes de velocidad segura (límite de vía)

**Estrategias compatibles (seleccionables):**

1. **Umbral usuario** (rápido y offline): valor fijo configurable por interfaz (ej. 25/50 km/h según modo bici/vehículo).
    
2. **OpenStreetMap (OSM)**:
    
    - En el teléfono/servidor: _snap_ a la vía con OSRM/Valhalla y consulta de `maxspeed` (si existe) para las coordenadas actuales.
        
    - En el ESP32: enviar lat/lon al teléfono (web UI) y que el navegador consulte el límite vía servicio REST propio.
        
3. **Proveedor comercial** (alternativa): Mapbox/Here/TomTom Speed Limits (si se requiere cobertura/consistencia empresarial).
    

> El MVP usará **Umbral usuario** y añadirá **OSM** como mejora sin bloquear el proyecto.

---

### 9) Interfaz de Usuario (Web local)

- Página servida por el ESP32 (HTML/JS)
    
    - Mapa Leaflet con marcador en tiempo real.
        
    - Panel con: velocidad actual, altitud, rumbo, precisión, estado GPS/Wi‑Fi/batería.
        
    - Controles: seleccionar modo (bici/vehículo), umbral velocidad, tonos de buzzer, SSID/clave hotspot.
        
    - Streaming de datos via **WebSocket** (JSON cada 200–1000 ms).
        

**Ejemplo de payload (JSON):**

```json
{
  "t": 1724301123,
  "lat": 4.7110,
  "lon": -74.0721,
  "speed_kmh": 28.4,
  "course": 123.0,
  "hdop": 0.9,
  "gps_fix": 3,
  "wifi_online": true,
  "v_limit": 30,
  "alert": false
}
```

---

### 10) Lógica de alertas y LEDs

- **Buzzer**: activar tono PWM cuando `speed_kmh > v_limit` durante **T = 2 s** continuos (histeresis de ±2 km/h para evitar _chatter_).
    
- **LED Internet (azul)**:
    
    - **Fijo**: Internet OK (DNS + HTTP 204)
        
    - **Parpadeo 1 Hz**: conectado a Wi‑Fi sin Internet
        
    - **Apagado**: sin Wi‑Fi
        
- **LED GPS (verde)**:
    
    - **Fijo**: fix 3D (>=4 satélites / HDOP < 1.5)
        
    - **Parpadeo lento**: fix 2D
        
    - **Parpadeo rápido**: sin fix
        

---

### 11) Gestión de energía

- **Perfiles**:
    
    - _Bici_: tasa 1 Hz, IMU activa, deep sleep por inactividad 5 min.
        
    - _Vehículo_: 5 Hz cuando en movimiento, 0.5 Hz en reposo.
        
- **Estimación autonomía** (ejemplo, 2500 mAh): 12–20 h (dependiendo del GPS/IMU/GNSS fix).
    
- **Protecciones**: LVP a 3.2 V, corte por sobrecorriente del cargador, telemetría de batería vía ADC (divisor resistivo de alto valor + mosfet de medida).
    

---

### 12) Plan de pruebas

- **Laboratorio**:
    
    - Validación de parseo NMEA y precisión de velocidad (comparar con app del teléfono).
        
    - Simulación de cortes de Internet (hotspot OFF/ON) y verificación de reintentos y buffer.
        
    - Verificación de OTA (si aplica), histeresis de buzzer, y patrones de LEDs.
        
- **Campo**:
    
    - Recorridos urbanos y vías rápidas; comparación de trayectorias.
        
    - Pruebas con OSM (si disponible) versus umbral usuario.
        

---

### 13) Entregables

1. **Hardware** ensamblado (prototipo).
    
2. **Firmware** ESP‑IDF y código UI web (repositorio).
    
3. **Documentación**: esquema/bom/pinout, guía de instalación y uso.
    
4. **Demo** funcional (video + recorrido con datos).
    

---

### 14) Cronograma sugerido (6 semanas)

- **S1**: BoM, compras, base de firmware (drivers GPS/IMU, Wi‑Fi, LEDs/buzzer).
    
- **S2**: Web UI local, almacenamiento local, pruebas de energía.
    
- **S3**: Integración mapa y streaming; lógica de alertas.
    
- **S4**: Integración con backend opcional (MQTT/HTTP) + histórico.
    
- **S5**: Pruebas de campo, calibraciones, robustez de reconexión.
    
- **S6**: Documentación, demo y mejoras (OSM límites de velocidad).
    

---

### 15) Costos estimados (prototipo)

- ESP32‑WROOM‑32: 8–15 USD
    
- GNSS u‑blox/L76K con antena: 8–20 USD
    
- IMU: 2–8 USD
    
- Cargador USB‑C (TP4056/IP5306): 2–6 USD
    
- Batería 18650 2500–3000 mAh: 5–10 USD
    
- Misceláneos (LEDs, buzzer, PCB, conectores, caja): 5–15 USD  
    **Total estimado**: 30–70 USD
    

---

### 16) Riesgos y mitigaciones

- **Cobertura GPS limitada** (túneles/edificios): usar smoothing y mantener último fix válido.
    
- **Datos de límite de velocidad incompletos** en OSM: fallback a umbral usuario.
    
- **Consumo del GPS** alto: duty‑cycling del GPS cuando el IMU detecta reposo.
    
- **Reconexión hotspot**: almacenar múltiples SSID; reintentos exponenciales + modo AP de emergencia para reconfigurar.
    

---

### 17) Esquema lógico y pinout (sugerido)

- UART0 ↔ GNSS (TX/RX)
    
- I2C0 ↔ IMU (SDA/SCL)
    
- GPIOx ↔ LED Internet (azul)
    
- GPIOy ↔ LED GPS (verde)
    
- GPIOz (LEDC) ↔ Buzzer
    
- ADC1_CHx ↔ Medición batería (divisor)
    

_(Los GPIO exactos dependerán del módulo ESP32; se definirá en el firmware.)_

---

### 18) Especificación de interfaces

**MQTT (opcional)**

- Topic pub: `tracker/<device_id>/telemetry`
    
- Topic sub: `tracker/<device_id>/cmd`
    
- QoS 0/1; payload JSON (ver §9).
    

**REST local**

- `GET /api/status` → estado (JSON)
    
- `GET /api/track?since=t` → puntos desde `t`
    
- `POST /api/config` → SSID/clave/umbral/etc.
    

---

### 19) Criterios de aceptación (MVP)

- Fijar GPS y mostrar lat/lon y velocidad en UI local en < 60 s a cielo abierto.
    
- Conectarse al hotspot y confirmar Internet (LED azul fijo) en < 15 s.
    
- Activar buzzer cuando superar el umbral por > 2 s y detener correctamente al bajar.
    
- Registrar y reenviar datos al restaurar la conexión.
    

---

### 20) Trabajo futuro

- App móvil nativa (Android) para pairing y configuración.
    
- Geocercas y alertas por notificación push.
    
- BLE para configuración sin hotspot.
    
- Carcasa IP65 y ahorro energético avanzado.
    

---

### 21) Licencias y uso de datos

- **OSM** bajo ODbL; si se usa, se debe acreditar adecuadamente y respetar términos.
    
- Considerar **privacidad**: anonimización de recorridos y control de exportación.
    

---

## Fin de la propuesta