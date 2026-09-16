#ifndef BUFFER_H
#define BUFFER_H

#include "Matning.h"

// Ringbuffert for matvarden som INTE kunnat skickas till brokern.
// 240 platser * 30 s matintervall = ca 2 timmars avbrott utan dataforlust.
// Minnesatgang: 240 * 28 byte = ca 6,7 kB RAM, vilket ESP32 har gott om.
#define BUFFER_KAPACITET 240

// Kort historik som alltid fylls pa, aven nar allt fungerar.
// Anvands enbart av den lokala dashboarden.
#define HISTORIK_LANGD 20

// --- Offline-buffert ---
void   bufferPush(const Matning& m);   // lagger sist; aldsta varde skrivs over nar bufferten ar full
bool   bufferPeek(Matning& m);         // laser aldsta vardet utan att ta bort det
void   bufferPop();                    // tar bort aldsta vardet (anropas forst efter lyckad publicering)
size_t bufferAntal();
size_t bufferKapacitet();
bool   bufferHarTappatVarden();        // true om bufferten nagon gang blivit overfull

// --- Historik for dashboarden ---
void   historikPush(const Matning& m);
size_t historikAntal();
bool   historikGet(size_t index, Matning& m);  // index 0 = aldsta i historiken

#endif
