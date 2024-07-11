#ifndef SPI_H
#define SPI_H

#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

class SPI
{
private:
    int channel;
    int clock;
    int A0;
    int RST;
    int CS;
    int MOSI;
    int CLK;

public:
    void setChannel(int channel);
    void setClock(int clock);
    void setA0(int A0);
    void setRST(int RST);
    void setCS(int CS);
    void setMOSI(int MOSI);
    void setCLK(int CLK);
    void getConfig()const;
    
    int getChannel();
    int getClock();
    int getA0();
    int getRST();
    int getCS();
    int getMOSI();
    int getCLK();
};

#endif // SPI_H
