#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "Matning.h"

// Startar den lokala webbservern pa port 80.
void setupLocalServer();

// Maste anropas varje varv i loop() - servern ar synkron och hanterar
// en forfragan i taget.
void handleLocalServer();

// Sparar senaste matningen sa att dashboarden kan visa den.
void serverSetSenaste(const Matning& m);

#endif
