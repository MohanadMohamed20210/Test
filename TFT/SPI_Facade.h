#ifndef SPI_FACADE_H
#define SPI_FACADE_H

#include <iostream>
#include <memory>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "SPI.h"
#include "SPIConfiguarations_Abstract.h"
#include "DefaultConfigurations.h"

class SPI_Facade
{
public:
    static std::shared_ptr<SPI> buildSPI(std::shared_ptr<SPIConfiguarations_Abstract> config);
    static void setup(std::shared_ptr<SPIConfiguarations_Abstract> config);
};

#endif // SPI_FACADE_H
