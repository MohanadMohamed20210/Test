#ifndef SPICONFIGURATIONS_ABSTRACT_H
#define SPICONFIGURATIONS_ABSTRACT_H

#include <iostream>
#include <memory>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "SPI.h"

class SPIConfiguarations_Abstract
{
    public:
        virtual void buildChannel() = 0;
        virtual void buildClock() = 0;
        virtual void buildA0() = 0;
        virtual void buildRST() = 0;
        virtual void buildCS() = 0;
        virtual void buildMOSI() = 0;
        virtual void buildCLK() = 0;
        virtual std::shared_ptr<SPI> getSPI() = 0;
};

#endif // SPICONFIGURATIONS_ABSTRACT_H
