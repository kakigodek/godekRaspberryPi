#!/bin/bash
# place in /etc/profile.d/motd.sh with 755 permissions
# get the load averages and uptime
# video tutorial https://www.youtube.com/watch?v=D3wCwTBQ6W0&t=4s
read one five fifteen rest < /proc/loadavg
upSeconds="$(/usr/bin/cut -d. -f1 /proc/uptime)"

printf "$(tput setaf 2)
   .~~.   .~~.    $(date +"%A, %d.%m.%Y, %R:%S")
  '. \ ' ' / .'   $(uname -srmo)$(tput setaf 1)
   .~ .~~~..~.    Uptime...............: $(printf "%d days, %02d:%02d:%02d" "$((${upSeconds}/86400))" "$((${upSeconds}/3600%24))" "$((${upSeconds}/60%60))" "$((${upSeconds}%60))")
  : .~.'~'.~. :   Memory...............: $(($(cat /proc/meminfo | grep MemAvailable | awk {'print $2'}) / 1024))M (Available) / $(($(cat /proc/meminfo | grep MemTotal | awk {'print $2'}) / 1024))M (Total)
 ~ (   ) (   ) ~  Free Disk Space......: $(df -h | grep -E '^/dev/root' | awk '{ print $4 }') on /dev/root
( : '~'.~.'~' : ) Running Processes....: $(ps ax | wc -l | tr -d " ")
 ~ .~ (   ) ~. ~  Load Averages........: ${one}, ${five}, ${fifteen} (1, 5, 15 min)
  (  : '~' :  )   Temperature..........: $(cpu=$(</sys/class/thermal/thermal_zone0/temp) && echo "$((cpu/1000))") C
   '~ .~~~. ~'    eth0 IP Address......: $(ip addr show eth0 | grep "inet\b" | awk '{print $2}' | cut -d/ -f1)
       '~'        Raspberry Pi Model...: $(cat /sys/firmware/devicetree/base/model)
\n$(tput sgr0)"
