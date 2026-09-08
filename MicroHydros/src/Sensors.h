#pragma once
#include <Arduino.h>
#include <Adafruit_SHT31.h>
 
/**
 * En avläsning från utomhussensorn.
 * valid = false betyder att värdena inte får användas (CRC-fel,
 * ingen kontakt på I2C, eller att begin() aldrig lyckades).
 */
struct OutdoorReading {
    float temperatureC = NAN;
    float humidityPct  = NAN;
    bool  valid        = false;
};
 
class Sensors {
public:
    /** Startar I2C och SHT31-D. Returnerar false om sensorn inte svarar. */
    bool begin();
 
    /** Försöker koppla upp sensorn igen efter ett fel. */
    bool reconnect();
 
    /** true om sensorn svarade senast vi pratade med den. */
    bool isReady() const { return _ready; }
 
    /** Läser temperatur + RH från utomhussensorn. */
    OutdoorReading readOutdoor();
 
private:
    Adafruit_SHT31 _sht31Outdoor;
    bool           _ready = false;
};