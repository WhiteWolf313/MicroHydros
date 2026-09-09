#include "WaterTemperature.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define WATER_TEMP_PIN 4

OneWire oneWire(WATER_TEMP_PIN);
DallasTemperature waterSensor(&oneWire);

void setupWaterTemperature()
{
    waterSensor.begin();
}

float readWaterTemperature()
{
    waterSensor.requestTemperatures();

    return waterSensor.getTempCByIndex(0);
}