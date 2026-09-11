#include "Sensors.h"
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>

// Två separata I2C-bussar => båda sensorerna får behålla adress 0x44.
// Ingen AD-pinne behöver kopplas om.
#define PIN_SDA_INNE 4
#define PIN_SCL_INNE 5
#define PIN_SDA_UTE  10
#define PIN_SCL_UTE  11

#define SHT31_ADDR 0x44
#define I2C_HASTIGHET 100000

#define FEL_VARDE -999.0

// VIKTIGT: TwoWire-pekaren skickas till KONSTRUKTORN, inte till begin().
// Wire = buss 0, Wire1 = buss 1. Båda finns redan i ESP32-kärnan.
Adafruit_SHT31 sht31_inne = Adafruit_SHT31(&Wire);
Adafruit_SHT31 sht31_ute  = Adafruit_SHT31(&Wire1);

static bool inneHittad = false;
static bool uteHittad  = false;

void setupSensors() {
    // 1. Inne-bussen
    Wire.begin(PIN_SDA_INNE, PIN_SCL_INNE, I2C_HASTIGHET);
    inneHittad = sht31_inne.begin(SHT31_ADDR);
    Serial.println(inneHittad ? "SHT31 INNE hittad (buss 0, GPIO 8/9)."
                              : "Kunde inte hitta SHT31 - INNE!");

    // 2. Ute-bussen
    Wire1.begin(PIN_SDA_UTE, PIN_SCL_UTE, I2C_HASTIGHET);
    uteHittad = sht31_ute.begin(SHT31_ADDR);
    Serial.println(uteHittad ? "SHT31 UTE hittad (buss 1, GPIO 10/11)."
                             : "Kunde inte hitta SHT31 - UTE!");
}

// --- Inne ---
float getAirTempIn() {
    if (!inneHittad) return FEL_VARDE;

    float temp = sht31_inne.readTemperature();
    if (!isnan(temp) && temp > -20 && temp < 60) {
        return temp;
    }
    return FEL_VARDE;
}

float getAirHumidityIn() {
    if (!inneHittad) return FEL_VARDE;

    float hum = sht31_inne.readHumidity();
    if (!isnan(hum) && hum >= 0 && hum <= 100) {
        return hum;
    }
    return FEL_VARDE;
}

// --- Ute ---
float getAirTempOut() {
    if (!uteHittad) return FEL_VARDE;

    float temp = sht31_ute.readTemperature();
    // Utökat nedre intervall för vintertemperatur
    if (!isnan(temp) && temp > -40 && temp < 60) {
        return temp;
    }
    return FEL_VARDE;
}

float getAirHumidityOut() {
    if (!uteHittad) return FEL_VARDE;

    float hum = sht31_ute.readHumidity();
    if (!isnan(hum) && hum >= 0 && hum <= 100) {
        return hum;
    }
    return FEL_VARDE;
}