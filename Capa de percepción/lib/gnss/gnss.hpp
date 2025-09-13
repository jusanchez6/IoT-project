#ifndef GNSS_HPP
#define GNSS_HPP

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>



class GNSS {

public:
    GNSS (HardwareSerial &serial, int rxPin, int txPin, uint32_t baudRate);

    void begin();
    void update();
    bool isReady();

    double latitude()   {return avgLat; }
    double longitude()  {return avgLon; }
    double altitude()   {return gps.altitude.meters(); }
    int satelites()     {return gps.satellites.value(); }
    double hdop()       {return gps.hdop.hdop(); }
    String localTime (int offsetHours = -5);

private:
    HardwareSerial &gnssSerial;
    TinyGPSPlus gps;
    int rx, tx;
    uint32_t baud;

    // verificación de numeros de satelites y hdop
    const double hdopMax = 2.0;
    const uint32_t satellitesMin = 3;

    // filtro de media movil
    static const int AVG_SIZE = 10;
    double latBuffer[AVG_SIZE];
    double lonBuffer[AVG_SIZE];
    int bufferIndex = 0;
    bool bufferFilled = false;

    double avgLat = 0;
    double avgLon = 0;
    
    void addToBuffer(double lat, double lon);

};



#endif // GNSS_HPP