# MicroHydros - Prototyp 1

## Projektets syfte
Denna prototyp samlar in miljödata från en hydroponisk odlingsmiljö (temperatur och luftfuktighet) och skickar detta via MQTT till en molntjänst (Adafruit IO) för övervakning över tid. Projektet är utvecklat på uppdrag av HydroGreen Fingers AB.
## Arkitekturdiagrammet
 graph LR
    subgraph Edge [Lokal Odlingsmiljö - MicroHydros]
        SHT[SHT31 Sensor<br>Luft & Fukt] -- I2C --> ESP[ESP32 Mikrokontroller]
        DS[DS18B20 Sensor<br>Vattentemp] -- 1-Wire --> ESP
    end

    subgraph Moln [Adafruit IO Molntjänst]
        ESP -- MQTT Publish<br>över Wi-Fi --> Broker[MQTT Broker]
        Broker --> DB[(Databas för<br>historisk data)]
    end

    subgraph Klient [Slutanvändare]
        DB -. HTTPS / WebSockets .-> Dash[Webb-Dashboard<br>Grafer & Analys]
    end

## Nödvändiga beroenden
- **Hårdvara:** ESP32, SHT31 (I2C), DS18B20 (1-Wire), 4.7kΩ motstånd.
- **Mjukvara:** MicroPython v1.19+, `umqtt.simple`, `sht31` (externt bibliotek).
- **Moln:** Adafruit IO-konto med uppsatta feeds.

## Hur systemet byggs eller startas
1. Flasha ESP32 med MicroPython.
2. Klona detta repository: `git clone https://github.com/WhiteWolf313/MicroHydros.git`
3. Öppna filen `src/config.py` och fyll i era WiFi- och Adafruit IO-uppgifter.
4. Ladda upp innehållet i `src/`-mappen till mikrokontrollern (t.ex. via VS Code + Pymakr eller Thonny).
5. Starta om enheten. Koden i `main.py` körs automatiskt.

## Grundläggande användning
När systemet är igång ansluter det automatiskt till WiFi och börjar publicera mätvärden var 10:e sekund till Adafruit IO. Datat kan visualiseras i den tillhörande Adafruit IO Dashboarden.

