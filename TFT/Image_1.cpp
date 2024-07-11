#include "Image_1.h"


Image_1::Image_1(std::shared_ptr<SPI> spi):TFT_Abstract(spi)
{
    // image = IMAGE_1;
}
void Image_1::DisplayImage()
{
    /* Set x Address */
    sendCommand(0x2A);
    /* Start byte */
    sendData(0);  // MS byte
    sendData(0);

    /* Stop byte */
    sendData(0);
    sendData(127);

    /* Set y Address */
    sendCommand(0x2B);
    /* Start byte */
    sendData((0 >> 8));  // MS byte
    sendData(0);
    /* Stop byte */
    sendData((159 >> 8));
    sendData((unsigned char)159);

    /* RAM write */
    sendCommand(0x2C);

    for (int i = 0 ; i < 2048 ; i++)
    {
        /* Write pixel */
        unsigned char high_byte = (image[i] >> 8);
        unsigned char low_byte = (image[i] & 0xff);
        sendData(high_byte);
        sendData(low_byte);
    }
}