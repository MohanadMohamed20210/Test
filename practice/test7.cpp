#include <iostream>
#include <wiringPi.h>
#include <signal.h>

// Global flag variable
static volatile int flag = 0;

void func() {
    // Debounce the tactile switch by checking its state
    if (digitalRead(0) == HIGH) {
        // Toggle LED state based on flag
        digitalWrite(2, flag == 0 ? HIGH : LOW);
        flag = flag == 0 ? 1 : 0;  // Toggle flag
        delay(200);  // Debounce delay
    }
}

void signal_handle(int sig) {
    std::cout << "\nSignal " << sig << " received. Cleaning up and exiting.\n";
    
    // Disable interrupt handling before exiting
    wiringPiISR(0, INT_EDGE_SETUP, &func);  // Disable ISR on GPIO pin 0
    
    digitalWrite(2, LOW);  // Ensure the LED is off before exiting
    exit(0);
}

int main() {
    wiringPiSetup();
    pinMode(0, INPUT);
    pullUpDnControl(0, PUD_UP);
    pinMode(3,OUTPUT);
    digitalWrite(3,LOW);
    pinMode(2, OUTPUT);
    digitalWrite(2,LOW);
    // Setup interrupt service routine (ISR) for GPIO pin 0
    wiringPiISR(0, INT_EDGE_RISING , &func);  // Trigger on falling edge (button press)

    // Setup signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, signal_handle);

    // Main loop (optional, depends on application logic)
    while (true) {

    }

    return 0;
}
