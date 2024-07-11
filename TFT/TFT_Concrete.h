#ifndef TFT_CONCRETE_H
#define TFT_CONCRETE_H

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

class TFT_Concrete : public TFT_Abstract
{
    public:
        TFT_Concrete(std::shared_ptr<SPI> spi);
};

#endif // TFT_CONCRETE_H
