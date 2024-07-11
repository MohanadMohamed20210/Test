#ifndef IMAGE_1_H
#define IMAGE_1_H

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
#include "Display_Abstract.h"

class Image_1 : public Display_Abstract, public TFT_Abstract
{
    private:
        unsigned short int image[2048];
    public:
        Image_1(std::shared_ptr<SPI> spi);
        void DisplayImage() override;
};

#endif // IMAGE_1_H
