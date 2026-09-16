#include "Cloud.h"
#include "Buffer.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "config.h"

static WiFiClient   espClient;
static PubSubClient mqttClient(espClient);

static unsigned long sistaMqttForsok = 0;
static unsigned long sistaWifiForsok = 0;
<<<<<<< HEAD
static unsigned long sistaFlush      = 0;
static uint8_t  misslyckadeWifi = 0;
static bool     apAktiv         = false;
static uint32_t skickade        = 0;
=======
static uint8_t  misslyckadeWifi = 0;
static bool     apAktiv         = false;
static uint32_t skickade        = 0;
static uint32_t ejSkickade      = 0;
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

// ---------------------------------------------------------------------------
// Wi-Fi
// ---------------------------------------------------------------------------
static void startaAccesspunkt() {
    if (apAktiv) return;
    // WIFI_AP_STA: accesspunkten startas men enheten fortsatter forsoka
<<<<<<< HEAD
    // ansluta till det vanliga natverket i bakgrunden.
=======
    // ansluta till det vanliga natverket i bakgrunden..
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASS);
    apAktiv = true;
    Serial.print("Lokal accesspunkt startad: ");
    Serial.print(AP_SSID);
    Serial.print(" -> http://");
    Serial.println(WiFi.softAPIP());
}

void setupCloud() {
    Serial.print("Ansluter till WiFi: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Vanta max WIFI_TIMEOUT_MS i uppstarten. Vi vagrar blockera langre an sa,
<<<<<<< HEAD
    // eftersom prototypen ska fungera aven utan natverk.
=======
    // eftersom prototypen ska fungera lokalt aven helt utan natverk.
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_TIMEOUT_MS) {
        delay(250);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("\nWiFi anslutet. IP: ");
        Serial.println(WiFi.localIP());
<<<<<<< HEAD
        // NTP: riktiga tidsstamplar gor buffrade varden anvandbara i efterhand.
=======
        // NTP ger riktiga tidsstamplar i JSON-paketet.
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
        configTime(0, 0, NTP_SERVER);
    } else {
        Serial.println("\nWiFi misslyckades - kor i lokalt lage.");
        startaAccesspunkt();
    }

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setBufferSize(512);   // standard ar 256 byte, vart JSON-paket ar storre
    mqttClient.setSocketTimeout(5);  // sekunder - sa att en dod broker inte lasar loopen
}

void maintainNetwork() {
    if (WiFi.status() == WL_CONNECTED) {
        misslyckadeWifi = 0;
        return;
<<<<<<< HEAD
    }

    unsigned long nu = millis();
    if (nu - sistaWifiForsok < WIFI_RETRY_MS) return;
    sistaWifiForsok = nu;

    Serial.println("WiFi nere - forsoker ateransluta.");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    if (++misslyckadeWifi >= 2) startaAccesspunkt();
}

// ---------------------------------------------------------------------------
// MQTT
// ---------------------------------------------------------------------------
static bool anslutMQTT() {
    if (WiFi.status() != WL_CONNECTED) return false;

    Serial.print("Forsoker ansluta till MQTT...");
    String clientId = String(DEVICE_ID) + "-" + String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_KEY)) {
        Serial.println(" ansluten!");
        return true;
    }
    Serial.print(" misslyckades, rc=");
    Serial.println(mqttClient.state());
    return false;
}

void maintainMQTT() {
    unsigned long nu = millis();

    if (!mqttClient.connected()) {
        if (nu - sistaMqttForsok > MQTT_RETRY_MS) {
            sistaMqttForsok = nu;
            anslutMQTT();
        }
        return;
    }

    mqttClient.loop();

    // Tom bufferten i lugn takt: ett meddelande per FLUSH_INTERVALL_MS.
    // Skalet ar att Adafruit IO (gratisnivan) tillater 30 datapunkter per minut.
    // En burst pa hundratals meddelanden skulle bli strypt eller slanga ut oss.
    if (bufferAntal() > 0 && nu - sistaFlush >= FLUSH_INTERVALL_MS) {
        sistaFlush = nu;
        Matning m;
        if (bufferPeek(m)) {
            if (publiceraMatning(m, true)) {
                bufferPop();   // tas bort forst NAR den ar bekraftat skickad
                Serial.print("Buffert tommd, kvar: ");
                Serial.println(bufferAntal());
            }
        }
=======
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    }

    unsigned long nu = millis();
    if (nu - sistaWifiForsok < WIFI_RETRY_MS) return;
    sistaWifiForsok = nu;

    Serial.println("WiFi nere - forsoker ateransluta.");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    if (++misslyckadeWifi >= 2) startaAccesspunkt();
}

// ---------------------------------------------------------------------------
// MQTT
// ---------------------------------------------------------------------------
static bool anslutMQTT() {
    if (WiFi.status() != WL_CONNECTED) return false;

    Serial.print("Forsoker ansluta till MQTT...");
    String clientId = String(DEVICE_ID) + "-" + String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_KEY)) {
        Serial.println(" ansluten!");
        return true;
    }
    Serial.print(" misslyckades, rc=");
    Serial.println(mqttClient.state());
    return false;
}

<<<<<<< HEAD
// ---------------------------------------------------------------------------
// JSON-paketering
// ---------------------------------------------------------------------------
static void laggTillVarde(JsonObject& obj, JsonArray& fel, const char* namn, float v) {
    if (arGiltigt(v)) {
        obj[namn] = serialized(String(v, 1));  // en decimal racker for sensorernas noggrannhet
    } else {
        obj[namn] = nullptr;                   // JSON null = matning saknas
        fel.add(namn);
    }
}

bool publiceraMatning(const Matning& m, bool franBuffert) {
    if (!mqttClient.connected()) return false;

    JsonDocument doc;
    doc["device_id"] = DEVICE_ID;
    doc["seq"]       = m.seq;
    doc["ts"]        = m.epoch;      // 0 = klockan inte synkad
    doc["uptime_s"]  = m.uptime_s;
    doc["buffrad"]   = franBuffert;  // true = vardet ar gammalt och skickas i efterhand

    JsonObject matvarden = doc["matvarden"].to<JsonObject>();
    JsonArray  fel       = doc["fel"].to<JsonArray>();

    laggTillVarde(matvarden, fel, "temp_inne_c",   m.tempInne);
    laggTillVarde(matvarden, fel, "fukt_inne_rh",  m.fuktInne);
    laggTillVarde(matvarden, fel, "temp_ute_c",    m.tempUte);
    laggTillVarde(matvarden, fel, "temp_vatten_c", m.tempVatten);

    char payload[400];
    size_t n = serializeJson(doc, payload, sizeof(payload));

    bool ok = mqttClient.publish(MQTT_TOPIC_DATA, (const uint8_t*)payload, n, false);
    if (ok) {
        skickade++;
        Serial.print(franBuffert ? "Skickade (buffrad): " : "Skickade: ");
        Serial.println(payload);
    } else {
        Serial.println("Publicering misslyckades - vardet buffras.");
        return false;
    }
=======
void maintainMQTT() {
    if (mqttClient.connected()) {
        mqttClient.loop();   // haller anslutningen vid liv
        return;
    }

    // Ateranslutning sker tidsstyrt och utan while-loop, sa att sensorlasning
    // och den lokala dashboarden fortsatter fungera under hela avbrottet.
    unsigned long nu = millis();
    if (nu - sistaMqttForsok > MQTT_RETRY_MS) {
        sistaMqttForsok = nu;
        anslutMQTT();
    }
}

// ---------------------------------------------------------------------------
// JSON-paketering
// ---------------------------------------------------------------------------
static void laggTillVarde(JsonObject& obj, JsonArray& fel, const char* namn, float v) {
    if (arGiltigt(v)) {
        obj[namn] = serialized(String(v, 1));  // en decimal racker for sensorernas noggrannhet
    } else {
        obj[namn] = nullptr;                   // JSON null = matning saknas
        fel.add(namn);
    }
}

bool publiceraMatning(const Matning& m) {
    if (!mqttClient.connected()) {
        ejSkickade++;
        return false;
    }

    JsonDocument doc;
    doc["device_id"] = DEVICE_ID;
    doc["seq"]       = m.seq;
    doc["ts"]        = m.epoch;      // 0 = klockan inte synkad
    doc["uptime_s"]  = m.uptime_s;

    JsonObject matvarden = doc["matvarden"].to<JsonObject>();
    JsonArray  fel       = doc["fel"].to<JsonArray>();

    laggTillVarde(matvarden, fel, "temp_inne_c",   m.tempInne);
    laggTillVarde(matvarden, fel, "fukt_inne_rh",  m.fuktInne);
    laggTillVarde(matvarden, fel, "temp_ute_c",    m.tempUte);
    laggTillVarde(matvarden, fel, "temp_vatten_c", m.tempVatten);

    char payload[400];
    size_t n = serializeJson(doc, payload, sizeof(payload));

    if (!mqttClient.publish(MQTT_TOPIC_DATA, (const uint8_t*)payload, n, false)) {
        ejSkickade++;
        Serial.println("Publicering misslyckades.");
        return false;
    }

    skickade++;
    Serial.print("Skickade: ");
    Serial.println(payload);
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

#if PUBLICERA_ENSKILDA_FEEDS
    // Adafruit IO ritar grafer per feed. Vi behaller darfor de enskilda feedsen
    // vid sidan av JSON-paketet sa att molndashboarden fortsatter fungera.
    if (arGiltigt(m.tempInne))   { char b[10]; dtostrf(m.tempInne,   4, 1, b); mqttClient.publish(FEED_LUFT_INNE, b); }
    if (arGiltigt(m.fuktInne))   { char b[10]; dtostrf(m.fuktInne,   4, 1, b); mqttClient.publish(FEED_FUKT_INNE, b); }
    if (arGiltigt(m.tempUte))    { char b[10]; dtostrf(m.tempUte,    4, 1, b); mqttClient.publish(FEED_LUFT_UTE,  b); }
    if (arGiltigt(m.tempVatten)) { char b[10]; dtostrf(m.tempVatten, 4, 1, b); mqttClient.publish(FEED_VATTEN,    b); }
#endif

    return true;
}

// ---------------------------------------------------------------------------
// Status
// ---------------------------------------------------------------------------
<<<<<<< HEAD
bool isWiFiConnected()   { return WiFi.status() == WL_CONNECTED; }
bool isAPMode()          { return apAktiv; }
bool isMQTTConnected()   { return mqttClient.connected(); }
uint32_t antalSkickade() { return skickade; }
=======
bool isWiFiConnected()     { return WiFi.status() == WL_CONNECTED; }
bool isAPMode()            { return apAktiv; }
bool isMQTTConnected()     { return mqttClient.connected(); }
uint32_t antalSkickade()   { return skickade; }
uint32_t antalEjSkickade() { return ejSkickade; }
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

String aktuellIP() {
    if (WiFi.status() == WL_CONNECTED) return WiFi.localIP().toString();
    if (apAktiv) return WiFi.softAPIP().toString();
    return String("-");
}

uint32_t aktuellEpoch() {
    time_t nu = time(nullptr);
    // Allt fore 2023 betyder att NTP inte hunnit svara an.
    return (nu > 1700000000) ? (uint32_t)nu : 0;
}
