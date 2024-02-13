#!/bin/bash

# Function to display interface information
show_interface_info() {
    local interface="$1"
    
    echo "Interface: $interface"
    
    # IP address and subnet mask
    ip addr show $interface | awk '/inet / {print "    IP Address:", $2; print "    Subnet Mask:", $4}'
    
    # Gateway
    local gateway=$(ip route | awk '/'$interface'/ && /default/ {print $3}')
    echo "    Gateway: $gateway"
    
    # DNS servers
    local dns_servers=$(grep nameserver /etc/resolv.conf | awk '{print $2}')
    echo "    DNS Servers:"
    for dns_server in $dns_servers; do
        echo "        $dns_server"
    done
    
    echo
}

# Get a list of network interfaces
interfaces=$(ip -o link show | awk -F': ' '{print $2}')

# Loop through each interface and display its information
for interface in $interfaces; do
    show_interface_info $interface
done
