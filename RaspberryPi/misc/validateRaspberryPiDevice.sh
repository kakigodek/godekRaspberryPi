//Author  : kakigodek
//website : kakigodek.my
//Youtube : www.youtube.com/c/kakigodek
//TikTok  : tiktok.com/@kakigodek?lang=en
//email   : kakig0dek2@gmail.com
//Desc    : Script to prevent your Raspberry Pi System to be REPLICATE. Start this script at cron.
//====================================================
#!/bin/bash

#set original serial number to compare
#execute this command to get serial number : sudo cat /sys/firmware/devicetree/base/serial-number
original=10000000b3f2ca6a

#get Serial number from hardware
getSerial=$(tr -d '\0' < /sys/firmware/devicetree/base/serial-number)

if [[ ( $original == $getSerial ) ]]; then
        echo "valid user"
else
        echo "invalid user"
        #You can proced next process if not valid (shutdown/shut interface/stop service etc..)
        #sudo shutdown -h now #uncomment this line if you want to shutdown your device.
fi
