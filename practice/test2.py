import RPi.GPIO as GPIO
import threading
import time

LED = 0
PIN_OLD = 5

def Setup():
    #set mode BCM for pin number
    GPIO.setmode(GPIO.BCM)
    #pin to be connected to one leg of the switch button
    #set it output low
    GPIO.setup(2,GPIO.OUT)
    GPIO.output(2,GPIO.LOW)
    #setup the other leg of the switch to be connected 
    # to this pin and make it input with pull up resistor to low sensitive
    GPIO.setup(3,GPIO.IN,pull_up_down=GPIO.PUD_UP)
    #setup thi pin to be able to contril led through it
    GPIO.setup(4,GPIO.OUT)
    GPIO.output(4,GPIO.LOW)
    GPIO.setwarnings(False)
def Check():
    global PIN_OLD
    pin = int (GPIO.input(3))
    if pin == GPIO.LOW and PIN_OLD != pin:
        GPIO.output(4,GPIO.HIGH)
        print("LED is on")
    elif pin == GPIO.HIGH and PIN_OLD != pin:
        GPIO.output(4,GPIO.LOW)
        print("LED is off")

    PIN_OLD = pin

def App():
    Setup()
    while True:
        Check()
        time.sleep(0.2)

try:
    App()
except KeyboardInterrupt:
    print("cleanning up")
    GPIO.cleanup
