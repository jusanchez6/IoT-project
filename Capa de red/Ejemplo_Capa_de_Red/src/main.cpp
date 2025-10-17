#include <Arduino.h>
#include <types.hpp>
#include <wifi_funcs.hpp>

SensorData_t sensor = {
    .latitude = 6.2518,    // Ejemplo: Medellín, Colombia
    .longitude = -75.5636, // Ejemplo: Medellín, Colombia
    .altitude = 1495.0,    // Metros sobre el nivel del mar
    .velocity = 12.3,      // km/h, simulando movimiento lento
    .gnssReady = true,     // GNSS con fix disponible
    .vibraciones = 0.035   // RMS bajo (sin mucha vibración)
};

void setup()
{
  Serial.begin(115200);
  init_communications();
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(5000);
  sendData(sensor);
}
