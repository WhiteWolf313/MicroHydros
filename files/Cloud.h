#ifndef CLOUD_H
#define CLOUD_H

#include "Matning.h"

// Startar Wi-Fi (station), NTP och konfigurerar MQTT-klienten.
// Om Wi-Fi inte gar att na startas en lokal accesspunkt sa att
// dashboarden anda ar atkomlig.
void setupCloud();

// Anropas varje varv i loop(): halller Wi-Fi vid liv och slar pa
// accesspunkten vid langre avbrott.
void maintainNetwork();

// Anropas varje varv i loop(): ateransluter till brokern utan att blockera
// och tommer offline-bufferten i lugn takt nar anslutningen ar tillbaka.
void maintainMQTT();

// Paketerar en matning som JSON och publicerar den.
// Returnerar false om publiceringen misslyckades -> anroparen buffrar vardet.
bool publiceraMatning(const Matning& m, bool franBuffert);

bool     isWiFiConnected();
bool     isAPMode();
bool     isMQTTConnected();
String   aktuellIP();
uint32_t antalSkickade();
uint32_t aktuellEpoch();   // 0 om NTP inte synkat an

#endif
