#include "Sensors.h"
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>


Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setupSensors() {

    
    // Starta I2C för SHT31 (0x44 är standardadressen)
    if (!sht31.begin(0x44)) {
        Serial.println("Kunde inte hitta SHT31!");
    }
}

float getAirTempIn() {
    float temp = sht31.readTemperature();
    if (!isnan(temp) && temp > -20 && temp < 60) {
        return temp;
    }
    return -999.0;
}

float getAirHumidityIn() {
    float hum = sht31.readHumidity();
    if (!isnan(hum) && hum >= 0 && hum <= 100) {
        return hum;
    }
    return -999.0;
}