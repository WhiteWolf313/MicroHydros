
#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi
const char* WIFI_SSID = "DITT_WIFI_NAMN";
const char* WIFI_PASS = "DITT_WIFI_LOSENORD";

// Adafruit IO / MQTT
const char* MQTT_SERVER = "io.adafruit.com";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "Ditt_Adafruit_Anvandarnamn";
const char* MQTT_KEY = "Din_Adafruit_Nyckel";

// Feeds (Byt ut 'Ditt_Adafruit_Anvandarnamn' mot ert riktiga användarnamn)
const char* FEED_VATTEN = "Ditt_Adafruit_Anvandarnamn/f/vatten-temp";
const char* FEED_LUFT_INNE = "Ditt_Adafruit_Anvandarnamn/f/temp-inne";
const char* FEED_FUKT_INNE = "Ditt_Adafruit_Anvandarnamn/f/fukt-inne";

#endif