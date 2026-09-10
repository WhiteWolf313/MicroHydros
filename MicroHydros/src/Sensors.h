#ifndef SENSORS_H
#define SENSORS_H

void setupSensors();

// Inne (SHT31 pa I2C-buss 0)
float getAirTempIn();
float getAirHumidityIn();

// Ute (SHT31 pa I2C-buss 1)
float getAirTempOut();

//vatten-temp (DS18B20 pin 4)
float getWaterTemp();


#endif