#include <wiringPi.h>
#include <iostream>

int main() {
    // Initialize wiringPi and set pin mode
    if (wiringPiSetup() == -1) {
        std::cerr << "Setup wiringPi failed!" << std::endl;
        return 1;
    }

    int ledPin = 0;  // GPIO pin 0

    pinMode(ledPin, OUTPUT);  // Set pin mode to output

    while (true) {
        digitalWrite(ledPin, HIGH);  // Turn LED on
        delay(500);  // Wait for 500 ms
        digitalWrite(ledPin, LOW);  // Turn LED off
        delay(500);  // Wait for 500 ms
    }

    return 0;
}
