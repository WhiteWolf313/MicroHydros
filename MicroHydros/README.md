# MicroHydros - Prototyp 1

## Projektets syfte

MicroHydros är en IoT-baserad prototyp för övervakning av en hydroponisk odlingsmiljö. Systemet samlar regelbundet in temperatur och luftfuktighet och skickar mätdata via MQTT till Adafruit IO.

Projektet är utvecklat på uppdrag av HydroGreen Fingers AB.

## Mätningar

Systemet mäter fyra mätvärden:

- Lufttemperatur inne
- Luftfuktighet inne
- Utetemperatur
- Vattentemperatur i näringslösningen

### Sensorer

- 2 × SHT31-D
  - SHT31-D inne: lufttemperatur och luftfuktighet
  - SHT31-D ute: utetemperatur
- 1 × DS18B20: vattentemperatur
- ESP32-S3 som mikrokontroller
- 4,7 kΩ motstånd för DS18B20

DHT11 och DHT22 används inte i prototypen.

## Systemarkitektur

![Arkitekturdiagram](docs/arkitektur.png)

Systemets huvuddelar är:

**Sensorer → ESP32-S3 → Wi-Fi → MQTT → Adafruit IO**

ESP32-S3 har även en inbyggd lokal webbserver som visar mätdata i en lokal dashboard.

**ESP32-S3 → Lokal dashboard**

Den lokala dashboarden är direkt tillgänglig från ESP32-S3 och är inte beroende av internet. Den visar aktuell mätdata, lokal historik och status för systemet.

## Lokal dashboard

ESP32-S3 fungerar även som en lokal webbserver.

Dashboarden visar:

- Lufttemperatur inne
- Luftfuktighet inne
- Utetemperatur
- Vattentemperatur
- Senaste mätningar
- Wi-Fi-status
- MQTT-status
- Antal skickade mätningar
- Uptime
- Mätningens sekvensnummer

När ESP32-S3 är ansluten till det vanliga Wi-Fi-nätverket öppnas dashboarden genom ESP32-S3:s IP-adress.

Om Wi-Fi-anslutningen inte fungerar kan ESP32-S3 starta ett eget access point. Dashboarden kan då användas lokalt utan internet.

## Datainsamling

ESP32-S3 läser av sensorerna med ett konfigurerat mätintervall.

Varje mätning får:

- Sekvensnummer
- Tidsstämpel
- Uptime
- Lufttemperatur inne
- Luftfuktighet inne
- Utetemperatur
- Vattentemperatur

Mätningen samlas i en gemensam datastruktur och skickas som JSON.

Exempel:

```json
{
  "device_id": "MicroHydros-01",
  "seq": 1,
  "ts": 0,
  "uptime_s": 15,
  "matvarden": {
    "temp_inne_c": 22.4,
    "fukt_inne_rh": 45.2,
    "temp_ute_c": 18.7,
    "temp_vatten_c": 21.1
  }
}

