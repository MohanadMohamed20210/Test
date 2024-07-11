#include "SPI.h"

void SPI::setChannel(int channel)
{
    this->channel = channel;
}
void SPI::setClock(int clock)
{
    this->clock = clock;
}
void SPI::setA0(int A0)
{
    this->A0 = A0;
}
void SPI::setRST(int RST)
{
    this->RST = RST;
}
void SPI::setCS(int CS)
{
    this->CS = CS;
}
void SPI::setMOSI(int MOSI)
{
    this->MOSI = MOSI;
}
void SPI::setCLK(int CLK)
{
    this->CLK = CLK;
}
void SPI::getConfig()const
{
    std::cout<<"--------------------------------------"<<std::endl;
    std::cout<<"Channel => "<<channel<<std::endl;
    std::cout<<"Clock => "<<clock<<std::endl;
    std::cout<<"A0 => "<<A0<<std::endl;
    std::cout<<"RST => "<<RST<<std::endl;
    std::cout<<"CS => "<<CS<<std::endl;
    std::cout<<"MOSI => "<<MOSI<<std::endl;
    std::cout<<"CLK => "<<CLK<<std::endl;
    std::cout<<"--------------------------------------"<<std::endl;
}

int SPI::getChannel()
{
    return channel;
}
int SPI::getClock()
{
    return clock;
}
int SPI::getA0()
{
    return A0;
}
int SPI::getRST()
{
    return RST;
}
int SPI::getCS()
{
    return CS;
}
int SPI::getMOSI()
{
    return MOSI;
}
int SPI::getCLK()
{
    return CLK;
}