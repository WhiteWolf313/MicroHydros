#include <Arduino.h>
#include "Sensors.h"
#include "Cloud.h"
#include "../include/config.h" // Inkludera våra hemligheter


// --- BUFFERT-INSTÄLLNINGAR ---
// Vi skapar en datastruktur för att hålla en hel mätning
struct Measurement {
    float waterTemp;
    float airTempIn;
    float airHumIn;
    float airTempOut;
};

const int BUFFER_SIZE = 30;     // Hur många mätningar vi kan spara offline
Measurement buffer[BUFFER_SIZE]; // Vår "kö" i arbetsminnet
int bufferCount = 0;            // Håller koll på hur många mätningar som ligger i kön

unsigned long lastMsgTime = 0;
const long interval = 10000; // Skicka data var 10:e sekund (10000 millisekunder)

void setup() {
    // Starta serieporten för att kunna läsa loggar i VS Code (monitor_speed i platformio.ini)
    Serial.begin(115200);
    delay(1000);

    Serial.println("--- MicroHydros Startar ---");

    setupSensors();
    setupCloud();
}

void loop() {
    // Håller MQTT-anslutningen vid liv och återansluter om den dör
    maintainMQTT();

    unsigned long now = millis();

    // Körs var 10:e sekund (istället för delay(), vilket kallas 'non-blocking')
    if (now - lastMsgTime > interval) {
        lastMsgTime = now;

        Serial.println("\nLäser sensorer...");

        // Inne
        float airTempIn = getAirTempIn();
        float airHumIn  = getAirHumidityIn();

        // Ute
        float airTempOut = getAirTempOut();

        //vatten-temp
        float waterTemp = getWaterTemp();
       


        // Kontrollera om vi har anslutning till molnet
        if (isMQTTConnected()) {
            
            // 1. HAR VI SPARAD DATA I BUFFERTEN? Töm den först!
            while (bufferCount > 0) {
                Serial.println("Skickar buffrad data från offline-period...");
                
                // Hämta det senaste sparade värdet
                bufferCount--; 
                
                publishData(FEED_VATTEN, buffer[bufferCount].waterTemp);
                publishData(FEED_LUFT_INNE, buffer[bufferCount].airTempIn);
                publishData(FEED_FUKT_INNE, buffer[bufferCount].airHumIn);
                publishData(FEED_LUFT_INNE, buffer[bufferCount].airTempOut);

                // VIKTIGT: Adafruit IO tillåter max 30 värden i minuten (Rate Limit).
                // Vi måste pausa lite mellan skickningarna, annars blockeras vi.
                delay(3000); 
            }

            // 2. Skicka den aktuella (nya) mätningen
            publishData(FEED_VATTEN, waterTemp);
            publishData(FEED_LUFT_INNE, airTempIn);
            publishData(FEED_FUKT_INNE, airHumIn);
            publishData(FEED_LUFT_INNE, airTempOut);
            
        } else {
            // NÄTVERKET ÄR NERE - SPARA TILL LOKAL BUFFERT
            if (bufferCount < BUFFER_SIZE) {
                buffer[bufferCount].waterTemp = waterTemp;
                buffer[bufferCount].airTempIn = airTempIn;
                buffer[bufferCount].airHumIn = airHumIn;
                buffer[bufferCount].airTempOut = airTempOut;
                bufferCount++;
                
                Serial.print("Offline! Mätning sparad i buffert. Plats: ");
                Serial.print(bufferCount);
                Serial.print("/");
                Serial.println(BUFFER_SIZE);
            } else {
                Serial.println("Varning: Nätverket är nere och bufferten är FULL! Gammal data slängs.");
                
            }
        }
    }
}

























