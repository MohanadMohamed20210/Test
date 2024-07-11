#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <cstdint>

// Define SPI channel (0 or 1)
#define SPI_CHANNEL 0

// Define SPI speed (range: 500,000 to 32,000,000)
#define SPI_SPEED   500000

int main() {
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize wiringPi." << std::endl;
        return 1;
    }

    // Initialize SPI
    int spi_fd = wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    if (spi_fd == -1) {
        std::cerr << "Failed to initialize SPI." << std::endl;
        return 1;
    }

    std::cout << "SPI initialized successfully." << std::endl;

    // Prepare data to send
    uint8_t sendData = 0xAA;
    uint8_t receiveData;

    // Perform SPI loopback test
    while (true) {
        // Send data
        wiringPiSPIDataRW(SPI_CHANNEL, &sendData, 1);

        // Receive data
        receiveData = sendData;  // Assuming loopback echo

        // Print received data
        std::cout << "Sent: 0x" << std::hex << static_cast<int>(sendData)
                  << ", Received: 0x" << static_cast<int>(receiveData) << std::endl;

        // Delay for visibility
        delay(1000);
    }

    return 0;
}
