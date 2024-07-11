// TFT_Abstract.h
#ifndef TFT_Abstract_H
#define TFT_Abstract_H

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

class TFT_Abstract
{
protected:
    void sendData(unsigned char data);
    void sendCommand(unsigned char command);
    static std::shared_ptr<SPI> _SPI;
public:
    TFT_Abstract(std::shared_ptr<SPI> spi);
};

#endif // TFT_Abstract_H
