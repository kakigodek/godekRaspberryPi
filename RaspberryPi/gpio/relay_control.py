"""
Author  : kakigodek
website : kakigodek.my
Youtube : www.youtube.com/c/kakigodek
TikTok  : tiktok.com/@kakigodek?lang=en
email   : kakig0dek2@gmail.com
Desc    : Control Relay with Python Script on Raspberry Pi.
"""

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(7,GPIO.OUT)    #Set gpio port as output

print "LED on"            #Print LED on
GPIO.output(7,GPIO.LOW)   #Turn on relay on gpio 7

time.sleep(1)             #delay 1 sec

print "LED off"           #Print LED on
GPIO.output(7,GPIO.HIGH)  #Turn on relay off gpio 7
