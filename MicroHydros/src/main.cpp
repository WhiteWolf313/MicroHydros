#include <Arduino.h>
#include "Sensors.h"
#include "Cloud.h"



unsigned long lastMsgTime = 0;
const long interval = 10000; // Skicka data var 10:e sekund (10000 millisekunder)

void setup() {
    // Starta serieporten för att kunna läsa loggar i VS Code (monitor_speed i platformio.ini)
    Serial.begin(115200);
    delay(1000);

    Serial.println("--- MicroHydros Startar ---");
    
  
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
        
      
        
        
    }
}
