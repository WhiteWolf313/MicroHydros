#ifndef SENSORS_H
#define SENSORS_H

#include "Matning.h"   // FEL_VARDE anvands nu av hela systemet

void setupSensors();

// Inne (SHT31 pa I2C-buss 0)
float getAirTempIn();
float getAirHumidityIn();

// Ute (SHT31 pa I2C-buss 1)
float getAirTempOut();
float getAirHumidityOut();   // extra matvarde, inte ett krav i projektet

// Vattentemperatur (DS18B20 pa ONE_WIRE_BUS)
float getWaterTemp();

#endif
