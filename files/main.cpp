#include <Arduino.h>
#include "Sensors.h"
#include "Cloud.h"
#include "Buffer.h"
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
    maintainMQTT();         // broker: ateranslutning och tomning av bufferten
    handleLocalServer();    // lokal dashboard, fungerar aven helt utan internet

    unsigned long nu = millis();
    if (nu - sistaMatning < MATINTERVALL_MS) return;
    sistaMatning = nu;

    Matning m = lasSensorer();
    historikPush(m);        // alltid, sa att dashboarden har nagot att visa
    serverSetSenaste(m);

    // Ordningen ar viktig: sa lange det finns aldre oskickade varden i
    // bufferten laggs aven det nya vardet sist i kon. Annars skulle matserien
    // komma till molnet i fel ordning.
    if (isMQTTConnected() && bufferAntal() == 0) {
        if (!publiceraMatning(m, false)) {
            bufferPush(m);
        }
    } else {
        bufferPush(m);
        Serial.print("Ingen broker - vardet buffras lokalt (");
        Serial.print(bufferAntal());
        Serial.print("/");
        Serial.print(bufferKapacitet());
        Serial.println(")");
    }
}
