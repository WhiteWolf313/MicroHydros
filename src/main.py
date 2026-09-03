import time
from sensors import SensorManager
from cloud import CloudManager
import config


def main():
    # Initiera våra moduler
    sensors = SensorManager()
    cloud = CloudManager()

    # Koppla upp
    cloud.connect_wifi()
    cloud.connect_mqtt()

    # Huvudloop för mätningar
    while True:
        print("--- Ny mätning ---")

        # 1. Hämta data från Edge (Sensors)
        water_temp = sensors.read_water_temp()
        air_temp, air_hum = sensors.read_air_conditions()

        # 2. Skicka data till Cloud/Databas (MQTT)
        cloud.publish_data(config.FEED_VATTEN, water_temp)
        cloud.publish_data(config.FEED_LUFT_INNE, air_temp)
        cloud.publish_data(config.FEED_FUKT_INNE, air_hum)

        # 3. Vänta till nästa mätning
        time.sleep(10)


if __name__ == "__main__":
    main()
