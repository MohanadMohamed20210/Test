#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <wiringPi.h>

class LedToggle {
private:
    int pin;
    int toggle;

public:
    LedToggle(int cp_pin) : pin(cp_pin), toggle(0) {
        std::cout << "System has started successfully......" << std::endl;
        wiringPiSetup();
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void TogglePin() {
        while (true) {
            toggle ^= 1;
            if (toggle == 0) {
                digitalWrite(pin, LOW);
            } else {
                digitalWrite(pin, HIGH);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    int getPin() const {
        return pin;
    }
};

LedToggle* global_led = nullptr;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        std::cout << "Ctrl+C pressed - Exiting program..." << std::endl;
        if (global_led) {
            pinMode(global_led->getPin(), OUTPUT);
            digitalWrite(global_led->getPin(), LOW);
        }
        // Clean up any resources (e.g., GPIO cleanup)
        // Optionally, exit the program gracefully
        exit(0);
    }
}

int main() {
    int choice;
    std::cout << "Enter pin to toggle => ";
    std::cin >> choice;

    LedToggle LED(choice);
    global_led = &LED;

    // Register signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, signal_handler);

    // Create thread to toggle LED
    std::thread App(&LedToggle::TogglePin, &LED);

    App.join(); // Wait for thread to finish

    return 0;
}
