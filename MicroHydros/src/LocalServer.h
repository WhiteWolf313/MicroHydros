#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "Matning.h"

// Antal matningar som sparas for dashboardens tabell.
// Detta ar en visningshistorik, inte en sandningsko: varden harifran
// skickas aldrig i efterhand till brokern.
#define HISTORIK_LANGD 20

// Startar den lokala webbservern pa port 80.
void setupLocalServer();

// Maste anropas varje varv i loop() - servern ar synkron och hanterar
// en forfragan i taget.
void handleLocalServer();

// Lagger in en ny matning i den lokala vyn. Anropas for VARJE matning,
// oavsett om den gick att skicka till brokern eller inte.
void serverNyMatning(const Matning& m);

#endif
