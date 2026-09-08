#include <Arduino.h>
#include "config.h"
#include "Sensors.h"

static Sensors  sensors;
static uint32_t lastReadMs = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(300);  // hinner USB-CDC på S3 komma upp

    Serial.println();
    Serial.println(F("MicroHydros - utetemperatur (SHT31-D)"));

    if (sensors.begin()) {
        Serial.printf("SHT31-D hittad pa adress 0x%02X\n", SHT31_OUTDOOR_ADDR);
    } else {
        Serial.printf("FEL: ingen SHT31-D pa 0x%02X. Kolla SDA/SCL, 3V3, GND och AD-pinnen.\n",
                      SHT31_OUTDOOR_ADDR);
    }
}

void loop() {
    const uint32_t now = millis();

    // Icke-blockerande intervall - inga delay() i loopen.
    if (now - lastReadMs < SENSOR_READ_INTERVAL_MS) {
        return;
    }
    lastReadMs = now;

    if (!sensors.isReady()) {
        Serial.println(F("Sensorn svarar inte - forsoker koppla upp igen..."));
        sensors.reconnect();
        return;
    }

    const OutdoorReading r = sensors.readOutdoor();

    if (!r.valid) {
        Serial.println(F("Lasfel fran SHT31-D (ute)."));
        return;
    }

    Serial.printf("Ute: %.2f C | %.1f %%RH\n", r.temperatureC, r.humidityPct);
}