# Projektplan – MicroHydros

**Projekt:** MicroHydros  
**Kund:** HydroGreen Fingers AB  
**Utbildning:** IoT och inbyggda system  
**Projektperiod:** 31 augusti – 25 september 2026  
**Grupp:** Yousif, Atosh och Star  

---

# 1. Vad är en projektplan?

Vår projektplan beskriver vad som ska utvecklas, varför och hur projektet ska genomföras. Målet är att utveckla en första teknisk IoT-prototyp för MicroHydros som kan samla in och följa mätdata från odlingsmiljön.

Vi har valt ESP32-S3 som mikrokontroller, C/C++ som programmeringsspråk och två SHT31-D samt en DS18B20 som sensorer. Inga aktuatorer ingår i den första prototypen.

Mätdata behandlas i ESP32-S3 och skickas via Wi-Fi och MQTT till Adafruit IO, där data kan lagras och visas i ett webbdashboard. Vi använder tidsstämplar och JSON för att strukturera mätdata.

Arbetet delas mellan gruppmedlemmarna med ansvar för bland annat sensorer, programmering, kommunikation, dokumentation och testning. Vi använder Trello för planering och GitHub för versionshantering.

Projektet genomförs enligt en tidsplan där utveckling, integration, testning och dokumentation genomförs stegvis. Systemet testas genom att kontrollera mätvärden, återkommande mätningar, datakommunikation och felhantering.

Viktiga risker är sensorfel, felaktiga mätvärden samt problem med Wi-Fi, MQTT eller Adafruit IO. Informationssäkerhet hanteras bland annat genom att inte lägga känsliga uppgifter eller lösenord direkt i koden.

---

# 2. Projektets bakgrund

MicroHydros är en kompakt hydroponisk odlingsenhet där temperatur och luftfuktighet kan förändras snabbt. HydroGreen Fingers AB behöver därför en mer tillförlitlig lösning för att mäta och följa odlingsmiljön över tid.

Den tidigare prototypen använde DHT11/DHT22, men dessa gav inte den tillförlitlighet som behövdes. Projektet ska därför ta fram en ny och stabil prototyp med bättre sensorer.

Liknande lösningar finns redan, men med hjälp av IoT och ett inbyggt system kan vi samla in mätdata, skicka den till en extern tjänst och spara historiken. Det gör det möjligt att följa odlingsmiljön och lägga grunden för framtida utveckling.

---

# 3. Syfte

Syftet med projektet är att utveckla en IoT-baserad prototyp för MicroHydros som kan övervaka temperatur och luftfuktighet i odlingsmiljön samt samla in och skicka mätdata för lagring och uppföljning.

Prototypen ska fokusera på en stabil och tillförlitlig grundlösning framför att implementera många funktioner samtidigt.

---

# 4. Projektmål

När projektet är färdigt ska systemet kunna:

- mäta temperatur inomhus
- mäta temperatur utomhus
- mäta temperaturen i vattnet
- mäta luftfuktighet inomhus
- genomföra mätningarna återkommande över tid
- behandla mätdata i ESP32-S3 med C/C++
- skapa strukturerade datapaket i JSON-format
- ansluta till Wi-Fi
- automatiskt försöka återansluta vid förlorad Wi-Fi-anslutning
- skicka mätdata via MQTT
- skicka mätdata till Adafruit IO
- lagra mätdata och historik i Adafruit IO
- visa mätdata i ett webbdashboard
- hantera felaktiga eller orimliga mätvärden
- hantera kommunikationsfel
- testa och verifiera att systemet fungerar som planerat.

---

# 5. Kravspecifikation

En kravspecifikation beskriver vilka krav systemet måste uppfylla. Kraven delas upp i funktionella och icke-funktionella krav.

## 5.1 Funktionella krav

Systemet ska kunna:

1. Mäta temperatur inomhus.
2. Mäta temperatur utomhus.
3. Mäta temperaturen i vattnet.
4. Mäta luftfuktighet inomhus.
5. Genomföra mätningarna återkommande över tid.
6. Ansluta till Wi-Fi.
7. Försöka återansluta om Wi-Fi-anslutningen bryts.
8. Skicka sensordata via MQTT.
9. Skicka data i ett strukturerat JSON-format.
10. Skicka mätdata till Adafruit IO.
11. Spara mätdata och historik i Adafruit IO.
12. Visa mätdata i ett webbdashboard.
13. Hantera felaktiga eller orimliga mätvärden.
14. Hantera kommunikationsfel, exempelvis om MQTT-anslutningen bryts.
15. Fortsätta mäta lokalt vid ett tillfälligt kommunikationsavbrott.

## 5.2 Icke-funktionella krav

Systemet ska:

1. Vara tillförlitligt och ge stabila mätvärden över tid.
2. Ha en rimlig svarstid vid mätning och överföring av data.
3. Vara energieffektivt så långt det är möjligt inom prototypens förutsättningar.
4. Vara säkert, exempelvis genom att inte lagra lösenord direkt i koden eller versionshanteringen.
5. Vara enkelt att använda och förstå.
6. Kunna fortsätta mäta även om internetanslutningen tillfälligt försvinner.
7. Vara enkelt att underhålla och bygga vidare på.
8. Ha en modulär struktur så att ytterligare funktioner eller sensorer kan läggas till senare.

---

# 6. Teknisk lösning

## 6.1 Hårdvara

Prototypen består av följande hårdvara:

- **Mikrokontroller:** ESP32-S3
- **Luftsensorer:** 2 st SHT31-D
- **Vattensensor:** 1 st DS18B20, vattentät sond
- **Motstånd:** 4,7 kΩ för DS18B20
- Kopplingsdäck
- Kablar
- USB-C-kabel/strömförsörjning

SHT31-D används för att mäta temperatur och relativ luftfuktighet. De två sensorerna används för att mäta temperatur på olika platser, exempelvis inomhus och utomhus.

DS18B20 används för att mäta temperaturen i vattnet/näringslösningen.

Inga aktuatorer ingår i den första prototypen.

## 6.2 Mjukvara

Systemet utvecklas i **C/C++** och byggs med **PlatformIO**.

Mjukvaran ansvarar för:

- initiering av sensorer
- avläsning av sensordata
- kontroll av mätvärden
- behandling av data
- skapande av JSON-data
- tidsstämpling av mätningar
- Wi-Fi-anslutning
- återanslutning vid kommunikationsproblem
- MQTT-kommunikation
- publicering av mätdata till Adafruit IO
- felhantering.

Projektets kod delas upp i separata delar för att göra systemet enklare att förstå, testa och underhålla.

## 6.3 Kommunikation

- **Nätverk:** Wi-Fi
- **Kommunikationsprotokoll:** MQTT
- **Dataformat:** JSON
- **Extern IoT-tjänst:** Adafruit IO
- **Tid:** NTP används för att hämta korrekt tid
- **Tidsformat:** ISO 8601/UTC används för tidsstämplar.

MQTT används eftersom protokollet är lättviktigt och passar bra för kommunikation mellan inbyggda system och IoT-tjänster.

---

# 7. Systemarkitektur

Systemets huvudsakliga dataflöde är:

**SHT31-D + SHT31-D + DS18B20**  
↓  
**ESP32-S3**  
↓  
**Wi-Fi**  
↓  
**MQTT**  
↓  
**Adafruit IO**  
↓  
**Webb-dashboard**

Sensorerna samlar in mätdata. ESP32-S3 behandlar och strukturerar informationen innan den skickas via Wi-Fi och MQTT.

Mätdata skickas som JSON och innehåller bland annat mätvärden och tidsstämpel.

Arkitekturdiagrammet finns i:

`docs/arkitektur.png`

![Arkitekturdiagram](arkitektur.png)

---

# 8. Projektorganisation

Projektet genomförs i ett agilt team om tre studerande.

Alla gruppmedlemmar ska delta i den tekniska utvecklingsprocessen och ansvaret fördelas gemensamt. Gruppen arbetar med bland annat:

- programmering
- sensorintegration
- kommunikation
- testning
- dokumentation
- Git och versionshantering.

Vi använder en Kanban-tavla i Trello för att planera och följa upp uppgifter.

GitHub används för versionshantering. Arbetet sker i separata branches och ändringar integreras genom commits och Pull Requests när det är lämpligt.

---

# 9. Tidsplan

Projektet sträcker sig över fyra veckor:

**31 augusti – 25 september 2026**

## Vecka 1

- Kravanalys
- Research
- Projektplanering
- Sensorval
- Val av teknisk lösning
- Arkitektur
- Första tekniska proof-of-concept

## Vecka 2

- Grundläggande implementation
- Konfiguration av ESP32-S3
- Integration av SHT31-D
- Integration av DS18B20
- Datainsamling
- Återkommande mätningar
- Tidsstämplar
- JSON-struktur

## Vecka 3

- Wi-Fi-kommunikation
- MQTT
- Integration mot Adafruit IO
- Felhantering
- Automatisk återanslutning
- Stabilisering
- Testning och förbättring

## Vecka 4

- Sluttest
- Stabilitetstest
- Dokumentation
- README
- Testresultat
- Demonstration
- Individuell reflektion

---

# 10. Riskanalys

| Risk | Sannolikhet | Konsekvens | Åtgärd |
|---|---|---|---|
| Sensorn ger orimliga eller felaktiga värden | Medel | Hög | Kontrollera mätvärden mot rimliga intervall och hantera sensorfel i mjukvaran. |
| Wi-Fi-anslutningen bryts | Hög | Medel | Systemet ska fortsätta mäta lokalt och automatiskt försöka återansluta. |
| MQTT-anslutningen bryts | Medel | Medel | Kontrollera anslutningen och försök återansluta innan nya data skickas. |
| Adafruit IO är tillfälligt otillgängligt | Medel | Medel | Fortsätt mäta lokalt och återuppta överföringen när kommunikationen fungerar igen. |
| Fel vid merge i Git | Medel | Medel | Arbeta i separata branches, gör tydliga commits och genomför Code Review vid Pull Requests. |
| Felaktig sensorinkoppling | Medel | Hög | Kontrollera kopplingsschema och testa varje sensor separat. |
| Problem med strömförsörjning | Låg | Hög | Kontrollera strömförsörjning och kablage innan testning. |
| Tidsstämplar blir fel | Låg | Medel | Kontrollera NTP-synkronisering och tidsformat innan data publiceras. |

---

# 11. Budget och resurser

| Komponent | Antal | Beräknad kostnad |
|---|---:|---:|
| ESP32-S3 utvecklingskort | 1 (+1 reserv) | 220 kr |
| SHT31-D-modul | 2 | 100 kr |
| DS18B20 vattentät sond | 1 (+1 reserv) | 150 kr |
| Motstånd, kopplingsdäck och kablage | – | 100 kr |
| USB-C strömförsörjning/kabel | 1 | 80 kr |
| **Total kostnad** | | **650 kr** |

Programvaran som används i projektet är kostnadsfri eller tillgänglig inom projektets förutsättningar.

Projektet använder bland annat:

- C/C++
- PlatformIO
- Git
- GitHub
- MQTT
- Adafruit IO

---

# 12. Testplan

Testplanen beskriver hur teamet kontrollerar att systemet uppfyller kravspecifikationen.

Varje test dokumenteras med:

- vad som testats
- hur testet genomförts
- förväntat resultat
- faktiskt resultat
- eventuell kommentar.

## 12.1 Komponenttest

Varje komponent testas separat innan hela systemet integreras.

### SHT31-D

- Kontrollera att båda SHT31-D upptäcks via I2C.
- Kontrollera att sensorerna kan läsa temperatur.
- Kontrollera att sensorerna kan läsa relativ luftfuktighet.
- Kontrollera att värdena är rimliga i normal rumsmiljö.
- Kontrollera att de två sensorerna kan skiljas åt genom sina I2C-adresser.

### DS18B20

- Kontrollera att DS18B20 hittas på 1-Wire-bussen.
- Kontrollera att sensorn returnerar ett giltigt värde.
- Jämför mätningen mot en referenstemperatur.
- Testa sensorn vid olika temperaturer om möjligt.

### Wi-Fi

- Kontrollera att ESP32-S3 kan ansluta till Wi-Fi.
- Kontrollera att enheten försöker återansluta efter avbrott.

### NTP

- Kontrollera att ESP32-S3 kan synkronisera tiden.
- Kontrollera att tidsstämpeln har rätt format.

### MQTT

- Kontrollera att ESP32-S3 kan ansluta till MQTT.
- Kontrollera att ett testmeddelande kan publiceras.
- Kontrollera att meddelandet tas emot korrekt.

---

## 12.2 Integrationstest

När komponenterna fungerar separat testas de tillsammans.

Tester:

- Alla fyra mätvärden ska kunna samlas in.
- Mätvärdena ska kunna placeras i samma JSON-paket.
- JSON-paketet ska innehålla en tidsstämpel.
- Mätningarna ska ske med det konfigurerade intervallet.
- Ett fel på en sensor ska kunna identifieras.
- Ett orimligt värde ska kunna identifieras.
- ESP32-S3 ska fortsätta mäta om Wi-Fi tillfälligt försvinner.
- ESP32-S3 ska försöka återansluta när Wi-Fi återkommer.
- MQTT-kommunikationen ska återupptas efter ett tillfälligt avbrott.
- Data ska kunna skickas till Adafruit IO när anslutningen fungerar.

---

## 12.3 Systemtest

Hela systemet testas som en sammanhängande lösning.

1. Starta systemet med alla sensorer anslutna.
2. Kontrollera att ESP32-S3 startar korrekt.
3. Kontrollera att sensorerna identifieras.
4. Kontrollera att alla fyra mätvärden läses in.
5. Kontrollera att mätningarna sker återkommande.
6. Kontrollera att varje mätning får en korrekt tidsstämpel.
7. Kontrollera att JSON-formatet är korrekt.
8. Kontrollera att data skickas via MQTT.
9. Kontrollera att data tas emot av Adafruit IO.
10. Kontrollera att mätdata visas i dashboarden.
11. Bryt Wi-Fi-anslutningen tillfälligt.
12. Kontrollera att systemet fortsätter mäta lokalt.
13. Återanslut Wi-Fi.
14. Kontrollera att systemet återansluter och fortsätter skicka nya mätningar.
15. Kör systemet under en längre period för att kontrollera stabiliteten.
16. Jämför de två SHT31-D-sensorerna när de placeras bredvid varandra.
17. Kontrollera att mätvärdena ligger inom rimlig felmarginal.

Rimligheten i mätvärdena bedöms genom jämförelse mellan de två identiska luftsensorerna, mot en referenstermometer och mot kända temperaturförhållanden när det är möjligt.

---

# 13. Informationssäkerhet

Informationssäkerhet är en del av projektet eftersom systemet kommunicerar över nätverk och skickar data till en extern tjänst.

Följande principer används:

| Område | Lösning |
|---|---|
| Wi-Fi-uppgifter | Ska inte hårdkodas och publiceras i GitHub. |
| MQTT-uppgifter | Ska inte hårdkodas och publiceras i GitHub. |
| Versionshantering | Känsliga uppgifter ska finnas i lokala konfigurationsfiler som inte checkas in. |
| Kommunikation | Säker kommunikation ska användas där det stöds av den valda MQTT-tjänsten. |
| Autentisering | MQTT-tjänsten använder autentisering enligt dess konfiguration. |
| Data | Systemet skickar endast tekniska mätvärden och tidsstämplar. |
| Personuppgifter | Prototypen hanterar inga personuppgifter. |

Känsliga uppgifter ska hanteras separat från versionshanterad kod. Exempelvis ska lösenord, API-nycklar och andra hemligheter inte läggas direkt i GitHub.

---

# 14. Energiförbrukning

MicroHydros är en nätansluten produkt och energiförbrukning är därför ett lägre prioriterat krav i denna prototyp.

Deep sleep används inte i den första prototypen eftersom systemet behöver ha en aktiv Wi-Fi-anslutning och kunna skicka mätdata återkommande.

Energiförbrukningen kan analyseras mer noggrant i en framtida version om produkten ska kunna drivas med batteri.

En framtida batteridriven sensornod skulle exempelvis kunna använda:

- deep sleep
- längre mätintervall
- energieffektiv kommunikation
- batterioptimerad hårdvara.

---

# 15. Tillförlitlighet

Systemet ska hantera fel på ett kontrollerat sätt.

Grundprincipen är att systemet ska upptäcka och hantera fel i stället för att krascha eller fortsätta skicka uppenbart felaktiga värden.

| Situation | Hantering |
|---|---|
| Sensor ger felaktigt värde | Värdet kontrolleras och flaggas vid behov. |
| SHT31-D kan inte läsas | Sensorfelet identifieras och hanteras. |
| DS18B20 svarar inte | Mätningen markeras som felaktig. |
| Orimligt temperaturvärde | Värdet kontrolleras mot ett rimligt intervall. |
| Orimlig luftfuktighet | Värdet kontrolleras mot ett rimligt intervall. |
| Plötsligt stort hopp i mätvärde | Värdet kan flaggas för vidare kontroll. |
| Wi-Fi försvinner | Systemet fortsätter mäta och försöker återansluta. |
| MQTT-anslutning försvinner | Systemet försöker återansluta. |
| Adafruit IO är otillgängligt | Systemet fortsätter samla in mätdata lokalt och återupptar kommunikationen när anslutningen fungerar igen. |
| Enheten startas om | Systemet initierar sensorer och kommunikation igen och återupptar mätning. |

Tillförlitligheten verifieras genom komponenttester, integrationstester och ett längre stabilitetstest.

---

# 16. Skalbarhet

Arkitekturen är förberedd för att kunna byggas ut i framtiden.

Exempel på framtida möjligheter:

- flera MicroHydros-enheter
- ytterligare sensorer
- fler DS18B20-sensorer
- fler mätpunkter
- ytterligare MQTT-feeds
- mer avancerad dashboard
- databas för längre historik
- automatiserad styrning med aktuatorer.

Enhets-ID kan användas för att skilja olika MicroHydros-enheter åt.

Sensorlagret hålls modulärt så att ytterligare sensorer kan läggas till utan att hela systemet behöver byggas om.

Skalbarheten är en framtida möjlighet och behöver inte implementeras fullt ut i den första prototypen.

---

# 17. Dokumentation

Följande dokumentation ska tas fram under projektet och lämnas in:

- Projektplan
- Prioriterad backlog
- Komplett Git-repository med versionshistorik
- README-fil med instruktioner för uppstart och beroenden
- Teknisk dokumentation
- Arkitekturdiagram
- Dokumentation av sensorval
- Testplan
- Testresultat
- Kända begränsningar
- Rekommendationer inför nästa utvecklingsfas
- Individuell reflektionsrapport, 2–4 sidor per studerande.

Dokumentationen ska uppdateras under projektets gång så att den beskriver den faktiska lösningen.

---

# 18. Checklista för projektplanen

Innan projektplanen lämnas in ska följande frågor kunna besvaras:

- [ ] Är projektets bakgrund tydligt beskriven?
- [ ] Finns ett tydligt syfte?
- [ ] Finns konkreta och mätbara mål?
- [ ] Finns en kravspecifikation?
- [ ] Är hårdvaran beskriven?
- [ ] Är mjukvaran beskriven?
- [ ] Är kommunikationen mellan systemen beskriven?
- [ ] Finns en systemarkitektur?
- [ ] Finns en tidsplan?
- [ ] Finns en ansvarsfördelning?
- [ ] Finns en riskanalys?
- [ ] Finns en testplan?
- [ ] Har informationssäkerhet analyserats?
- [ ] Har energiförbrukning analyserats?
- [ ] Är projektets dokumentation planerad?
- [ ] Är README uppdaterad?
- [ ] Är Git-repositoryt uppdaterat?
- [ ] Är testresultaten dokumenterade?

---

# 19. Sammanfattning

Projektet utvecklar en avsiktligt liten men stabil prototyp för MicroHydros.

Lösningen består av:

**ESP32-S3 + C/C++ + två SHT31-D + en DS18B20**

Systemet samlar in:

- temperatur inomhus
- temperatur utomhus
- luftfuktighet inomhus
- temperatur i vattnet.

Mätdata behandlas i ESP32-S3, struktureras som JSON och skickas via Wi-Fi och MQTT till Adafruit IO. Där kan data lagras och visas i ett dashboard.

Fokus ligger på en stabil grundlösning med återkommande mätningar, korrekt tidsstämpling, kommunikation, felhantering och testning.

Kundens tidigare erfarenhet visar att en enkel men tillförlitlig lösning är viktigare än att implementera många funktioner samtidigt. Därför prioriterar projektet sensorernas tillförlitlighet, kommunikation, felhantering och testning.

Projektplanen besvarar:

**Vad?**  
En IoT-baserad prototyp för övervakning av MicroHydros.

**Varför?**  
För att kunna mäta och följa odlingsmiljön på ett mer tillförlitligt sätt.

**Hur?**  
Med ESP32-S3, C/C++, SHT31-D, DS18B20, Wi-Fi, MQTT och Adafruit IO.

**När?**  
31 augusti – 25 september 2026.

**Vem?**  
Ett agilt team om tre studerande.

Planen fungerar som underlag för projektets utveckling, testning, dokumentation och avslutande demonstration.