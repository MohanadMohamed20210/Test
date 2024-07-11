#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <cstring>

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000   // 1 MHz

int main() {
    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize WiringPi.\n";
        return 1;
    }

    pinMode(10, OUTPUT);  // MOSI
    pinMode(9, INPUT);    // MISO
    pinMode(11, OUTPUT);  // SCLK
    pinMode(8, OUTPUT);   // CE0
    digitalWrite(8,HIGH);

    int spi_fd = wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    if (spi_fd == -1) {
        std::cerr << "Failed to initialize SPI.\n";
        return 1;
    }

    const char* message = "Hello, SPI1!";
    int message_len = std::strlen(message);

    while (true) {
        if (wiringPiSPIDataRW(SPI_CHANNEL, (unsigned char*)message, message_len) == -1) {
            std::cerr << "Failed to send data over SPI0. Error: " << strerror(errno) << "\n";
            return 1;
        } else {
            std::cout << "Data sent successfully: " << message << std::endl;
        }
        std::cout << "Data sent: " << message << std::endl;
        usleep(1000000);  // 1 second delay
    }

    close(spi_fd);
    return 0;
}
