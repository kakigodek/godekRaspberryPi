#!/usr/bin/env python3

import time
import psutil
import socket
from datetime import datetime
import RPi.GPIO as GPIO
from PIL import Image, ImageDraw, ImageFont
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306
import threading

class SystemDisplay:
    def __init__(self):
        # Disable GPIO warnings
        GPIO.setwarnings(False)
        
        # Clean up GPIO first
        try:
            GPIO.cleanup()
        except:
            pass
        
        # Initialize I2C
        self.serial = i2c(port=1, address=0x3C)
        self.device = ssd1306(self.serial)
        
        # Load font
        self.font = ImageFont.load_default()
        
        # Screen dimensions
        self.width = self.device.width
        self.height = self.device.height
        
        # Button setup
        self.BUTTON_PIN = 4
        self.BUZZER_PIN = 19
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(self.BUZZER_PIN, GPIO.OUT)
        GPIO.output(self.BUZZER_PIN, GPIO.LOW)  # Ensure buzzer is off initially
        
        # Screen state (0: Network Info, 1: System Info)
        self.screen_state = 0
        self.last_button_press = 0
        
        # Interfaces to monitor
        self.interfaces = ['eth0', 'eth1', 'wlan0']
    
    def beep(self):
        """Beep the buzzer"""
        def beep_thread():
            try:
                GPIO.output(self.BUZZER_PIN, GPIO.HIGH)
                time.sleep(0.1)  # Beep duration
                GPIO.output(self.BUZZER_PIN, GPIO.LOW)
            except:
                pass
        
        # Start beep in separate thread
        threading.Thread(target=beep_thread, daemon=True).start()

    def cleanup(self):
        """Cleanup GPIO"""
        try:
            GPIO.output(self.BUZZER_PIN, GPIO.LOW)
            GPIO.cleanup()
        except:
            pass

    def get_ip(self, interface):
        """Get IP for interface"""
        try:
            addrs = psutil.net_if_addrs()
            if interface in addrs:
                for addr in addrs[interface]:
                    if addr.family == socket.AF_INET:
                        return f"{interface}: {addr.address}"
            return f"{interface}: Not Connected"
        except:
            return f"{interface}: Error"
    
    def get_system_info(self):
        """Get system metrics"""
        try:
            # CPU Load
            cpu_load = f"CPU: {psutil.cpu_percent()}%"
            
            # CPU Temperature
            with open('/sys/class/thermal/thermal_zone0/temp', 'r') as f:
                temp = float(f.read()) / 1000.0
            cpu_temp = f"Temp: {temp:.1f}°C"
            
            # RAM Usage
            memory = psutil.virtual_memory()
            ram = f"RAM: {memory.percent}%"
            
            # Disk Usage
            disk = psutil.disk_usage('/')
            disk_usage = f"Disk: {disk.percent}%"
            
            return [cpu_load, cpu_temp, ram, disk_usage]
        except Exception as e:
            return [f"Error: {str(e)}"]
    
    def draw_network_screen(self, draw):
        """Draw network information screen"""
        # Draw title
        draw.text((0, 0), "Kakigodek.my", font=self.font, fill=255)
        
        # Draw datetime
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        draw.text((0, 12), now, font=self.font, fill=255)
        
        # Draw network interfaces
        y = 24
        for interface in self.interfaces:
            draw.text((0, y), self.get_ip(interface), font=self.font, fill=255)
            y += 12
    
    def draw_system_screen(self, draw):
        """Draw system information screen"""
        # Draw title
        draw.text((0, 0), "System Information", font=self.font, fill=255)
        
        # Draw system metrics
        y = 12
        for metric in self.get_system_info():
            draw.text((0, y), metric, font=self.font, fill=255)
            y += 12
    
    def button_callback(self, channel):
        """Button press callback"""
        current_time = time.time()
        if current_time - self.last_button_press > 0.3:  # Debounce
            self.screen_state = not self.screen_state
            self.beep()  # Beep when button is pressed
            self.last_button_press = current_time
    
    def run(self):
        """Main display loop"""
        print("System Display Active...")
        print("Press Ctrl+C to exit")
        
        # Add button interrupt
        GPIO.add_event_detect(self.BUTTON_PIN, GPIO.FALLING, 
                            callback=self.button_callback, bouncetime=300)
        
        try:
            while True:
                # Create new image
                image = Image.new('1', (self.width, self.height))
                draw = ImageDraw.Draw(image)
                
                # Clear display
                draw.rectangle((0, 0, self.width, self.height), outline=0, fill=0)
                
                # Draw appropriate screen
                if self.screen_state:
                    self.draw_system_screen(draw)
                else:
                    self.draw_network_screen(draw)
                
                # Display image
                self.device.display(image)
                
                # Wait
                time.sleep(0.1)
                
        except Exception as e:
            print(f"Error: {str(e)}")
        finally:
            self.cleanup()

def main():
    display = None
    try:
        display = SystemDisplay()
        display.run()
    except KeyboardInterrupt:
        print("\nExiting...")
    except Exception as e:
        print(f"Error: {str(e)}")
    finally:
        if display:
            display.cleanup()

if __name__ == "__main__":
    main()
