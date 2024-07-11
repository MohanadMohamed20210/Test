#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <signal.h>
#include <fstream>
#include <cmath>

class PWM {
private:
    double BaseClock;   // Base clock frequency of the PWM module
    double ClockRange;  // Range of PWM values (default 1024)
    double ClockFreq;   // Calculated PWM clock frequency
    double Freq;        // Current PWM frequency
    int pin;            // GPIO pin number used for PWM

public:
    PWM() : Freq(50), ClockRange(1024), BaseClock(19200000), pin(26) {
        // Constructor initializes with the fixed frequency for servo motor
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
        pwmSetClock(static_cast<int>(ClockFreq));  // Set PWM clock frequency
    }

    void setAngle(double angle) {
        // Updates the PWM duty cycle based on the desired servo angle (0 to 180 degrees)
        double pulseWidth = (1.0 + angle / 180.0) * 1.0;  // Calculate pulse width in ms (1ms to 2ms)
        double dutyCycle = (pulseWidth / 20.0) * ClockRange;  // Convert pulse width to duty cycle
        pwmWrite(pin, static_cast<int>(dutyCycle));  // Set PWM duty cycle
    }

    void startPWM() {
        // Starts PWM operation
        while (true) {
            delay(20);  // 20ms period for 50Hz frequency
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
    double angle;  // Variable to hold servo angle input by user

    signal(SIGINT, signal_handle);  // Set up signal handler for SIGINT (Ctrl+C)
    pwm = new PWM();  // Create PWM object with fixed frequency for servo motor

    std::thread App(&PWM::startPWM, pwm);  // Start PWM thread

    // Main loop to dynamically update servo angle
    while (true) 
    {
        std::ifstream input("file.txt");
        input >> angle;
        input.close();
        pwm->setAngle(angle);  // Update servo angle based on file input
        delay(2000);
    }

    delete pwm;  // Delete PWM object to release resources
    return 0;  // Exit program
}
