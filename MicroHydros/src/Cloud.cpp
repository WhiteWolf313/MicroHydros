#include "Cloud.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "../include/config.h" // Inkludera våra hemligheter

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Variabel för att hålla koll på när vi senast försökte ansluta
unsigned long lastReconnectAttempt = 0;

void setupCloud() {
    Serial.print("Ansluter till WiFi: ");
    Serial.println(WIFI_SSID);
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi anslutet!");
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

// Ändrad från void till bool för att kunna meddela om anslutningen lyckades
bool connectMQTT() {
    Serial.print("Försöker ansluta till MQTT...");
    
    // Skapa ett slumpmässigt ID för enheten
    String clientId = "MicroHydros-";
    clientId += String(random(0xffff), HEX);
    
    // Försök ansluta en gång (utan while-loop!)
    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_KEY)) {
        Serial.println("Ansluten!");
        return true; // Lyckades
    } else {
        Serial.print("Misslyckades, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" försöker igen om 5 sekunder");
        return false; // Misslyckades
    }
}

void maintainMQTT() {
    // Om vi inte är anslutna, försök ansluta, men lås inte programmet!
    if (!mqttClient.connected()) {
        unsigned long now = millis();
        // Vänta 5 sekunder mellan försöken så vi inte spammar servern
        if (now - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = now;
            // Om den lyckas, återställ tiden
            if (connectMQTT()) {
                lastReconnectAttempt = 0;
            }
        }
    } else {
        // Om vi är anslutna, håll anslutningen vid liv och lyssna efter inkommande meddelanden
        mqttClient.loop();
    }
}

void publishData(const char* feed, float value) {
    if (value != -999.0) { // Skicka bara om värdet är rimligt
        // Konvertera float till char-array eftersom MQTT kräver det
        char payload[10];
        dtostrf(value, 4, 1, payload); // Behåll 1 decimal
        
        mqttClient.publish(feed, payload);
        Serial.print("Skickade: ");
        Serial.print(payload);
        Serial.print(" till ");
        Serial.println(feed);
    }
}
//Funnktionen frågar om MQTT är konaktad
bool isMQTTConnected() {
    return mqttClient.connected();
}