/**
 * @file gnss.cpp
 * @brief Librería para manejar un receptor GNSS con ESP32 usando TinyGPS++.
 *
 * Esta clase encapsula la configuración y lectura de datos de un módulo GNSS 
 * conectado por UART, permitiendo obtener latitud, longitud, altitud y hora local.
 * 
 * @author Julian Sanchez
 * @date 12/09/2025
 * @version 1.0
 * 
 */

#include <gnss.hpp>

GNSS::GNSS(HardwareSerial &serial, int rxPin, int txPin, uint32_t baudRate)
    : gnssSerial(serial), rx(rxPin), tx(txPin), baud(baudRate) {}

void GNSS::begin()
{
    // de esta forma se ve como mas elegant ejasjdajsd
    gnssSerial.begin(baud, SERIAL_8N1, rx, tx);
    setGnssRate(gnssSerial);
}

void GNSS::update()
{
    dataReady = false;
    while (gnssSerial.available())
    {
        char c = gnssSerial.read();
        gps.encode(c);
    }

    if (gps.location.isUpdated() && gps.location.isValid())
    {
        dataReady = true;
        
    }
}


String GNSS::localTime(int offSetHours)
{
    if (gps.time.isValid())
    {
        int h = gps.time.hour();
        int m = gps.time.minute();
        int s = gps.time.second();

        // Ajuste a hora local Colombia (UTC−5)
        h = h - 5;
        if (h < 0)
        {
            h += 24; // retrocede al día anterior
        }

        char buff[20];
        sprintf(buff, "%02d:%02d:%02d", h, m, s);

        return String(buff);

    } else {
        return "Invalid";
    }
}


void GNSS::setGnssRate(HardwareSerial &serial) {

    uint8_t setrate[] = {
    0xB5, 0x62,             // header
    0x06, 0x08,             // CFG-RATE
    0x06, 0x00,             // length = 6
    0xC8, 0x00,             // measRate = 200 ms (5 Hz)
    0x01, 0x00,             // navRate = 1
    0x01, 0x00,             // timeRef = UTC
    0xDE, 0x6A              // checksum (muy importante)
  };

  serial.write(setrate, sizeof(setrate));
}
