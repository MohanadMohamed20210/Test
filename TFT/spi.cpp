#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <cstdint>
#include <unistd.h>

// SPI settings
#define SPI_CHANNEL 0
#define SPI_SPEED 24000000

// Display settings
#define ST7735_TFTWIDTH 128
#define ST7735_TFTHEIGHT 160

// Color definitions
#define ST7735_BLACK 0x0000
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE 0x001F
#define ST7735_WHITE 0xFFFF

// ST7735 Commands
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

// Pin configuration
#define DC_PIN    6
#define RESET_PIN 5

void sendCommand(uint8_t cmd) {
    digitalWrite(DC_PIN, LOW);
    wiringPiSPIDataRW(SPI_CHANNEL, &cmd, 1);
}

void sendData(uint8_t data) {
    digitalWrite(DC_PIN, HIGH);
    wiringPiSPIDataRW(SPI_CHANNEL, &data, 1);
}

void sendColor(uint16_t color, int len) {
    uint8_t high = color >> 8;
    uint8_t low = color & 0xFF;
    for (int i = 0; i < len; i++) {
        sendData(high);
        sendData(low);
    }
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    sendCommand(ST7735_CASET);
    sendData(0x00);
    sendData(x0);
    sendData(0x00);
    sendData(x1);

    sendCommand(ST7735_RASET);
    sendData(0x00);
    sendData(y0);
    sendData(0x00);
    sendData(y1);

    sendCommand(ST7735_RAMWR);
}

void fillScreen(uint16_t color) {
    setAddrWindow(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    sendColor(color, ST7735_TFTWIDTH * ST7735_TFTHEIGHT);
}

void drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    sendColor(color, w * h);
}

void sendPixel(uint8_t x, uint8_t y, uint16_t color) {
    setAddrWindow(x, y, x, y);
    sendColor(color, 1);
}

void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    sendPixel(x0, y0 + r, color);
    sendPixel(x0, y0 - r, color);
    sendPixel(x0 + r, y0, color);
    sendPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        sendPixel(x0 + x, y0 + y, color);
        sendPixel(x0 - x, y0 + y, color);
        sendPixel(x0 + x, y0 - y, color);
        sendPixel(x0 - x, y0 - y, color);
        sendPixel(x0 + y, y0 + x, color);
        sendPixel(x0 - y, y0 + x, color);
        sendPixel(x0 + y, y0 - x, color);
        sendPixel(x0 - y, y0 - x, color);
    }
}

void setup() {
    wiringPiSetup();
    pinMode(DC_PIN, OUTPUT);
    pinMode(RESET_PIN, OUTPUT);

    // Reset the display
    digitalWrite(RESET_PIN, HIGH);
    usleep(500000);
    digitalWrite(RESET_PIN, LOW);
    usleep(500000);
    digitalWrite(RESET_PIN, HIGH);
    usleep(500000);

    // Initialize SPI
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) {
        std::cerr << "Failed to initialize SPI" << std::endl;
        exit(1);
    }

    // Initialize display
    sendCommand(ST7735_SWRESET);
    usleep(150000);
    sendCommand(ST7735_SLPOUT);
    usleep(150000);
    sendCommand(ST7735_COLMOD);
    sendData(0x05);  // 16-bit color
    usleep(150000);
    sendCommand(ST7735_DISPON);
    usleep(150000);
}

int main() {
    setup();

    // Clear the screen with black color
    fillScreen(ST7735_BLACK);

    // Draw red rectangle
    drawRectangle(10, 10, 50, 50, ST7735_RED);

    // Draw green rectangle
    drawRectangle(70, 10, 50, 50, ST7735_GREEN);

    // Draw blue circle
    drawCircle(50, 100, 20, ST7735_BLUE);

    // Pause for a few seconds
    usleep(5000000);

    return 0;
}
