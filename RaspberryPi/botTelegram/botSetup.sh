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

cd "$bot_directory"
sudo wget https://raw.githubusercontent.com/kakigodek/godekRaspberryPi/main/RaspberryPi/misc/sysinfo.sh && sudo chmod +x sysinfo.sh
# Set bot token as environment variable for this session
export BOT_TOKEN="$bot_token"

# Create a config file with the bot token
config="module.exports = { telegramToken: '$bot_token' };"
echo "$config" > "config.js"

# Create a new Node.js file named bot.js
echo "Creating bot.js file..."
echo "const { Telegraf } = require('telegraf')" > bot.js
echo "const { message } = require('telegraf/filters')" >> bot.js
echo "" >> bot.js
echo "const config = require('./config');" >> bot.js
echo "" >> bot.js
echo "// Create a new Telegraf instance with your Telegram token" >> bot.js
echo "const bot = new Telegraf(config.telegramToken);" >> bot.js
echo "const botToken = config.telegramToken;" >> bot.js
echo "console.log('Bot Token: '+botToken+'' );" >> bot.js
echo "" >> bot.js
echo "// Define your bot commands or actions" >> bot.js
echo "bot.start((ctx) => ctx.reply('Welcome! /info'));" >> bot.js
echo "bot.help((ctx) => ctx.reply('Send me a sticker'));" >> bot.js
echo "bot.on('sticker', (ctx) => ctx.reply('👍'));" >> bot.js
echo "bot.hears('hi', (ctx) => ctx.reply('Hey there'));" >> bot.js
echo "bot.hears('Hi', (ctx) => ctx.reply('Hey there'));" >> bot.js
echo "" >> bot.js
echo "bot.command('info', (ctx) => {" >> bot.js
echo "    console.log(ctx.from);" >> bot.js
echo "    ctx.deleteMessage();" >> bot.js
echo "    ctx.telegram.sendMessage(ctx.chat.id, 'Senarai Arahan :'," >> bot.js
echo "        {" >> bot.js
echo "            reply_markup:{" >> bot.js
echo "                inline_keyboard: [" >> bot.js
echo "                    [{text: 'My IP', callback_data: 'ip'}]" >> bot.js
echo "                ]" >> bot.js
echo "            }" >> bot.js
echo "        })" >> bot.js
echo "});" >> bot.js
echo "" >> bot.js
echo "// Handle the callback for the 'Get Linux Info' button" >> bot.js
echo "bot.action('ip', (ctx) => {" >> bot.js
echo "    ctx.deleteMessage();" >> bot.js
echo "    // Execute the 'uname -a' command and send the result" >> bot.js
echo "    const { exec } = require('child_process');" >> bot.js
echo "    exec('sudo sh /opt/${bot_name}/sysinfo.sh', (error, stdout, stderr) => {" >> bot.js
echo "        if (error) {" >> bot.js
echo "            ctx.reply('An error occurred while running the command.');" >> bot.js
echo "            return;" >> bot.js
echo "        }" >> bot.js
echo "        ctx.reply('Linux Info:\\n' + stdout);" >> bot.js
echo "    });" >> bot.js
echo "});" >> bot.js
echo "" >> bot.js
echo "// Start the bot" >> bot.js
echo "bot.launch();" >> bot.js
echo "" >> bot.js
echo "console.log('Bot is running...');" >> bot.js
echo "bot.js file created."

# Make bot.js executable
chmod +x bot.js

# Execute bot.js script
echo "Executing bot.js script..."
node bot.js
echo "bot.js script execution completed."
