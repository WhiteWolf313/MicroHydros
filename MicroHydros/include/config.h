#ifndef CONFIG_H
#define CONFIG_H

// Kopiera denna fil till include/config.h och fyll i era egna uppgifter.
// include/config.h ska ligga i .gitignore och ALDRIG checkas in.
<<<<<<< HEAD

// --- Identitet ---
#define DEVICE_ID "microhydros-01"

// --- Wi-Fi (station) ---
#define WIFI_SSID "Vodafone-EBDF"
#define WIFI_PASS "xxxx"
#define WIFI_TIMEOUT_MS 15000    // hur lange uppstarten vantar pa Wi-Fi
#define WIFI_RETRY_MS   20000    // intervall mellan ateranslutningsforsok

=======

// --- Identitet ---
#define DEVICE_ID "microhydros-01"

// --- Wi-Fi (station) ---
#define WIFI_SSID "wifi-name"
#define WIFI_PASS "pass"  //
#define WIFI_TIMEOUT_MS 15000    // hur lange uppstarten vantar pa Wi-Fi
#define WIFI_RETRY_MS   20000    // intervall mellan ateranslutningsforsok

>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
// --- Lokal accesspunkt (reservlage nar Wi-Fi inte gar att na) ---
// Startas automatiskt efter tva misslyckade ateranslutningsforsok.
// Dashboarden nas da pa http://192.168.4.1
#define AP_SSID "MicroHydros-01"
#define AP_PASS "hydrogreen2026"   // minst 8 tecken, byt till ett eget

// --- MQTT ---
#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT   1883
#define MQTT_USER   "WhiteWolf313"
<<<<<<< HEAD
#define MQTT_KEY    "key?"
#define MQTT_RETRY_MS      5000    // intervall mellan ateranslutningsforsok
#define FLUSH_INTERVALL_MS 2500    // takt nar bufferten toms (Adafruit IO: max 30/min)
=======
#define MQTT_KEY    "key"
#define MQTT_RETRY_MS      5000    // intervall mellan ateranslutningsforsok
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

// --- Topic for det strukturerade JSON-paketet ---
// Pa Adafruit IO maste feeden finnas och namnet skrivas som anvandare/f/feed-namn.
#define MQTT_TOPIC_DATA "WhiteWolf313/f/microhydros-json"

<<<<<<< HEAD
=======
// --- Topic for det strukturerade JSON-paketet ---
// Pa Adafruit IO maste feeden finnas och namnet skrivas som anvandare/f/feed-namn.
#define MQTT_TOPIC_DATA "WhiteWolf313/f/microhydros-json"

>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
// Enskilda feeds behalls sa att Adafruit IO kan rita grafer per matvarde.
// Satt till 0 om ni bara vill skicka JSON-paketet.
#define PUBLICERA_ENSKILDA_FEEDS 1
#define FEED_VATTEN    "WhiteWolf313/f/vatten-temp"
#define FEED_LUFT_INNE "WhiteWolf313/f/temp-inne"
#define FEED_FUKT_INNE "WhiteWolf313/f/fukt-inne"
#define FEED_LUFT_UTE  "WhiteWolf313/f/temp-ute"

// --- Matning ---
// 15 sekunder: temperatur och luftfuktighet i en odlingsbehallare andras
// langsamt, och tatare matningar skulle bara oka nattrafiken och slosa energi.
#define MATINTERVALL_MS 15000

// --- Tid ---
#define NTP_SERVER "pool.ntp.org"

#endif
