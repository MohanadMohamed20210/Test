#include "TFT_Abstract.h"

std::shared_ptr<SPI> TFT_Abstract::_SPI = nullptr;


void TFT_Abstract::sendData(unsigned char data)
{
    digitalWrite(_SPI->getCS(),LOW);
    digitalWrite(_SPI->getA0(),HIGH);
    wiringPiSPIDataRW(_SPI->getChannel(), &data, 8);
    digitalWrite(_SPI->getCS(),HIGH);
}


void TFT_Abstract::sendCommand(unsigned char command)
{
    digitalWrite(_SPI->getCS(),LOW);
    digitalWrite(_SPI->getA0(),LOW);
    wiringPiSPIDataRW(_SPI->getChannel(), &command, 8);
    digitalWrite(_SPI->getCS(),HIGH);
}

TFT_Abstract::TFT_Abstract(std::shared_ptr<SPI> spi)
{
    _SPI = spi;

    pinMode(_SPI->getA0(),OUTPUT);

    pinMode(_SPI->getCLK(),OUTPUT);

    pinMode(_SPI->getCS(),OUTPUT);
    

    pinMode(_SPI->getRST(),OUTPUT);

    pinMode(_SPI->getMOSI(),OUTPUT);
}