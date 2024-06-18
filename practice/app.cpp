#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <signal.h>
#include <fstream>

class PWM {
private:
    double BaseClock;   // Base clock frequency of the PWM module
    double ClockRange;  // Range of PWM values (default 1024)
    double ClockFreq;   // Calculated PWM clock frequency
    double Freq;        // Current PWM frequency
    int pin;            // GPIO pin number used for PWM

public:
    PWM(double cp_Freq) : Freq(cp_Freq), ClockRange(1024), BaseClock(19200000), pin(26) {
        // Constructor initializes with the given frequency
        std::cout << "Set Up PWM......" << std::endl;
        setupPWM();  // Setup PWM configuration
    }

    void setupPWM() {
        // Sets up PWM parameters
        ClockFreq = BaseClock / (ClockRange * Freq);  // Calculate PWM clock frequency
        wiringPiSetup();  // Initialize wiringPi
        pinMode(pin, PWM_OUTPUT);  // Set pin mode to PWM output
        pwmSetMode(PWM_MODE_MS);   // Set PWM mode to mark-space mode
        pwmSetRange(ClockRange);   // Set PWM range
        pwmSetClock(ClockFreq);    // Set PWM clock frequency
    }

    void updateFrequency(double newFreq) {
        // Updates PWM frequency dynamically
        Freq = newFreq;  // Update frequency member variable
        ClockFreq = BaseClock / (ClockRange * Freq);  // Recalculate PWM clock frequency
        pwmSetClock(ClockFreq);  // Update PWM clock with new frequency
    }

    void startPWM() {
        // Starts PWM operation
        double _delay = 1000.0 / Freq;  // Calculate delay between PWM cycles
        while (true) {
            pwmWrite(pin, ClockRange / 2);  // Set PWM duty cycle to 50% (middle of range)
            delay(_delay);  // Delay for one PWM cycle period
        }
    }

    int getPin() {
        // Returns GPIO pin number
        return pin;
    }
};

PWM* pwm = nullptr;  // Pointer to PWM object

void signal_handle(int sig) {
    // Signal handler function (SIGINT - Ctrl+C)
    if (pwm) {
        std::cout << "\nInterrupt signal has been received....." << std::endl;
        pinMode(pwm->getPin(), OUTPUT);   // Set pin mode to OUTPUT
        digitalWrite(pwm->getPin(), LOW);  // Set pin output to LOW
        delete pwm;  // Delete PWM object to release resources
        exit(0);  // Exit program
    }
}

int main() {
    double freq;  // Variable to hold PWM frequency input by user

    signal(SIGINT, signal_handle);  // Set up signal handler for SIGINT (Ctrl+C)
    pwm = new PWM(10);  // Create PWM object with initial frequency

    std::thread App(&PWM::startPWM, pwm);  // Start PWM thread

    // Main loop to dynamically update PWM frequency
    while (true) 
    {
        std::ifstream input("file.txt");
        input>>freq;
        input.close();
        pwm->updateFrequency(freq);  // Update PWM frequency dynamically
        delay(2000);
    }

    delete pwm;  // Delete PWM object to release resources
    return 0;  // Exit program
}
