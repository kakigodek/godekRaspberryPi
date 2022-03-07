#Author  : kakigodek
#website : kakigodek.my
#Youtube : www.youtube.com/c/kakigodek
#TikTok  : tiktok.com/@kakigodek?lang=en
#email   : kakig0dek2@gmail.com
#Desc    : Copy protection SD-Card Script, to prevent somebody to copy/duplicate your Raspberry Pi Product. Start this script at cron.
#Cron cmd: @reboot /path/to/script >> /path/to/log
#====================================================


#!/bin/bash
NOW=$(date)
sleep 1m ##Pause 1 minutes before start process.

#set original serial number to compare
#execute this command to get serial number : sudo cat /sys/firmware/devicetree/base/serial-number
original=Change to your device serial number.

#get Serial number from hardware
getSerial=$(tr -d '\0' < /sys/firmware/devicetree/base/serial-number)

if [[ ( $original == $getSerial ) ]]; then
        echo "$NOW :: valid user"
else
        echo "$NOW :: invalid user"
        #You can proced next process if not valid (shutdown/shut interface/stop service etc..)
        #sudo shutdown -h now #uncomment this line if you want to shutdown your device.
fi
