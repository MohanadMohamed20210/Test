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
#include "Image_1.h"

int main()
{
    std::shared_ptr<SPIConfiguarations_Abstract> ptrConfig = std::make_shared<DefaultConfigurations>();
    std::shared_ptr<SPI> spi = SPI_Facade::buildSPI(ptrConfig);
    SPI_Facade::setup(ptrConfig);

    ptrConfig->getSPI()->getConfig();
    std::shared_ptr<TFT_Abstract> ptrTFT = std::make_shared<TFT_Concrete>(spi);
    // std::shared_ptr<Display_Abstract> ptrImage = std::make_shared<Image_1>(spi);
    // ptrImage->DisplayImage();
    return 0;
}