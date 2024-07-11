import RPi.GPIO as GPIO
import time

# Pin Definitions
MOSI_PIN = 10  # BCM 10 (MOSI)
MISO_PIN = 9   # BCM 9 (MISO)

def setup():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(MOSI_PIN, GPIO.OUT)
    GPIO.setup(MISO_PIN, GPIO.IN)

def spi_write_read_byte(data_out):
    received_byte = 0
    for bit in range(8):
        GPIO.output(MOSI_PIN, data_out & (1 << (7 - bit)))
        # Simulate clock transition (not actually needed for MOSI to MISO loopback)
        time.sleep(0.001)  # Adjust this delay based on your requirements
        received_byte = (received_byte << 1) | GPIO.input(MISO_PIN)
        # Simulate clock transition (not actually needed for MOSI to MISO loopback)
        time.sleep(0.001)  # Adjust this delay based on your requirements
    return received_byte

def main():
    setup()

    # Example SPI communication
    data_out = [0xDE, 0xAD, 0xBE, 0xEF]
    data_in = []

    for byte in data_out:
        received_byte = spi_write_read_byte(byte)
        data_in.append(received_byte)

    print("Sent:     {}".format(data_out))
    print("Received: {}".format(data_in))

    GPIO.cleanup()

if __name__ == "__main__":
    main()
