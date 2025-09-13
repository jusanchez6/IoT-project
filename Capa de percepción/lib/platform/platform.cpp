#include <platform.hpp>

LED::LED(uint16_t numPixels, int16_t pin, neoPixelType type)
    : rgb(numPixels, pin, type) // 👈 aquí eliges explícitamente el constructor
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
