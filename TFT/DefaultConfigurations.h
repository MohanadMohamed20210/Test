#ifndef DEFAULTCONFIGURATIONS_H
#define DEFAULTCONFIGURATIONS_H

#include <iostream>
#include <memory>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "SPI.h"
#include "SPIConfiguarations_Abstract.h"

class DefaultConfigurations : public SPIConfiguarations_Abstract
{
    private:
        std::shared_ptr<SPI> ptrSPI;
    public:
        DefaultConfigurations();
        void buildChannel()override;
        void buildClock()override;
        void buildA0()override;
        void buildRST()override;
        void buildCS()override;
        void buildMOSI()override;
        void buildCLK()override;
        std::shared_ptr<SPI> getSPI()override;
};

#endif // DEFAULTCONFIGURATIONS_H
