import RPi.GPIO as GPIO
import threading

# Initial LED state
LED = 0

def Toggle():
    global LED
    LED ^= 1
    if LED == 0:
        GPIO.output(17, GPIO.LOW)
        print("LED Off")
    else:
        GPIO.output(17, GPIO.HIGH)
        print("LED On")
    
    # Create and start the timer
    timer = threading.Timer(1, Toggle)
    timer.start()

# Set up GPIO pin numbering mode
GPIO.setmode(GPIO.BCM)
# Set up GPIO pin 17 as an output
GPIO.setup(17, GPIO.OUT)
# Ensure the LED is initially off
GPIO.output(17, GPIO.LOW)

# Create and start the initial timer
timer = threading.Timer(1, Toggle)
timer.start()

try:
    # Keep the script running
    while True:
        pass
except KeyboardInterrupt:
    # Clean up GPIO settings before exiting
    print("Clean up everything")
    GPIO.cleanup()
