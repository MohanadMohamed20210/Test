#include "TFT_Concrete.h"

TFT_Concrete::TFT_Concrete(std::shared_ptr<SPI> spi):TFT_Abstract(spi)
{
    /* Reset pulse */
    digitalWrite(_SPI->getRST(), HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    digitalWrite(_SPI->getRST(), LOW);
    std::this_thread::sleep_for(std::chrono::microseconds(2));
    digitalWrite(_SPI->getRST(), HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    digitalWrite(_SPI->getRST(), LOW);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    digitalWrite(_SPI->getRST(), HIGH);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    
    /* Sleep out */
    sendCommand(0x11);
    
    /* Wait 150 ms */
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    /* Colour mode command */
    sendCommand(0x3A);
    sendData(0x05);
    
    /* Display on */
    sendCommand(0x29);
}