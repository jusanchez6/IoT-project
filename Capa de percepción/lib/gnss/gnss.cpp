#include <gnss.hpp>

GNSS::GNSS(HardwareSerial &serial, int rxPin, int txPin, uint32_t baudRate)
    : gnssSerial(serial), rx(rxPin), tx(txPin), baud(baudRate) {}

void GNSS::begin()
{
    // de esta forma se ve como mas elegant ejasjdajsd
    gnssSerial.begin(baud, SERIAL_8N1, rx, tx);
}

void GNSS::update()
{
    while (gnssSerial.available())
    {
        char c = gnssSerial.read();
    }

    if (gps.location.isUpdated())
    {
        addToBuffer(gps.location.lat(), gps.location.lng());
    }
}

bool GNSS::isReady()
{
    return gps.location.isValid() &&
           gps.hdop.isValid() &&
           gps.hdop.hdop() < hdopMax &&
           gps.satellites.isValid() &&
           gps.satellites.value() >= satellitesMin;
}

String GNSS::localTime(int offSetHours = -5)
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
        } else {
            h -= 24;
        }

        char buff[20];
        sprintf(buff, "%02d:%02d:%02d", h, m, s);

        return String(buff);
        
    } else {
        return "Invalid";
    }
}