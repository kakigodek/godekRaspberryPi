#!/usr/bin/env python3

# show ip address on screen.
# using Luma Oled.
# ref : https://www.raspberrypi-spy.co.uk/2018/04/i2c-oled-display-module-with-raspberry-pi/
# install : python -m pip install --upgrade luma.oled
# sudo apt install -y python3-pil libjpeg-dev zlib1g-dev python3-av
# sudo apt install -y libfreetype6-dev liblcms2-dev libopenjp2-7 libtiff5 -y
# example : git clone https://github.com/rm-hull/luma.examples.git

import time
import psutil
import socket
from PIL import Image, ImageDraw, ImageFont
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

def get_ip(interface):
    """Get IP address for specific interface"""
    try:
        addrs = psutil.net_if_addrs()
        if interface in addrs:
            for addr in addrs[interface]:
                if addr.family == socket.AF_INET:
                    return f"{interface}: {addr.address}"
        return f"{interface}: Not Connected"
    except Exception as e:
        return f"{interface}: Error"

def display_ips(device):
    # Load default font
    font = ImageFont.load_default()
    
    # Get screen dimensions
    width = device.width
    height = device.height
    
    while True:
        # Create blank image for drawing
        image = Image.new('1', (width, height))
        draw = ImageDraw.Draw(image)
        
        try:
            # Clear display
            draw.rectangle((0, 0, width, height), outline=0, fill=0)
            
            # Get IP addresses
            eth0_ip = get_ip('eth0')
            eth1_ip = get_ip('eth1')
            wlan0_ip = get_ip('wlan0')
            
            # Draw text
            y_offset = 0
            for ip in [eth0_ip, eth1_ip, wlan0_ip]:
                draw.text((0, y_offset), ip, font=font, fill=255)
                y_offset += 12  # Spacing between lines
            
            # Show timestamp
            timestamp = time.strftime("%H:%M:%S")
            draw.text((0, height-10), timestamp, font=font, fill=255)
            
            # Display image
            device.display(image)
            
        except Exception as e:
            # Display error
            draw.rectangle((0, 0, width, height), outline=0, fill=0)
            draw.text((0, 0), f"Error: {str(e)}", font=font, fill=255)
            device.display(image)
            
        # Wait 1 second before refresh
        time.sleep(1)

def main():
    try:
        # Initialize I2C
        serial = i2c(port=1, address=0x3C)
        device = ssd1306(serial)
        
        print("OLED Display Active...")
        print("Press Ctrl+C to exit")
        
        # Run display loop
        display_ips(device)
        
    except KeyboardInterrupt:
        print("\nExiting...")
    except Exception as e:
        print(f"Error: {str(e)}")

if __name__ == "__main__":
    main()
