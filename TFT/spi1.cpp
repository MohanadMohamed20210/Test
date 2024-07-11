#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <cstring>

#define SPI_CHANNEL 1
#define SPI_SPEED   1000000   // 1 MHz

int main() {
    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize WiringPi.\n";
        return 1;
    }

    pinMode(20, OUTPUT);  // MOSI
    pinMode(19, INPUT);   // MISO
    pinMode(21, OUTPUT);  // SCLK
    pinMode(18, INPUT);  // CE0

    int spi_fd = wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    if (spi_fd == -1) {
        std::cerr << "Failed to initialize SPI.\n";
        return 1;
    }

    unsigned char buffer[256];  // Buffer to hold received data

    while (true) {
        if (wiringPiSPIDataRW(SPI_CHANNEL, buffer, sizeof(buffer)) == -1) {
            std::cerr << "Failed to receive data over SPI1.\n";
            return 1;
        }
        std::cout << "Received data: " << buffer << std::endl;
        usleep(1000000);  // 1 second delay
    }

    close(spi_fd);
    return 0;
}
