#include "Sensors.h"
#include "config.h"
#include <Wire.h>

bool Sensors::begin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ);
    return reconnect();
}

bool Sensors::reconnect() {
    _ready = _sht31Outdoor.begin(SHT31_OUTDOOR_ADDR);
    if (_ready) {
        _sht31Outdoor.heater(SHT31_HEATER_ENABLED);
    }
    return _ready;
}

OutdoorReading Sensors::readOutdoor() {
    OutdoorReading reading;

    if (!_ready) {
        return reading;  // valid = false
    }

    const float t = _sht31Outdoor.readTemperature();
    const float h = _sht31Outdoor.readHumidity();

    // Biblioteket returnerar NAN vid CRC-fel eller uteblivet svar.
    if (isnan(t) || isnan(h)) {
        _ready = false;  // tvinga fram en reconnect i loop()
        return reading;
    }

    // Enkel rimlighetskontroll mot databladets mätområde.
    if (t < -40.0f || t > 125.0f || h < 0.0f || h > 100.0f) {
        return reading;
    }

    reading.temperatureC = t;
    reading.humidityPct  = h;
    reading.valid        = true;
    return reading;
}