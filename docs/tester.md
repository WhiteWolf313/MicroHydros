# Testdokumentation - MicroHydros

## 1. Teststrategi
För att säkerställa att prototypen av MicroHydros uppfyller kraven från HydroGreen Fingers AB har systemet testats på tre nivåer:
1. **Komponenttest:** Verifiering av att enskilda sensorer levererar förväntad data.
2. **Integrationstest:** Verifiering av nätverksfunktioner (Wi-Fi, MQTT och lokal webbserver).
3. **Systemtest (Edge Cases):** Verifiering av systemets felhantering och robusthet vid hårdvarufel och nätverksavbrott.

---

## 2. Testfall och Resultat

### 2.1 Komponenttester (Sensorer)

| Test-ID | Vad som testades | Hur testet genomfördes | Förväntat resultat | Faktiskt resultat | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **K01** | Avläsning: SHT31 (Inne/Ute) | Systemet startades i rumstemperatur. En sensor värmdes lätt med handen. | Båda sensorerna initieras på I2C-bussarna (0x44). Handvärmd sensor ska visa ökande temperatur. | Sensor INNE visade 22.1°C / 45% RH. Sensor UTE (värmd) steg till 28.4°C. | Godkänt |
| **K02** | Avläsning: DS18B20 (Vatten) | Vattensensorn placerades i ett glas med kallt kranvatten. | Sensorn initieras via 1-Wire och visar en temperatur lägre än rumstemperaturen. | Sensorn läste framgångsrikt av 14.3°C. | Godkänt |
| **K03** | Jämförelse SHT31 inne/ute | Båda sensorerna placerades bredvid varandra i 10 minuter. | Skillnad inom sensorernas noggrannhet (ca ±0,3 °C / ±2 %RH enligt datablad). | stämmer | Godkänd |
| **K04** | Jämförelse mot referens | DS18B20 och SHT31 jämfördes med en referenstermometer. | Skillnad inom ca ±0,5 °C. | 0,4 | Godkänd |

### 2.2 Integrationstester (Nätverk & Moln)

| Test-ID | Vad som testades | Hur testet genomfördes | Förväntat resultat | Faktiskt resultat | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **I01** | Wi-Fi och tidssynkronisering | Enheten startas med korrekta Wi-Fi-uppgifter i `config.h`. | Ansluter till nätverket och hämtar korrekt UNIX-tid (epoch) via NTP-servern. | Anslöt på 2 sekunder. Fick korrekt tidsstämpel (t.ex. 1700000000+) för mätningarna. | Godkänt |
| **I02** | MQTT Publicering (Adafruit IO) | Verifiering av dataström i Adafruit IO dashboard. | Enheten paketerar data som JSON och skickar till brokern var 15:e sekund. Graferna uppdateras. | JSON-payload mottogs korrekt av Adafruit IO. Enskilda feeds (t.ex. `temp-inne`) uppdaterades. | Godkänt |
| **I03** | Lokal Webbserver (Dashboard) | Surfa till enhetens lokala IP-adress via en dator på samma nätverk. | Den inbyggda webbsidan laddas snabbt (från PROGMEM) och visar live-data som uppdateras via JavaScript. | Sidan laddades direkt. Live-uppdatering av mätvärden och upptid fungerade felfritt. | Godkänt |

### 2.3 Systemtester och Felhantering (Edge Cases)

| Test-ID | Vad som testades | Hur testet genomfördes | Förväntat resultat | Faktiskt resultat | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **S01** | Bortkopplad sensor under drift | Datakabeln till DS18B20-sensorn drogs ur under pågående körning. | Systemet ska upptäcka felet, kasta mätningen, och inte skicka skräpdata till molnet. Loopen får inte krascha. | Funktionen returnerade felkoden `-127.0` som filtrerades till `-999.0` (`FEL_VARDE`). Den lokala vyn visade "Ingen avläsning". Inget orimligt värde skickades till Adafruit IO. | Godkänt |
| **S02** | Nätverksavbrott och Fallback (AP-läge) | Huvudroutern stängdes av under drift. | Enheten misslyckas med MQTT. Efter två Wi-Fi-försök startas den lokala Accesspunkten ("MicroHydros-01"). MQTT-meddelanden hoppas över (ejSkickade ökar). | Accesspunkten dök upp i mobilens Wi-Fi-lista. Den lokala dashboarden nåddes på 192.168.4.1. `ejSkickade` räknade upp, men mätningarna fortgick lokalt. | Godkänt |
| **S03** | Återställning efter nätverksavbrott | Huvudroutern slogs på igen efter test S02. | Systemet återansluter till Wi-Fi och MQTT automatiskt utan omstart och återupptar publiceringen. | Återanslöt automatiskt efter ca 20 sekunder. Nya mätvärden dök direkt upp på Adafruit IO igen. | Godkänt |
| **S04** | Långtidsstabilitet | Systemet kördes oavbrutet i *10* timmar. | Inga omstarter, `seq` ökar utan luckor, `ejSkickade` = 0 vid stabilt nätverk. | ingen fel | Godkänd |

---

## 3. Resonemang kring mätvärdenas rimlighet

För att bygga ett tillförlitligt system för HydroGreen Fingers AB krävs det att systemet kan urskilja verkliga klimatförändringar från hårdvarufel. Därför har vi implementerat strikta rimlighetskontroller i vår mjukvara (`Matning.h` och `Sensors.cpp`).

### 3.1 Fysiska begränsningar i odlingsmiljön
Ett hydroponiskt system är en kontrollerad inomhusmiljö.
* **Vattentemperatur:** Vi har definierat ett giltigt spann för näringslösningen mellan **-10°C och 50°C**. En realistisk näringslösning ligger normalt runt 15–25°C, och redan över ca 30°C tar växtrötterna skada. Spannet är därför medvetet bredare än det som är bra för växterna: syftet med kontrollen är att filtrera bort **mätfel**, inte att bedöma om miljön är bra. Ett verkligt men onormalt värde, t.ex. 35°C, ska rapporteras så att användaren kan reagera. Skulle sensorn däremot rapportera t.ex. 75°C är det med största sannolikhet ett mätfel, varför det kastas.
* 
* **Luftfuktighet:** Enligt fysikens lagar kan relativ luftfuktighet (RH) endast vara mellan **0 % och 100 %**. Värden utanför detta (exempelvis om SHT31-sensorn kortsluts av kondens och rapporterar `NaN` eller över 100%) filtreras bort direkt.

### 3.2 Hårdvaruspecifika felkoder
Vi har designat felhanteringen för att känna igen de specifika felkoder som protokollen och sensorerna genererar vid elektriska problem:
* **DS18B20 (1-Wire):** Om datapinnen tappar kontakten men sensorn fortfarande har ström, returneras ofta värdet `85.0°C` (vilket är sensorns power-on reset state). Om sensorn är helt urkopplad returnerar DallasTemperature-biblioteket `-127.0°C`. Vår kod identifierar uttryckligen dessa två värden och konverterar dem till vår centrala felkod `FEL_VARDE` (-999.0).
* **SHT31 (I2C):** Vid kommunikationsfel på I2C-bussen returnerar biblioteket ofta `NaN` (Not a Number). Vår kontroll `!isnan(temp)` ser till att systemet aldrig försöker publicera korrupt data till molnet eller kraschar JSON-parsningen.

### 3.3 Slutsats av datakvalitet
Genom vår arkitektur (där felaktiga värden filtreras tidigt vid sensorn, aldrig lagras i `Matning.h`, och därmed aldrig når MQTT-brokern) garanterar vi att kundens framtida historiska dataanalys baseras på ren och tillförlitlig data.
