#ifndef MATNING_H
#define MATNING_H

#include <Arduino.h>

// Gemensam felkod for alla sensorlasningar.
// Valdes som ett varde som aldrig kan forekomma som en rimlig matning.
#define FEL_VARDE -999.0f

// Ett komplett matvarde fran ett matningstillfalle.
// Samma struct anvands av sensorlasning, MQTT-publicering och dashboard
// sa att det bara finns EN representation av matdata i systemet.
struct Matning {
    uint32_t seq;        // lopnummer, gor det mojligt att upptacka tappade varden
    uint32_t epoch;      // unix-tid i sekunder, 0 om NTP inte hunnit synka
    uint32_t uptime_s;   // sekunder sedan uppstart (fungerar aven utan NTP)
    float tempInne;      // grader C, FEL_VARDE vid fel
    float fuktInne;      // % RH, FEL_VARDE vid fel
    float tempUte;       // grader C, FEL_VARDE vid fel
    float tempVatten;    // grader C, FEL_VARDE vid fel
};

inline bool arGiltigt(float v) {
    return !isnan(v) && v != FEL_VARDE;
}

#endif
