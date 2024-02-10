#!/bin/bash

# Get Raspberry Pi model
model=$(tr -d '\0' </proc/device-tree/model)
echo "RPi Model: $model"

# Get Raspberry Pi OS name and version
os=$(lsb_release -ds)
echo "OS: $os"

# Get Raspberry Pi serial number
serial=$(tr -d '\0' </proc/cpuinfo | grep Serial | cut -d ' ' -f 2)
echo "Serial Number: $serial"

# Get Raspberry Pi RAM information
ram=$(free -h | awk '/^Mem:/ {print $2}')
echo "RAM: $ram"

# Get Raspberry Pi temperature
temperature=$(vcgencmd measure_temp | cut -d '=' -f 2)
echo "Temperature: $temperature"

# Get Raspberry Pi IP address
ip=$(hostname -I)
echo "IP Address: $ip"

# Get Raspberry Pi uptime
uptime=$(uptime -p)
echo "Uptime: $uptime"
