import machine
import onewire
import ds18x20
import sht31
import time


class SensorManager:
    def __init__(self, pin_1wire=4, pin_scl=22, pin_sda=21):
        # Initiera DS18B20 (Vatten)
        self.ow = onewire.OneWire(machine.Pin(pin_1wire))
        self.ds = ds18x20.DS18X20(self.ow)
        self.roms = self.ds.scan()

        # Initiera SHT31 (Luft)
        self.i2c = machine.I2C(0, scl=machine.Pin(
            pin_scl), sda=machine.Pin(pin_sda), freq=100000)
        self.sht = sht31.SHT31(self.i2c, addr=0x44)

    def read_water_temp(self):
        if not self.roms:
            return None
        self.ds.convert_temp()
        time.sleep_ms(750)
        temp = self.ds.read_temp(self.roms[0])
        # Felhantering
        if -10 < temp < 50 and temp != 85.0:
            return round(temp, 1)
        return None

    def read_air_conditions(self):
        temp, hum = self.sht.get_temp_humi()
        # Felhantering
        if -20 < temp < 60 and 0 <= hum <= 100:
            return round(temp, 1), round(hum, 1)
        return None, None
