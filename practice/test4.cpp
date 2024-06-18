#include <wiringPi.h>
#include <iostream>
#include <unistd.h>  // For sleep function

// Define GPIO pin numbers
const int LED_PIN = 0;   // GPIO pin 0
const int SWITCH_PIN = 2;  // GPIO pin 2

int pin_old = 5;  // Variable to store old pin state

void setup() {
    wiringPiSetup();  // Initialize wiringPi library
    pinMode(LED_PIN, OUTPUT);  // LED pin as output
    digitalWrite(LED_PIN, LOW);  // Set LED pin initially low
    pinMode(SWITCH_PIN, INPUT);  // Switch pin as input
    pullUpDnControl(SWITCH_PIN, PUD_UP);  // Enable pull-up resistor on switch pin
}

void check() {
    int pin = digitalRead(SWITCH_PIN);  // Read switch pin state

    if (pin == LOW && pin_old != pin) {
        digitalWrite(LED_PIN, HIGH);  // Turn LED on
        std::cout << "LED is on" << std::endl;
    } else if (pin == HIGH && pin_old != pin) {
        digitalWrite(LED_PIN, LOW);  // Turn LED off
        std::cout << "LED is off" << std::endl;
    }

    pin_old = pin;  // Update old pin state
}

int main() {
    setup();  // Initialize GPIO pins

    while (true) {
        check();  // Check switch state and control LED
        delay(200);  // Delay for 200 milliseconds
    }

    return 0;
}
