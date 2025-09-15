/**
 * @file platform.hpp
 * @brief Definición de periféricos de plataforma: LED RGB, alarma (buzzer) y batería.
 *
 * Este archivo define macros de colores preestablecidos y clases que encapsulan
 * el control de hardware como LEDs RGB, alarmas acústicas y gestión de batería.
 * 
 * 
 * @author Julian Sanchez
 * @date 15/09/2025
 * 
 * @version 1.0
 */

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ====== Colores básicos ======
#define COLOR_RED       0x00000077  ///< Rojo intenso
#define COLOR_GREEN     0x00007700  ///< Verde intenso
#define COLOR_BLUE      0x00770000  ///< Azul intenso
#define COLOR_WHITE     0x00777777  ///< Blanco
#define BLACK           0x00000000  ///< Negro

// ====== Variantes ======
#define COLOR_YELLOW    0x0000FFFF  ///< Amarillo (Rojo + Verde)
#define COLOR_CYAN      0x00FFFF00  ///< Cian (Verde + Azul)
#define COLOR_MAGENTA   0x00FF00FF  ///< Magenta (Rojo + Azul)
#define COLOR_ORANGE    0x000066FF  ///< Naranja cálido
#define COLOR_PURPLE    0x00CC00CC  ///< Púrpura bonito
#define COLOR_PINK      0x00FF66CC  ///< Rosa vibrante
#define COLOR_TURQUOISE 0x00FFCC66  ///< Turquesa claro
#define COLOR_MUSTARD   0x000099CC  ///< Mostaza (curioso y elegante)
#define COLOR_MINT      0x00CCFFCC  ///< Verde menta
#define COLOR_SKY       0x00FFCC00  ///< Azul cielo

/**
 * @class LED
 * @brief Clase para el control de un LED RGB basado en la librería Adafruit NeoPixel.
 */
class LED
{
public:
    /**
     * @brief Constructor de la clase LED.
     * @param numPixels Número de píxeles (LEDs) a controlar.
     * @param pin Pin digital donde está conectado el LED.
     * @param type Tipo de protocolo de color (por defecto NEO_GBR + NEO_KHZ800).
     */
    LED(uint16_t numPixels, int16_t pin, neoPixelType type = NEO_GBR + NEO_KHZ800);

    /**
     * @brief Inicializa el LED.
     */
    void begin(void);

    /**
     * @brief Establece un color en el LED.
     * @param color Código de color en formato GBR de 32 bits.
     */
    void setColor (uint32_t color);

private:
    Adafruit_NeoPixel rgb; ///< Objeto NeoPixel para controlar el LED.
};

/**
 * @class ALARM
 * @brief Clase para controlar una alarma acústica (buzzer).
 */
class ALARM
{
public:
    /**
     * @brief Constructor de la clase ALARM.
     * @param pin Pin digital donde está conectado el buzzer.
     */
    ALARM(uint8_t pin);

    /**
     * @brief Inicializa la alarma configurando el pin como salida.
     */
    void begin(void);

    /**
     * @brief Activa la alarma (enciende el buzzer).
     */
    void turnOnAlarm (void);

    /**
     * @brief Desactiva la alarma (apaga el buzzer).
     */
    void turnOffAlarm (void);

    /**
     * @brief Obtiene el estado actual de la alarma.
     * @return `true` si la alarma está encendida, `false` si está apagada.
     */
    bool getState (void ) {return alarmState;}

private:
    uint8_t alarmPin;     ///< Pin conectado al buzzer.
    bool alarmState = false; ///< Estado actual de la alarma.
};


// ============== UNDER CONSTRUCTION =================================

/**
 * @class BATTERY
 * @brief Clase en construcción para el monitoreo de batería.
 *
 * Permite medir el nivel de carga de la batería y detectar si está cargando.
 */
class BATTERY
{
public:
    /**
     * @brief Constructor de la clase BATTERY.
     */
    BATTERY();

    /**
     * @brief Inicializa el monitoreo de batería (configura ADC).
     */
    void begin();

    /**
     * @brief Obtiene el nivel de batería medido.
     * @return Nivel de batería en unidades ADC (0–4095 para ESP32).
     */
    uint32_t getBateryLevel () {return bateryLevel;}

    /**
     * @brief Indica si la batería está en carga.
     * @return `true` si está cargando, `false` si no.
     */
    bool isCharging () {return charging;}

private:
    uint8_t adcPin = 4;         ///< Pin ADC usado para medir voltaje (ADC1_3 en ESP32-S3).
    uint32_t bateryLevel = 0;   ///< Último valor leído del ADC.
    bool charging = false;      ///< Estado de carga de la batería.
};

#endif // PLATFORM_HPP
