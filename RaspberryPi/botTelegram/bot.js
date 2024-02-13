const { Telegraf } = require('telegraf')
const { message } = require('telegraf/filters')

const config = require('./config');

// Create a new Telegraf instance with your Telegram token
const bot = new Telegraf(config.telegramToken);
const botToken = config.telegramToken;
console.log('Bot Token: '+botToken+'' );

// Define your bot commands or actions
bot.start((ctx) => ctx.reply('Welcome! /info'));
bot.help((ctx) => ctx.reply('Send me a sticker'));
bot.on('sticker', (ctx) => ctx.reply('👍'));
bot.hears('hi', (ctx) => ctx.reply('Hey there'));
bot.hears('Hi', (ctx) => ctx.reply('Hey there'));

bot.command('info', (ctx) => {
    console.log(ctx.from);
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Senarai Arahan :',
        {
            reply_markup:{
                inline_keyboard: [
                    [{text: 'My Info', callback_data: 'info'}]
                ]
            }
        })
});

// Handle the callback for the 'Get Linux Info' button
bot.action('info', (ctx) => {
    ctx.deleteMessage();
    // Execute the 'uname -a' command and send the result
    const { exec } = require('child_process');
    exec('sudo sh /opt/bot123/sysinfo.sh', (error, stdout, stderr) => {
        if (error) {
            ctx.reply('An error occurred while running the command.');
            return;
        }
        ctx.reply('Linux Info:\n' + stdout);
    });
});


//Add new function menu here.

// Start the bot
bot.launch();

console.log('Bot is running...');
