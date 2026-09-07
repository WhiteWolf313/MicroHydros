# MicroHydros - Teknisk Prototyp (C++ / PlatformIO)

## Projektets syfte
Denna prototyp är framtagen på uppdrag av HydroGreen Fingers AB för att övervaka klimatet i småskaliga hydroponiska odlingssystem[cite: 1]. Systemet mäter kontinuerligt lufttemperatur, luftfuktighet och vattentemperatur och skickar dessa mätdata via MQTT till en molntjänst (Adafruit IO) för framtida analys[cite: 1, 2].

## Nödvändiga beroenden
För att arbeta med och köra detta projekt krävs följande:

* **Hårdvara:** ESP32 mikrokontroller, 2 st SHT31-sensorer (I2C), 1 st vattentät DS18B20-sensor (1-Wire) samt ett 4.7kΩ motstånd[cite: 3].
* **Utvecklingsmiljö:** Visual Studio Code med tillägget **PlatformIO**.
* **Mjukvarubibliotek:** Hanteras helt automatiskt! Alla externa C++ bibliotek (t.ex. `PubSubClient` för MQTT och `Adafruit_SHT31`) är definierade i filen `platformio.ini` och laddas ner av PlatformIO vid första kompileringen[cite: 2].
* **Molntjänst:** Ett aktivt Adafruit IO-konto med uppsatta "Feeds" för mätpunkterna[cite: 2].

## Hur systemet byggs eller startas
Följ dessa steg för att kompilera koden och starta systemet på er lokala ESP32-enhet[cite: 2]:

1. **Klona projektet:** Ladda ner källkoden via Git (`git clone https://github.com/WhiteWolf313/MicroHydros.git`) och öppna mappen i Visual Studio Code[cite: 2].
2. **Skapa konfigurationsfilen:** 
   * Gå till mappen `include/`.
   * Skapa en ny fil som heter `config.h`. 
   * Kopiera strukturen från (eventuell) `config_template.h` och fyll i era egna uppgifter för Wi-Fi (SSID/Lösenord) samt era unika API-nycklar för Adafruit IO. *(Obs: `config.h` är exkluderad i `.gitignore` av säkerhetsskäl)*[cite: 3].
3. **Bygg (Kompilera):** Klicka på **Bock-ikonen** (Build) i PlatformIO:s nedre statusrad. PlatformIO kommer nu automatiskt ladda ner alla nödvändiga bibliotek och kompilera C++ koden[cite: 2].
4. **Flasha (Ladda upp):** Anslut din ESP32 via USB och klicka på **Pil-ikonen** (Upload) i statusraden för att föra över programmet till mikrokontrollern[cite: 2].

## Grundläggande användning
När systemet har flashats och startats om krävs ingen manuell interaktion från användaren[cite: 2].
1. Enheten ansluter automatiskt till det konfigurerade Wi-Fi-nätverket och därefter till Adafruit IO:s MQTT-broker[cite: 3].
2. Sensorerna läses av kontinuerligt (var 10:e sekund).
3. Mjukvaran filtrerar automatiskt bort uppenbart felaktiga mätvärden (t.ex. vid glapp i kablar)[cite: 1, 2].
4. Godkänd mätdata publiceras direkt till era Adafruit IO Feeds där de kan övervakas via en webb-dashboard[cite: 3].