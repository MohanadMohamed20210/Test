#include <wiringPi.h>
#include <iostream>

// Define the GPIO pin number
#define LED_PIN 0  // WiringPi pin 0 is BCM_GPIO 17 (physical pin 11)

int main() {
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "WiringPi setup failed." << std::endl;
        return 1;
    }

    // Set the pin mode to output
    pinMode(LED_PIN, OUTPUT);

    // Blink the LED
    while (true) {
        digitalWrite(LED_PIN, HIGH);  // Turn the LED on
        delay(500);                   // Wait for 500ms
        digitalWrite(LED_PIN, LOW);   // Turn the LED off
        delay(500);                   // Wait for 500ms
    }

    return 0;
}
