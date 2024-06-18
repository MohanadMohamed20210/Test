import RPi.GPIO as GPIO
import time

# Set GPIO mode (BCM or BOARD)
GPIO.setmode(GPIO.BCM)

# Set up GPIO pin for output
led_pin = 17
GPIO.setup(led_pin, GPIO.OUT)

try:
    while True:
        # Turn LED on
        GPIO.output(led_pin, GPIO.HIGH)
        print("LED ON")
        time.sleep(1)  # Wait for 1 second

        # Turn LED off
        GPIO.output(led_pin, GPIO.LOW)
        print("LED OFF")
        time.sleep(1)  # Wait for 1 second

except KeyboardInterrupt:
    # Clean up GPIO on CTRL+C exit
    GPIO.cleanup()
