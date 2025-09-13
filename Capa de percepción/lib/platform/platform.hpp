#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ====== Colores básicos ======
#define COLOR_RED       0x000000FF  // Rojo intenso
#define COLOR_GREEN     0x0000FF00  // Verde intenso
#define COLOR_BLUE      0x00FF0000  // Azul intenso
#define COLOR_WHITE     0x00FFFFFF  // Blanco

// ====== Variantes ======
#define COLOR_YELLOW    0x0000FFFF  // Amarillo (Rojo + Verde)
#define COLOR_CYAN      0x00FFFF00  // Cian (Verde + Azul)
#define COLOR_MAGENTA   0x00FF00FF  // Magenta (Rojo + Azul)
#define COLOR_ORANGE    0x000066FF  // Naranja cálido
#define COLOR_PURPLE    0x00CC00CC  // Púrpura bonito
#define COLOR_PINK      0x00FF66CC  // Rosa vibrante
#define COLOR_TURQUOISE 0x00FFCC66  // Turquesa claro
#define COLOR_MUSTARD   0x000099CC  // Mostaza (curioso y elegante)
#define COLOR_MINT      0x00CCFFCC  // Verde menta
#define COLOR_SKY       0x00FFCC00  // Azul cielo



class LED
{
public:
    LED(uint16_t numPixels, int16_t pin, neoPixelType type = NEO_GBR + NEO_KHZ800);


    void begin(void);
    void setColor (uint32_t color);

private:
    Adafruit_NeoPixel rgb;

};


class ALARM
{
public:

    ALARM();

    void begin (uint8_t pin);
    void turnOnAlarm (void);
    void turnOffAlarm (void);

private:
    uint8_t alarmPin;
};

class BATTERY
{
    public:
    BATTERY();

    void begin();
    uint32_t getBateryLevel () {return bateryLevel;}
    bool isCharging () {return charging;}
    

    private:
        uint8_t adcPin = 4;         //ADC1_3
        uint32_t bateryLevel = 0;
        bool charging = false;


};

#endif // PLATFORM_HPP