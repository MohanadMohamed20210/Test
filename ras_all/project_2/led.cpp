#include <iostream>
#include <wiringPi.h>

int main()
{
    wiringPiSetup();
    pinMode(0,OUTPUT);
    digitalWrite(0,HIGH);
    return 0;
}