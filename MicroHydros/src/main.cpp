#include <Arduino.h>
#include "Sensors.h"
#include "Cloud.h"
#include "LocalServer.h"
#include "Matning.h"
#include "config.h"

static unsigned long sistaMatning = 0;
static uint32_t seqRaknare = 0;

// Laser alla fyra matpunkter och paketerar dem i en Matning.
static Matning lasSensorer() {
    Matning m;
    m.seq        = ++seqRaknare;
    m.epoch      = aktuellEpoch();
    m.uptime_s   = millis() / 1000;
    m.tempInne   = getAirTempIn();
    m.fuktInne   = getAirHumidityIn();
    m.tempUte    = getAirTempOut();
    m.tempVatten = getWaterTemp();
    return m;
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("--- MicroHydros startar ---");

    setupSensors();
    setupCloud();
    setupLocalServer();
}

void loop() {
    maintainNetwork();      // Wi-Fi: ateranslutning och accesspunkt vid avbrott
    maintainMQTT();         // broker: ateranslutning utan att blockera loopen
    handleLocalServer();    // lokal dashboard, fungerar aven helt utan internet

    unsigned long nu = millis();
    if (nu - sistaMatning < MATINTERVALL_MS) return;
    sistaMatning = nu;

    Matning m = lasSensorer();

    // Den lokala vyn uppdateras ALLTID och forst. Den ar oberoende av
    // internet och ar darfor systemets tillforlitliga vag ut for matdata.
    serverNyMatning(m);

    // Molnet ar ett tillagg. Nar brokern inte gar att na hoppas den har
    // matningen over - inget buffras och inget skickas i efterhand.
    if (!publiceraMatning(m)) {
        Serial.print("Ingen kontakt med brokern - matning ");
        Serial.print(m.seq);
        Serial.println(" visas bara i den lokala vyn.");
    }
}
