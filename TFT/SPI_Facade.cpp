#include "SPI_Facade.h"

#define SPI_MODE (SPI_CPOL | SPI_CPHA)

std::shared_ptr<SPI> SPI_Facade::buildSPI(std::shared_ptr<SPIConfiguarations_Abstract> config)
{
    config->buildChannel();
    config->buildClock();
    config->buildA0();
    config->buildCLK();
    config->buildCS();
    config->buildMOSI();
    config->buildRST();
    return config->getSPI();
}

void SPI_Facade::setup(std::shared_ptr<SPIConfiguarations_Abstract> config)
{
    wiringPiSetup();
    wiringPiSPISetup(config->getSPI()->getChannel(),config->getSPI()->getClock());
    int fd = open("/dev/spidev0.0", O_RDWR);
    unsigned char mode = SPI_MODE;
    ioctl(fd, SPI_IOC_WR_MODE, &mode);

}