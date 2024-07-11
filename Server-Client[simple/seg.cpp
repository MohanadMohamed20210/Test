#include <iostream>
#include <wiringPi.h>
#include <signal.h>
// Define the GPIO pins for each segment
#define SEG_A 6
#define SEG_B 10
#define SEG_C 11
#define SEG_D 31
#define SEG_E 27
#define SEG_F 28
#define SEG_G 29

void setup() {
    wiringPiSetup();
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);
}

void displayDigit(int digit) {
    // Define the bit patterns for digits 0-9
    const int digits[10][7] = {
        {1, 1, 1, 1, 1, 1, 0}, // 0
        {0, 1, 1, 0, 0, 0, 0}, // 1
        {1, 1, 0, 1, 1, 0, 1}, // 2
        {1, 1, 1, 1, 0, 0, 1}, // 3
        {0, 1, 1, 0, 0, 1, 1}, // 4
        {1, 0, 1, 1, 0, 1, 1}, // 5
        {1, 0, 1, 1, 1, 1, 1}, // 6
        {1, 1, 1, 0, 0, 0, 0}, // 7
        {1, 1, 1, 1, 1, 1, 1}, // 8
        {1, 1, 1, 1, 0, 1, 1}  // 9
    };

    // Set the segments
    digitalWrite(SEG_A, digits[digit][0]);
    digitalWrite(SEG_B, digits[digit][1]);
    digitalWrite(SEG_C, digits[digit][2]);
    digitalWrite(SEG_D, digits[digit][3]);
    digitalWrite(SEG_E, digits[digit][4]);
    digitalWrite(SEG_F, digits[digit][5]);
    digitalWrite(SEG_G, digits[digit][6]);
}
void signal_handle(int SIG)
{
    digitalWrite(SEG_A, LOW);
    digitalWrite(SEG_B, LOW);
    digitalWrite(SEG_C, LOW);
    digitalWrite(SEG_D, LOW);
    digitalWrite(SEG_E, LOW);
    digitalWrite(SEG_F, LOW);
    digitalWrite(SEG_G, LOW);

    exit(0);
}
int main() {
    setup();
    signal(SIGINT,signal_handle);
    while (1) {
        for (int i = 0; i < 10; i++) {
            displayDigit(i);
            delay(1000); // Display each digit for 1 second
        }
    }

    return 0;
}
