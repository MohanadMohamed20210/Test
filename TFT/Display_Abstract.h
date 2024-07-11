#ifndef DISPLAY_ABSTRACT_H
#define DISPLAY_ABSTRACT_H

#include <iostream>
#include <memory>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <thread>

#include "SPI.h"
#include "SPIConfiguarations_Abstract.h"
#include "DefaultConfigurations.h"
#include "SPI_Facade.h"
#include "TFT_Abstract.h"
#include "TFT_Concrete.h"

class Display_Abstract
{
    public:
        virtual void DisplayImage() = 0;
};

#endif // DISPLAY_ABSTRACT_H
