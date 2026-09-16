#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "Matning.h"

<<<<<<< HEAD
=======
// Antal matningar som sparas for dashboardens tabell.
// Detta ar en visningshistorik, inte en sandningsko: varden harifran
// skickas aldrig i efterhand till brokern.
#define HISTORIK_LANGD 20

>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
// Startar den lokala webbservern pa port 80.
void setupLocalServer();

// Maste anropas varje varv i loop() - servern ar synkron och hanterar
// en forfragan i taget.
void handleLocalServer();

<<<<<<< HEAD
// Sparar senaste matningen sa att dashboarden kan visa den.
void serverSetSenaste(const Matning& m);

#endif
=======
// Lagger in en ny matning i den lokala vyn. Anropas for VARJE matning,
// oavsett om den gick att skicka till brokern eller inte.
void serverNyMatning(const Matning& m);

#endif
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
