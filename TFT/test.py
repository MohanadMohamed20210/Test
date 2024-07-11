import spidev
import RPi.GPIO as GPIO
import time

class SPI:
    def __init__(self):
        self.channel = 0
        self.clock = 500000
        self.A0 = 21  # BCM 21
        self.RST = 20 # BCM 20
        self.CS = 8   # BCM 8
        self.MOSI = 10 # BCM 10
        self.CLK = 11  # BCM 11

    def get_config(self):
        print("--------------------------------------")
        print(f"Channel => {self.channel}")
        print(f"Clock => {self.clock}")
        print(f"A0 => {self.A0}")
        print(f"RST => {self.RST}")
        print(f"CS => {self.CS}")
        print(f"MOSI => {self.MOSI}")
        print(f"CLK => {self.CLK}")
        print("--------------------------------------")

class DefaultConfigurations:
    def __init__(self):
        self.spi = SPI()

    def build_channel(self):
        self.spi.channel = 0

    def build_clock(self):
        self.spi.clock = 500000

    def build_a0(self):
        self.spi.A0 = 21

    def build_rst(self):
        self.spi.RST = 20

    def build_cs(self):
        self.spi.CS = 8

    def build_mosi(self):
        self.spi.MOSI = 10

    def build_clk(self):
        self.spi.CLK = 11

    def get_spi(self):
        return self.spi

class SPI_Facade:
    @staticmethod
    def build_spi(config):
        config.build_channel()
        config.build_clock()
        config.build_a0()
        config.build_clk()
        config.build_cs()
        config.build_mosi()
        config.build_rst()
        return config.get_spi()

    @staticmethod
    def setup(config):
        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(config.spi.A0, GPIO.OUT)
        GPIO.setup(config.spi.RST, GPIO.OUT)
        GPIO.setup(config.spi.CS, GPIO.OUT)
        GPIO.setup(config.spi.MOSI, GPIO.OUT)
        GPIO.setup(config.spi.CLK, GPIO.OUT)
        
        spi = spidev.SpiDev()
        spi.open(0, 0)
        spi.max_speed_hz = config.spi.clock
        spi.mode = 0b11

class TFT_Abstract:
    _SPI = None

    def __init__(self, spi):
        TFT_Abstract._SPI = spi
        GPIO.setup(self._SPI.A0, GPIO.OUT)
        GPIO.setup(self._SPI.CLK, GPIO.OUT)
        GPIO.setup(self._SPI.CS, GPIO.OUT)
        GPIO.setup(self._SPI.RST, GPIO.OUT)
        GPIO.setup(self._SPI.MOSI, GPIO.OUT)
        self.spi = spidev.SpiDev()
        self.spi.open(0, 0)
        self.spi.max_speed_hz = self._SPI.clock
        self.spi.mode = 0b11

    def send_data(self, data):
        GPIO.output(self._SPI.CS, GPIO.LOW)
        GPIO.output(self._SPI.A0, GPIO.HIGH)
        self.spi.xfer([data])
        GPIO.output(self._SPI.CS, GPIO.HIGH)

    def send_command(self, command):
        GPIO.output(self._SPI.CS, GPIO.LOW)
        GPIO.output(self._SPI.A0, GPIO.LOW)
        self.spi.xfer([command])
        GPIO.output(self._SPI.CS, GPIO.HIGH)

class TFT_Concrete(TFT_Abstract):
    def __init__(self, spi):
        super().__init__(spi)
        # Reset pulse
        GPIO.output(self._SPI.RST, GPIO.HIGH)
        time.sleep(0.0001)
        GPIO.output(self._SPI.RST, GPIO.LOW)
        time.sleep(0.000002)
        GPIO.output(self._SPI.RST, GPIO.HIGH)
        time.sleep(0.0001)
        GPIO.output(self._SPI.RST, GPIO.LOW)
        time.sleep(0.0001)
        GPIO.output(self._SPI.RST, GPIO.HIGH)
        time.sleep(0.12)
        # Sleep out
        self.send_command(0x11)
        time.sleep(0.15)
        # Colour mode command
        self.send_command(0x3A)
        self.send_data(0x05)
        # Display on
        self.send_command(0x29)

class Image_1(TFT_Abstract):
    def __init__(self, spi):
        super().__init__(spi)
        self.image = [0] * 2048  # Replace with your actual image data

    def display_image(self):
        # Set x Address
        self.send_command(0x2A)
        # Start byte
        self.send_data(0)  # MS byte
        self.send_data(0)
        # Stop byte
        self.send_data(0)
        self.send_data(127)

        # Set y Address
        self.send_command(0x2B)
        # Start byte
        self.send_data(0 >> 8)  # MS byte
        self.send_data(0)
        # Stop byte
        self.send_data(159 >> 8)
        self.send_data(159)

        # RAM write
        self.send_command(0x2C)

        for i in range(2048):
            # Write pixel
            high_byte = (self.image[i] >> 8)
            low_byte = (self.image[i] & 0xff)
            self.send_data(high_byte)
            self.send_data(low_byte)

if __name__ == "__main__":
    config = DefaultConfigurations()
    spi = SPI_Facade.build_spi(config)
    SPI_Facade.setup(config)
    spi.get_config()
    tft = TFT_Concrete(spi)
    # image_display = Image_1(spi)
    # image_display.display_image()
