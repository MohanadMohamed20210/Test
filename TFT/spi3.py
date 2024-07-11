import sys
import time
import board
import digitalio
import adafruit_rgb_display.st7735 as st7735
from PIL import Image, ImageDraw
from image import pixel_data, pixel_data_2, pixel_data_3  # Import pixel_data from pixel_data.py

# Configuration for CS, DC, and Reset pins:
cs_pin = digitalio.DigitalInOut(board.CE0)  # CS pin
dc_pin = digitalio.DigitalInOut(board.D25)  # Data/Command pin
reset_pin = digitalio.DigitalInOut(board.D24)  # Reset pin

# Config for display baudrate (default max is 24mhz):
BAUDRATE = 24000000

# Setup SPI bus using hardware SPI:
spi = board.SPI()

# Create the ST7735 display:
disp = st7735.ST7735R(
    spi,
    rotation=0,
    cs=cs_pin,
    dc=dc_pin,
    rst=reset_pin,
    baudrate=BAUDRATE,
)

# Print display dimensions
print(f"Display width: {disp.width}, height: {disp.height}")

# Create blank image for drawing.
# Make sure to create an image with mode 'RGB' for full color.
# The display resolution is 128x160 pixels.
width = 128  # Use display width for image width
height = 160  # Use display height for image height
image = Image.new("RGB", (width, height))

# Print image dimensions
print(f"Image width: {width}, height: {height}")

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# Determine which pixel data to use based on command-line arguments
if len(sys.argv) > 1 and sys.argv[1] == '2':
    selected_pixel_data = pixel_data_2
elif len(sys.argv) > 1 and sys.argv[1] == '3':
    selected_pixel_data = pixel_data_3
else:
    selected_pixel_data = pixel_data

# Draw pixels from selected_pixel_data onto the image
for y in range(height):
    for x in range(width):
        index = y * width + x
        color = selected_pixel_data[index]
        # Extract individual RGB components from RGB565 format
        r = (color >> 11) & 0x1F
        g = (color >> 5) & 0x3F
        b = color & 0x1F
        # Scale components from 5-bit to 8-bit (0-255)
        r = (r * 255) // 31
        g = (g * 255) // 63
        b = (b * 255) // 31
        # Draw pixel on the image
        draw.point((x, y), fill=(r, g, b))

# Display the image on the TFT
disp.image(image)

# Wait for a few seconds.
time.sleep(5)
