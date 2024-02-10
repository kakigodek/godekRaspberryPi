#!/bin/bash

# Function to prompt user for input
prompt_input() {
    read -p "$1: " input
    echo "$input"
}

# Prompt user to insert bot name
bot_name=$(prompt_input "Please enter the bot name")

# Create a directory for the bot
bot_directory="/opt/$bot_name"
echo "Creating directory $bot_directory..."
sudo mkdir -p "$bot_directory"

# Change directory to bot directory
cd "$bot_directory" || exit

# Check if Node.js is already installed
if ! command -v node &> /dev/null; then
    echo "Node.js is not installed. Installing..."

    # Update package lists
    sudo apt update
    
    # Install necessary packages
    sudo apt install -y curl wget
    
    # Download Node.js setup script
    curl -sL https://deb.nodesource.com/setup_current.x | sudo -E bash -
    
    # Install Node.js and npm
    sudo apt install -y nodejs
    
    # Verify installation
    node -v
    npm -v
    
    echo "Node.js and npm installation completed."
else
    echo "Node.js is already installed."
    echo "Version: $(node -v)"
fi

# Install telegraf and node-datetime modules
echo "Installing telegraf and node-datetime modules..."
npm install telegraf node-datetime --save
echo "telegraf and node-datetime modules installation completed."

# Prompt user to insert bot token
bot_token=$(prompt_input "Please enter the bot token")

# Set bot token as environment variable for this session
export BOT_TOKEN="$bot_token"

# Create a config file with the bot token
# config="module.exports = { telegramToken: '$bot_token' };"
# echo "$config" > "config.js"

# Download sysinfo.sh file from git
echo "Downloading sysinfo.sh file..."
wget -O sysinfo.sh https://raw.githubusercontent.com/kakigodek/godekRaspberryPi/main/RaspberryPi/misc/sysinfo.sh

# Make sysinfo.sh executable
chmod +x sysinfo.sh

