import network
import time
from lib.umqtt.simple import MQTTClient
import config


class CloudManager:
    def __init__(self, client_id="microhydros_edge"):
        self.client = MQTTClient(
            client_id,
            config.AIO_SERVER,
            port=config.AIO_PORT,
            user=config.AIO_USER,
            password=config.AIO_KEY,
            keepalive=60
        )

    def connect_wifi(self):
        wifi = network.WLAN(network.STA_IF)
        wifi.active(True)
        wifi.connect(config.WIFI_SSID, config.WIFI_PASS)
        print("Ansluter till WiFi...")
        while not wifi.isconnected():
            time.sleep(1)
        print("WiFi anslutet!")

    def connect_mqtt(self):
        try:
            self.client.connect()
            print("Ansluten till Adafruit IO!")
        except Exception as e:
            print("MQTT Anslutningsfel:", e)

    def publish_data(self, feed, value):
        if value is not None:
            try:
                self.client.publish(feed, str(value))
                print(f"Skickade {value} till {feed}")
            except Exception as e:
                print("Fel vid publicering:", e)
