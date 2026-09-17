# MicroHydros - Prototyp 1

## Projektets syfte

Denna prototyp samlar in miljödata från en hydroponisk odlingsmiljö och skickar mätdata via MQTT till en extern tjänst (Adafruit IO) för övervakning och lagring över tid[cite: 23]. Projektet är utvecklat på uppdrag av HydroGreen Fingers AB[cite: 23].

För att möta höga krav på tillförlitlighet har systemet designats med en fallback-mekanism. Om internetanslutningen bryts startar enheten automatiskt ett eget lokalt Wi-Fi-nätverk och serverar en inbyggd webb-dashboard där mätningarna kan övervakas i realtid lokalt.

**Systemet mäter:**
- Lufttemperatur inne[cite: 23]
- Luftfuktighet inne[cite: 23]
- Lufttemperatur ute[cite: 23]
- Temperatur i vattnet (näringslösningen)[cite: 23]

## Arkitekturdiagram

![Arkitekturdiagram](docs/arkitektur.png)

## Nödvändiga beroenden

### Hårdvara
- ESP32-S3 (Mikrokontroller)[cite: 23]
- 2 × SHT31-D (Luftsensorer via I2C)[cite: 23]
- 1 × DS18B20 (Vattensensor via 1-Wire)[cite: 23]
- 4.7 kΩ motstånd (Pull-up för DS18B20)[cite: 23]

### Mjukvara & Utvecklingsmiljö
- Visual Studio Code med tillägget **PlatformIO**[cite: 23].
- C/C++ med Arduino-ramverket[cite: 23].
- Externa bibliotek (hanteras automatiskt av `platformio.ini`): `ArduinoJson`, `PubSubClient`, `Adafruit SHT31 Library`, `DallasTemperature`.

### Molntjänst
- Adafruit IO-konto för mottagande av MQTT-data[cite: 23].

## Hur systemet byggs eller startas

1. **Klona repositoryt:**
   `git clone https://github.com/WhiteWolf313/MicroHydros.git`[cite: 23]

2. **Öppna projektet:** 
   Öppna mappen i VS Code och vänta på att PlatformIO initierar projektet och laddar ner beroenden[cite: 23].

3. **Konfigurera inloggningsuppgifter (VIKTIGT):**
   * Kopiera filen `config_2.h` (eller motsvarande mall) och döp om den till `config.h`. 
   * Placera den i mappen `include/`. 
   * Fyll i dina egna uppgifter för Wi-Fi (SSID/Lösenord) och MQTT/Adafruit IO. *(Filen `config.h` ignoreras av Git av säkerhetsskäl).*

4. **Kompilera och ladda upp:**
   * Anslut ESP32-S3 till datorn[cite: 23].
   * Klicka på **Build** (bocken) i PlatformIO för att kompilera[cite: 23].
   * Klicka på **Upload** (pilen) för att flasha enheten[cite: 23].

## Grundläggande användning

När systemet är igång arbetar enheten autonomt och läser av sensorerna var 15:e sekund.

* **Online-läge:** Systemet ansluter till det lokala nätverket, synkroniserar klockan via NTP och publicerar ett strukturerat JSON-paket med mätdata till Adafruit IO via MQTT[cite: 23]. Datan kan sedan följas över tid i en Adafruit IO Dashboard[cite: 23].
* **Offline-läge (Lokal Dashboard):** Om huvudroutern förlorar anslutningen (eller stängs av), skapar ESP32-enheten automatiskt en egen Accesspunkt med namnet `MicroHydros-01`. 
   * Anslut en mobil eller dator till detta Wi-Fi.
   * Surfa till `http://192.168.4.1` i webbläsaren.
   * Här visas en lokal, inbyggd dashboard med de senaste 20 mätningarna. Denna dashboard laddas blixtsnabbt direkt från enhetens flashminne (PROGMEM).

## Projektstruktur

- `src/` – projektets C/C++-källkod uppdelad i ansvarsområden (Sensorer, Nätverk, Lokal Server)[cite: 23].
- `include/` – konfigurationsfiler (bl.a. `config.h`) och headerfiler[cite: 23]. Här finns även `web_page.h` som innehåller den lokala webbplatsen.
- `docs/` – projektets tekniska dokumentation, testprotokoll och arkitekturdiagram[cite: 23].
- `platformio.ini` – PlatformIO-konfiguration som definierar beroenden och miljö[cite: 23].