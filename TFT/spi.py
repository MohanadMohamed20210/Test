import time
import board
import digitalio
import adafruit_rgb_display.st7735 as st7735
from PIL import Image, ImageDraw, ImageFont

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
    rotation=90,
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
width = 128
height = 160
image = Image.new("RGB", (width, height))

# Print image dimensions
print(f"Image width: {width}, height: {height}")

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# # Draw a black filled box to clear the image.
# draw.rectangle((0, 0, width, height), outline=0, fill=(0, 0, 0))

# Draw a red rectangle.
draw.rectangle((0, 0, 127, 159), outline=(255, 0, 0), fill=(255, 0, 0))

# # Draw a green filled rectangle.
# draw.rectangle((70, 10, 120, 60), outline=(0, 255, 0), fill=(0, 255, 0))

# Draw a blue circle.
# draw.ellipse((0, 127, 0, 120), outline=(0, 0, 255), fill=(0, 0, 255))

# Load a TTF font.
try:
    font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 14)
except IOError:
    font = ImageFont.load_default()

# Draw some text.
draw.text((50, 150), "Hello!", font=font, fill=(255, 255, 255))

for y in range(0, height, 32):
    # Define the cropping box for each section
    box = (0, y, min(width, width), min(y + 32, height))
    
    # Crop the image section
    section = image.crop(box)
    
    # Display the cropped section on TFT with rotation
    disp.image(section, rotation=90)

# Wait for a few seconds.
time.sleep(5)
