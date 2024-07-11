#include "DefaultConfigurations.h"

DefaultConfigurations::DefaultConfigurations()
{
    ptrSPI = std::make_shared<SPI>();
}
void DefaultConfigurations::buildChannel()
{
    ptrSPI->setChannel(0);
}
void DefaultConfigurations::buildClock()
{
    ptrSPI->setClock(500000);
}
void DefaultConfigurations::buildA0()
{
    ptrSPI->setA0(29);
}
void DefaultConfigurations::buildRST()
{
    ptrSPI->setRST(28);
}
void DefaultConfigurations::buildCS()
{
    ptrSPI->setCS(10);
}
void DefaultConfigurations::buildMOSI()
{
    ptrSPI->setMOSI(12);
}
void DefaultConfigurations::buildCLK()
{
    ptrSPI->setCLK(14);
}
std::shared_ptr<SPI> DefaultConfigurations::getSPI()
{
    return ptrSPI;
}