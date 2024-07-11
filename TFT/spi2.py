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

# Draw a blue filled rectangle to fill the screen.
draw.rectangle((0, 0, width, height), outline=0, fill=(0, 0, 255))

# Load a TTF font.
try:
    font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 20)
except IOError:
    font = ImageFont.load_default()

# Define the text to display
text = "Bad Day!\nbut i will\nfind a way"

# Draw the text anywhere on the screen (for example, at coordinates (30, 70))
draw.text((10, 30), text, font=font, fill=(255, 255, 255))

# Display the image on the TFT
disp.image(image)

# Wait for a few seconds.
time.sleep(5)
