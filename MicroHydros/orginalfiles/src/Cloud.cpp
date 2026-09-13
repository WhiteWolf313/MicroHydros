#include "Cloud.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "../include/config.h" // Inkludera våra hemligheter

WiFiClient espClient;
PubSubClient mqttClient(espClient);

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

void connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Försöker ansluta till MQTT...");
        // Skapa ett slumpmässigt ID för enheten
        String clientId = "MicroHydros-";
        clientId += String(random(0xffff), HEX);
        
        if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_KEY)) {
            Serial.println("Ansluten!");
        } else {
            Serial.print("Misslyckades, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" försöker igen om 5 sekunder");
            delay(5000);
        }
    }
}

void maintainMQTT() {
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();
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