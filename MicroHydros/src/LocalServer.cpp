#include "LocalServer.h"
<<<<<<< HEAD
#include "Buffer.h"
=======
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
#include "Cloud.h"
#include "web_page.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.h"

// WebServer ingar i ESP32-karnan. Vi valde den synkrona varianten framfor
// ESPAsyncWebServer eftersom sidan ar liten, hogst nagra anvandare ar
// anslutna samtidigt och koden blir lattare att folja for en ny utvecklare.
static WebServer server(80);

<<<<<<< HEAD
static Matning senaste;
static bool harMatning = false;
=======
// Visningshistorik i RAM. Ringbuffert: nar den ar full skrivs det aldsta
// vardet over. Datan finns bara sa lange enheten ar igang.
static Matning historiken[HISTORIK_LANGD];
static size_t  histHuvud = 0;
static size_t  histAntal = 0;

static bool harMatning = false;
static Matning senaste;
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

// Samma JSON-namn anvands har som i MQTT-meddelandet, sa att bade molnet och
// den lokala dashboarden laser matdata i exakt samma format.
static void laggTillVarde(JsonObject obj, const char* namn, float v) {
    if (arGiltigt(v)) {
        obj[namn] = serialized(String(v, 1));
    } else {
        obj[namn] = nullptr;
    }
}

static void fyllMatning(JsonObject obj, const Matning& m) {
    obj["seq"]      = m.seq;
    obj["ts"]       = m.epoch;
    obj["uptime_s"] = m.uptime_s;
    laggTillVarde(obj, "temp_inne_c",   m.tempInne);
    laggTillVarde(obj, "fukt_inne_rh",  m.fuktInne);
    laggTillVarde(obj, "temp_ute_c",    m.tempUte);
    laggTillVarde(obj, "temp_vatten_c", m.tempVatten);
}

static void handleRoot() {
    server.send_P(200, "text/html; charset=utf-8", DASHBOARD_HTML);
}

static void handleStatus() {
    JsonDocument doc;
    doc["device_id"] = DEVICE_ID;
    doc["uptime_s"]  = millis() / 1000;

    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ansluten"] = isWiFiConnected();
    wifi["ap_lage"]  = isAPMode();
    wifi["ip"]       = aktuellIP();
    wifi["rssi"]     = isWiFiConnected() ? WiFi.RSSI() : 0;

    JsonObject mqtt = doc["mqtt"].to<JsonObject>();
<<<<<<< HEAD
    mqtt["ansluten"] = isMQTTConnected();
    mqtt["skickade"] = antalSkickade();

    JsonObject buf = doc["buffert"].to<JsonObject>();
    buf["antal"]      = bufferAntal();
    buf["kapacitet"]  = bufferKapacitet();
    buf["tappat"]     = bufferHarTappatVarden();
=======
    mqtt["ansluten"]    = isMQTTConnected();
    mqtt["skickade"]    = antalSkickade();
    mqtt["ej_skickade"] = antalEjSkickade();

    JsonObject lokal = doc["lokal_historik"].to<JsonObject>();
    lokal["antal"]     = histAntal;
    lokal["kapacitet"] = (uint32_t)HISTORIK_LANGD;
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961

    if (harMatning) {
        fyllMatning(doc["senaste"].to<JsonObject>(), senaste);
    } else {
        doc["senaste"] = nullptr;
    }

    JsonArray hist = doc["historik"].to<JsonArray>();
<<<<<<< HEAD
    Matning m;
    for (size_t i = 0; i < historikAntal(); i++) {
        if (historikGet(i, m)) fyllMatning(hist.add<JsonObject>(), m);
=======
    size_t start = (histHuvud + HISTORIK_LANGD - histAntal) % HISTORIK_LANGD;
    for (size_t i = 0; i < histAntal; i++) {
        fyllMatning(hist.add<JsonObject>(), historiken[(start + i) % HISTORIK_LANGD]);
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
    }

    String ut;
    serializeJson(doc, ut);
    server.send(200, "application/json", ut);
}

static void handleNotFound() {
    server.send(404, "text/plain", "Sidan finns inte. Oppna / for dashboarden.");
}

void setupLocalServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api/status", HTTP_GET, handleStatus);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.print("Lokal dashboard: http://");
    Serial.println(aktuellIP());
}

void handleLocalServer() {
    server.handleClient();
}

<<<<<<< HEAD
void serverSetSenaste(const Matning& m) {
    senaste = m;
    harMatning = true;
}
=======
void serverNyMatning(const Matning& m) {
    senaste = m;
    harMatning = true;

    historiken[histHuvud] = m;
    histHuvud = (histHuvud + 1) % HISTORIK_LANGD;
    if (histAntal < HISTORIK_LANGD) histAntal++;
}
>>>>>>> ba1165bbac4cb8fc0699ba6b99d6a62ee683a961
