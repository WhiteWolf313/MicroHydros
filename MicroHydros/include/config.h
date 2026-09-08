#pragma once
#include <Arduino.h>
 
// =====================================================
//  MicroHydros - konfiguration
//  Steg 1: endast utetemperatur (SHT31-D)
// =====================================================
 
// ---- Seriell debug ----
constexpr uint32_t SERIAL_BAUD = 115200;
 
// ---- I2C (ESP32-S3) ----
// Ändra till de pinnar du faktiskt lödde/kopplade på ditt kort.
constexpr int      I2C_SDA_PIN = 8;
constexpr int      I2C_SCL_PIN = 9;
constexpr uint32_t I2C_FREQ_HZ = 100000;  // 100 kHz, säkert med långa kablar ute
 
// ---- SHT31-D (ute) ----
// GY-SHT3x: AD -> GND (eller obelagd) = 0x44, AD -> VCC = 0x45
constexpr uint8_t SHT31_OUTDOOR_ADDR = 0x44;
 
// Inbyggd värmare. Håll false vid normal mätning - den värmer chippet
// några grader och förstör temperaturvärdet. Används bara för att
// torka bort kondens vid hög luftfuktighet.
constexpr bool SHT31_HEATER_ENABLED = false;
 
// ---- Mätintervall ----
constexpr uint32_t SENSOR_READ_INTERVAL_MS = 5000;