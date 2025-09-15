/**
 * @file platform.hpp
 * @brief Implementación de periféricos de plataforma: LED RGB, alarma (buzzer) y batería.
 *
 * Este archivo la implementación de los metodos de las clases que encapsulan
 * el control de hardware como LEDs RGB, alarmas acústicas y gestión de batería.
 * 
 * 
 * @author Julian Sanchez
 * @date 15/09/2025
 * 
 * @version 1.0
 */

#include <platform.hpp>

LED::LED(uint16_t numPixels, int16_t pin, neoPixelType type)
    : rgb(numPixels, pin, type)
{
}

void LED::begin(void)
{
    rgb.begin();
    rgb.show();
}

void LED::setColor(uint32_t color) {
    rgb.clear();
    rgb.setPixelColor(0, color);
    rgb.show();
}


ALARM::ALARM (uint8_t pin) : alarmPin(pin) {}

void ALARM::begin () {
    pinMode(alarmPin, OUTPUT);
    digitalWrite(alarmPin, LOW);
}

void ALARM::turnOnAlarm () {
    digitalWrite(alarmPin, HIGH);
    alarmState = true;
}

void ALARM::turnOffAlarm () {
    if (alarmState) {
        digitalWrite(alarmPin, LOW);
        alarmState = false; 
    } else  {
        Serial.println("La alarma ya está apagada.");
    }
}


