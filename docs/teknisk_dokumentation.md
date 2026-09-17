# Teknisk Dokumentation - MicroHydros

## Programmets huvudsakliga struktur

Källkoden är skriven i C++ med Arduino-ramverket via PlatformIO och är strikt uppdelad i moduler (header- och källkodsfiler). Detta skapar en tydlig separation av ansvarsområden (separation of concerns), vilket gör koden enkel att underhålla och vidareutveckla.

Programmet är uppbyggt kring följande huvudmoduler:

* **`main.cpp` (Huvudloop):** Orkestrerar systemet genom en "non-blocking" design. Istället för att använda `delay()` styrs mätintervallen (15 sekunder) av `millis()`. Loopen ser till att nätverket underhålls, sensorerna läses av, den lokala servern uppdateras alltid, och därefter görs ett försök att publicera till molnet.
* **`Matning.h` (Datamodell):** Definierar en gemensam `struct Matning` som hela systemet använder. Genom att ha en (1) central representation av mätdata undviks konverteringsfel mellan olika moduler.
* **`Sensors.cpp` / `Sensors.h` (Hårdvarulager):** Hanterar I2C-kommunikation för två SHT31-sensorer (inne/ute) samt 1-Wire för DS18B20 (vatten). Modulen hanterar även grundläggande felhantering; om en sensor är urkopplad eller ger orimliga värden returneras konstanten `FEL_VARDE` (-999.0).
* **`Cloud.cpp` / `Cloud.h` (Nätverk & Moln):** Hanterar systemets anslutning till omvärlden. Om Wi-Fi-anslutningen (Station mode) misslyckas efter två försök, startar ESP32-enheten automatiskt en lokal Accesspunkt (`WIFI_AP_STA`). Modulen ansvarar även för att ansluta till Adafruit IO via MQTT, hämta exakt tid via NTP, samt strukturera mätvärdena till ett JSON-dokument.
* **`LocalServer.cpp` / `web_page.h` (Lokal Edge-Dashboard):** Driver en inbyggd webbserver (`WebServer` på port 80). Hela webbsidans gränssnitt (HTML, CSS, JS) lagras extremt minneseffektivt i ESP32:ans flashminne (via `PROGMEM` och `rawliteral`). Detta eliminerar beroenden av externa filer eller CDN, vilket garanterar att gränssnittet fungerar helt offline.
* **`config.h`:** Samlar alla makron, nätverksuppgifter (SSID/lösenord) och API-nycklar. Denna fil är exkluderad från versionshanteringen för informationssäkerhet.

---

## Hur mätdata representeras och hanteras

Dataflödet i MicroHydros är designat för hög tillförlitlighet och tydlighet. Hanteringen av en mätning från sensor till slutanvändare sker i följande steg:

### 1. Inläsning och representation (`struct Matning`)
När sensorerna läses av paketeras all information i objektet `Matning`. Detta objekt innehåller:
* Temperatur och luftfuktighet från de olika zonerna, representerade som decimaltal (`float`).
* Ett löpnummer (`seq`) som inkrementeras vid varje avläsning, vilket gör det möjligt att spåra tappade paket.
* Tidsstämplar i form av upptid (`uptime_s`) och UNIX-tid (`epoch`) via NTP-synkronisering.

### 2. Lokal hantering och offline-vy (Prioriterad data)
Den lokala vyn uppdateras alltid först och är oberoende av internetanslutningen.
* När en ny `Matning` skapas, skickas den direkt till `LocalServer` där den sparas i en ringbuffert i RAM-minnet (`historiken`) som rymmer de 20 senaste mätningarna. 
* När en användare besöker dashboarden, anropar gränssnittet API-endpointen `/api/status`. Datan serialiseras då med hjälp av `ArduinoJson` till ett strukturerat format och skickas till webbläsaren.

### 3. Molnpublicering och frånvaro av buffert (Edge Case Hantering)
Arkitekturen bygger på beslutet att den lokala miljön är den primära källan till sanning vid nätverksavbrott.
* Om systemet har en aktiv MQTT-anslutning byggs ett omfattande JSON-dokument upp i `Cloud.cpp`. Alla giltiga mätvärden formateras med en (1) decimal via C-funktionen `dtostrf()` och publiceras till MQTT-brokern. För bakåtkompatibilitet uppdateras även enskilda feeds.
* **Hantering av nätverksavbrott:** Om brokern är onåbar returnerar publiceringsfunktionen `false`. I denna arkitektur sparas *inte* mätningen för senare uppladdning till molnet, och inget skickas i efterhand. Mätningen registreras enbart som "missad sändning" (`ejSkickade`) och visas i den lokala dashboarden. Detta förhindrar minnesöverbelastning och Rate Limit-blockeringar hos Adafruit IO när nätverket återvänder.

## Komponenter och Datablad

För att underlätta framtida underhåll, felsökning och vidareutveckling av hårdvaran, listas nedan de officiella databladen för systemets huvudkomponenter:

* **ESP32 (Mikrokontroller)**
  Huvudenheten från Espressif Systems som driver systemet, hanterar nätverksuppkoppling (Wi-Fi/Accesspunkt) och exekverar den kompilerade C++ koden. 
  [ESP32 Series Datasheet (PDF)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

* **SHT31-D (Luftsensor)**
  Högprecisionssensor från Sensirion som mäter både lufttemperatur och relativ luftfuktighet. I detta projekt används två stycken som kommunicerar via I2C-protokollet på samma databuss (adresser `0x44` och `0x45`).
  [Sensirion SHT3x-DIS Datasheet (PDF)](https://sensirion.com/media/documents/213E6A3B/63A5A569/Datasheet_SHT3x_DIS.pdf)

* **DS18B20 (Vattensensor)**
  Kapslad och vattentät temperatursensor från Maxim Integrated (numera Analog Devices). Placeras direkt i näringslösningen och kommunicerar med mikrokontrollern via 1-Wire-protokollet.
  [Maxim DS18B20 Datasheet (PDF)](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf)