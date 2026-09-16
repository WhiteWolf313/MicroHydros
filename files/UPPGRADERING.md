# MicroHydros – tillförlitlighet och lokal vy

Tre tillägg till prototypen: lokal dashboard, lokal buffring vid avbrott och
strukturerat dataformat (JSON). Texten är skriven för att kunna återanvändas i
den tekniska dokumentationen och testdokumentationen.

## Systemöversikt efter ändringen

```
SHT31 (inne) ─┐
SHT31 (ute)  ─┼─→ Sensors ─→ Matning ─┬─→ Cloud ──→ MQTT (JSON) ──→ broker/moln
DS18B20      ─┘                       │      ↑
                                      │   Buffer (ringbuffert, 240 värden)
                                      │      ↑ töms när brokern är tillbaka
                                      └─→ LocalServer ─→ HTTP :80 ─→ dashboard
```

`Matning` (se `src/Matning.h`) är systemets enda representation av ett mätvärde.
Sensorläsning, buffert, MQTT-paket och dashboard använder samma struct och samma
fältnamn, vilket gör att ett värde inte kan betyda olika saker i olika delar av
systemet.

## 1. Lokal dashboard

* Ligger i `src/web_page.h` (HTML + CSS + minimal JavaScript) och serveras av
  `src/LocalServer.cpp` på port 80.
* **Inga externa resurser.** Inga webbfonter, inga CDN-bibliotek, ingen extern CSS.
  Sidan ska fungera när enheten saknar internet, och då går inget att hämta utifrån.
  Hela sidan levereras i ett HTTP-svar från flashminnet (`PROGMEM`, ca 8,7 kB).
* **Reservnätverk.** Om Wi-Fi inte går att nå startar enheten efter två
  misslyckade återanslutningsförsök en egen accesspunkt (`AP_SSID`). Användaren
  ansluter då direkt till enheten och når dashboarden på `http://192.168.4.1`.
  Läget är `WIFI_AP_STA`, så enheten fortsätter försöka nå det vanliga nätverket
  i bakgrunden.
* **Vad sidan visar:** aktuell status för var mätvärdena tar vägen just nu, de
  fyra mätpunkterna, nätverks- och buffertstatus samt de 20 senaste mätningarna.
* **Val av webbserver:** synkrona `WebServer` ur ESP32-kärnan i stället för
  ESPAsyncWebServer. Sidan är liten, antalet samtidiga användare är någon enstaka,
  och koden blir lättare att läsa för en ny utvecklare. Priset är att `loop()`
  blockeras några millisekunder per förfrågan, vilket inte påverkar ett
  mätintervall på 30 sekunder.

Endpoints:

| Metod | Väg | Svar |
|---|---|---|
| GET | `/` | dashboarden (HTML) |
| GET | `/api/status` | nuläge, nätverk, buffert och historik (JSON) |

## 2. Buffring vid avbrott mot brokern

Implementerad som en ringbuffert i `src/Buffer.cpp`.

* **Kapacitet:** 240 mätningar. Med 30 sekunders mätintervall klaras ca 2 timmars
  avbrott utan dataförlust. Minnesåtgång ca 6,7 kB RAM.
* **Vid full buffert** skrivs det äldsta värdet över, och flaggan `tappat` sätts
  och visas i dashboarden. Motivering: färska värden säger mer om odlingsmiljöns
  tillstånd just nu än två timmar gamla värden.
* **Ordning bevaras.** Så länge det finns oskickade värden i bufferten läggs även
  nya mätningar sist i kön, så att mätserien når molnet i rätt ordning.
* **Ett värde tas bort ur bufferten först efter en bekräftat lyckad publicering**
  (`bufferPeek` → publicera → `bufferPop`). Misslyckas publiceringen ligger värdet
  kvar.
* **Tömningstakt:** ett meddelande per 2,5 sekund (`FLUSH_INTERVALL_MS`).
  Adafruit IO tillåter 30 datapunkter per minut på gratisnivån; en burst av
  hundratals meddelanden skulle strypas eller ge avstängning.
* **Ingen blockering.** Återanslutning till Wi-Fi och MQTT sker med tidsstyrda
  försök i `loop()`, aldrig med `while`-loopar. Mätning och dashboard fungerar
  därför som vanligt under hela avbrottet.

Begränsning att nämna i dokumentationen: bufferten ligger i RAM och överlever
inte en omstart eller strömavbrott. Nästa steg vore att skriva den till flash
(LittleFS/NVS). Det valdes bort i prototypen eftersom flashminnet har begränsat
antal skrivcykler och en enkel, förutsägbar lösning prioriterades.

## 3. Strukturerat dataformat (JSON)

Ett mätningstillfälle skickas som ett meddelande till `MQTT_TOPIC_DATA`:

```json
{
  "device_id": "microhydros-01",
  "seq": 42,
  "ts": 1789412345,
  "uptime_s": 1260,
  "buffrad": false,
  "matvarden": {
    "temp_inne_c": 23.4,
    "fukt_inne_rh": 56.2,
    "temp_ute_c": 20.8,
    "temp_vatten_c": null
  },
  "fel": ["temp_vatten_c"]
}
```

Motivering fält för fält:

* `device_id` – flera odlingsenheter ska kunna dela samma broker i framtiden.
* `seq` – löpnummer gör det möjligt för mottagaren att upptäcka tappade värden.
* `ts` – unix-tid från NTP. `0` betyder att klockan inte hunnit synka, och då
  används `uptime_s` i stället. Ett buffrat värde behåller tidpunkten då det
  mättes, inte tidpunkten då det skickades.
* `buffrad` – talar om att värdet kommer i efterhand, så att mottagaren inte
  tolkar det som ett aktuellt värde.
* `matvarden` – en ogiltig avläsning blir JSON `null`, aldrig `-999`. En felkod
  som ser ut som en temperatur är lätt att råka behandla som mätdata.
* `fel` – listar vilka mätpunkter som saknas, vilket gör det enkelt att larma på
  trasiga sensorer utan att analysera varje fält.

Enskilda Adafruit IO-feeds behålls parallellt (`PUBLICERA_ENSKILDA_FEEDS`), så att
de färdiga graferna i molndashboarden fortsätter fungera. Samma JSON-fältnamn
används av `/api/status` lokalt.

## Testfall

| # | Vad testas | Hur | Förväntat resultat |
|---|---|---|---|
| 1 | JSON-format | Prenumerera på topicet med `mosquitto_sub` | Giltig JSON med alla fält, ett meddelande per 30 s |
| 2 | Buffring | Koppla ur routerns internetanslutning i 10 min | Serieutskrift visar att värden buffras, antalet räknas upp i dashboarden |
| 3 | Återsändning | Återställ internet | Buffrade värden skickas med `"buffrad": true`, ca ett per 2,5 s, bufferten når 0 |
| 4 | Ordning | Kontrollera `seq` hos mottagaren efter test 3 | Löpnumren kommer i stigande ordning utan luckor |
| 5 | Dashboard utan router | Stäng av routern helt, anslut till `MicroHydros-01` | Dashboarden svarar på `http://192.168.4.1` och visar färska mätvärden |
| 6 | Felaktig sensor | Koppla loss DS18B20 under drift | Kortet visar "Ingen avläsning", JSON får `null` och `"fel": ["temp_vatten_c"]` |
| 7 | Full buffert | Sänk `BUFFER_KAPACITET` till 5 och bryt anslutningen | Äldsta värdet skrivs över, dashboarden visar att värden tappats |
| 8 | Rimlighet | Jämför inne-/utesensor med en referenstermometer | Avvikelse inom sensorns angivna noggrannhet (±0,2–0,3 °C för SHT31) |

## Säkerhet – åtgärda innan inlämning

`include/config.h` i det nuvarande repot innehåller en riktig Adafruit IO-nyckel.
Om filen har commitats ligger nyckeln kvar i Git-historiken även om den tas bort nu.

1. Skapa en ny AIO-nyckel i Adafruit IO (den gamla slutar då att fungera).
2. Lägg `include/config.h` i `.gitignore` och checka in `include/config.example.h`.
3. Nämn i dokumentationen att MQTT körs okrypterat på port 1883 och att port 8883
   med TLS är nästa steg. Det är en medveten begränsning i en prototyp, och att
   kunna förklara den är värt mer vid bedömningen än att dölja den.
