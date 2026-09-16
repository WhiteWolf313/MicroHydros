#ifndef CONFIG_H
#define CONFIG_H

// Kopiera denna fil till include/config.h och fyll i era egna uppgifter.
// include/config.h ska ligga i .gitignore och ALDRIG checkas in.

// --- Identitet ---
#define DEVICE_ID "microhydros-01"

// --- Wi-Fi (station) ---
#define WIFI_SSID "DITT_WIFI_NAMN"
#define WIFI_PASS "DITT_WIFI_LOSENORD"
#define WIFI_TIMEOUT_MS 15000    // hur lange uppstarten vantar pa Wi-Fi
#define WIFI_RETRY_MS   20000    // intervall mellan ateranslutningsforsok

// --- Lokal accesspunkt (reservlage nar Wi-Fi inte gar att na) ---
// Startas automatiskt efter tva misslyckade ateranslutningsforsok.
// Dashboarden nas da pa http://192.168.4.1
#define AP_SSID "MicroHydros-01"
#define AP_PASS "hydrogreen2026"   // minst 8 tecken, byt till ett eget

// --- MQTT ---
#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT   1883
#define MQTT_USER   "DITT_ANVANDARNAMN"
#define MQTT_KEY    "DIN_AIO_KEY"
#define MQTT_RETRY_MS      5000    // intervall mellan ateranslutningsforsok
#define FLUSH_INTERVALL_MS 2500    // takt nar bufferten toms (Adafruit IO: max 30/min)

// --- Topic for det strukturerade JSON-paketet ---
// Pa Adafruit IO maste feeden finnas och namnet skrivas som anvandare/f/feed-namn.
#define MQTT_TOPIC_DATA "DITT_ANVANDARNAMN/f/microhydros-json"

// Enskilda feeds behalls sa att Adafruit IO kan rita grafer per matvarde.
// Satt till 0 om ni bara vill skicka JSON-paketet.
#define PUBLICERA_ENSKILDA_FEEDS 1
#define FEED_VATTEN    "DITT_ANVANDARNAMN/f/vatten-temp"
#define FEED_LUFT_INNE "DITT_ANVANDARNAMN/f/temp-inne"
#define FEED_FUKT_INNE "DITT_ANVANDARNAMN/f/fukt-inne"
#define FEED_LUFT_UTE  "DITT_ANVANDARNAMN/f/temp-ute"

// --- Matning ---
// 30 sekunder: temperatur och luftfuktighet i en odlingsbehallare andras
// langsamt, och tatare matningar skulle bara oka nattrafiken och slosa energi.
#define MATINTERVALL_MS 30000

// --- Tid ---
#define NTP_SERVER "pool.ntp.org"

#endif
