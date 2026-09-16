#ifndef CLOUD_H
#define CLOUD_H

#include "Matning.h"

// Startar Wi-Fi (station), NTP och konfigurerar MQTT-klienten.
// Om Wi-Fi inte gar att na startas en lokal accesspunkt sa att
// dashboarden anda ar atkomlig.
void setupCloud();

<<<<<<< HEAD
// Anropas varje varv i loop(): halller Wi-Fi vid liv och slar pa
// accesspunkten vid langre avbrott.
void maintainNetwork();

// Anropas varje varv i loop(): ateransluter till brokern utan att blockera
// och tommer offline-bufferten i lugn takt nar anslutningen ar tillbaka.
void maintainMQTT();

// Paketerar en matning som JSON och publicerar den.
// Returnerar false om publiceringen misslyckades -> anroparen buffrar vardet.
bool publiceraMatning(const Matning& m, bool franBuffert);
=======
// Anropas varje varv i loop(): haller Wi-Fi vid liv och slar pa
// accesspunkten vid langre avbrott.
void maintainNetwork();

// Anropas varje varv i loop(): ateransluter till brokern utan att blockera.
void maintainMQTT();

// Paketerar en matning som JSON och publicerar den till brokern.
// Returnerar false om brokern inte ar nabar eller om publiceringen misslyckades.
// Matningen visas anda i den lokala vyn, men nar da aldrig molnet.
bool publiceraMatning(const Matning& m);
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

bool     isWiFiConnected();
bool     isAPMode();
bool     isMQTTConnected();
String   aktuellIP();
uint32_t antalSkickade();
<<<<<<< HEAD
=======
uint32_t antalEjSkickade();
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
uint32_t aktuellEpoch();   // 0 om NTP inte synkat an

#endif
