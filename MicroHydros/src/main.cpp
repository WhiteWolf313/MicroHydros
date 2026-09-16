#include <Arduino.h>
#include "Sensors.h"
#include "Cloud.h"
<<<<<<< HEAD
#include "Buffer.h"
=======
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
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
<<<<<<< HEAD
    maintainMQTT();         // broker: ateranslutning och tomning av bufferten
=======
    maintainMQTT();         // broker: ateranslutning utan att blockera loopen
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    handleLocalServer();    // lokal dashboard, fungerar aven helt utan internet

    unsigned long nu = millis();
    if (nu - sistaMatning < MATINTERVALL_MS) return;
    sistaMatning = nu;

    Matning m = lasSensorer();
<<<<<<< HEAD
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
=======

    // Den lokala vyn uppdateras ALLTID och forst. Den ar oberoende av
    // internet och ar darfor systemets tillforlitliga vag ut for matdata.
    serverNyMatning(m);

    // Molnet ar ett tillagg. Nar brokern inte gar att na hoppas den har
    // matningen over - inget buffras och inget skickas i efterhand.
    if (!publiceraMatning(m)) {
        Serial.print("Ingen kontakt med brokern - matning ");
        Serial.print(m.seq);
        Serial.println(" visas bara i den lokala vyn.");
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    }
}
