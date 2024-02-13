#!/bin/bash

# Function to configure dhcpcd.conf and reboot the Raspberry Pi
configure_and_reboot_pi() {
    local interface="$1"
    local ip_address="$2"
    local subnet_mask="$3"
    local gateway="$4"
    local dns_server="$5"

    # Check if dhcpcd.conf exists
    if [ ! -f "/etc/dhcpcd.conf" ]; then
        echo "Error: dhcpcd.conf not found."
        exit 1
    fi

    # Backup original dhcpcd.conf
    cp /etc/dhcpcd.conf /etc/dhcpcd.conf.backup

    # Remove existing configuration for the specified interface
    sudo sed -i "/interface $interface/,/^$/d" /etc/dhcpcd.conf

    # Append new configuration for the specified interface
    cat << EOF | sudo tee -a /etc/dhcpcd.conf > /dev/null
interface $interface
    static ip_address=$ip_address/$subnet_mask
    static routers=$gateway
    static domain_name_servers=$dns_server
EOF
    # Delete all IP addresses associated with the interface
    sudo ip addr flush dev "$interface"

    echo "IP addresses flushed from interface $interface."

    # Reboot the Raspberry Pi

    echo "Rebooting the Raspberry Pi..."
    sudo reboot
}

# Main script
echo "Configure dhcpcd.conf and reboot the Raspberry Pi"

# Prompt user for network interface
read -p "Enter network interface (e.g., wlan0, eth0): " interface

# Prompt user for IP address
read -p "Enter static IP address: " ip_address

# Prompt user for subnet mask
read -p "Enter subnet mask in CIDR notation (e.g., 24 for 255.255.255.0): " subnet_mask

# Prompt user for default gateway
read -p "Enter default gateway IP address: " gateway

# Prompt user for DNS server
read -p "Enter DNS server IP address: " dns_server

# Call function to configure dhcpcd.conf and reboot the Raspberry Pi
configure_and_reboot_pi "$interface" "$ip_address" "$subnet_mask" "$gateway" "$dns_server"
