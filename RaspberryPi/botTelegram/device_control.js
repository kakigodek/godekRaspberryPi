
//Author  : kakigodek
//website : kakigodek.my
//Youtube : www.youtube.com/c/kakigodek
//TikTok  : tiktok.com/@kakigodek?lang=en
//email   : kakig0dek2@gmail.com
//Desc    : Control Relay with Telegram Bot on Raspberry Pi.
//====================================================


const {Telegraf} = require('telegraf')
const {execSync} = require('child_process')

const bot = new Telegraf('Put Token Here')

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}


console.log("MyNodeJS has started. Start conversations in your Telegram.");

bot.start((ctx) => ctx.reply('Hello there! I am Kakigodek bot Telegram. Type /help to start'))

bot.command('help', (ctx)=>{
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Please select your request :-',
    {
        reply_markup:{
            inline_keyboard: [
                [{text: "My Raspberry Pi IP Adress", callback_data:"ipScript"}],
                [{text: "My Raspberry Pi Uptime", callback_data:"uptime"}],
                [{text: "Kipas", callback_data:"kipas"},{text: "Lampu", callback_data:"lampu"}],
                [{text: "GPIO Status", callback_data:"status"}]
            ]
        }
    })
})

bot.action('bot_menu', (ctx)=>{
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Please select your request :-',
    {
        reply_markup:{
            inline_keyboard: [
                [{text: "My Raspberry Pi IP Adress", callback_data:"ipScript"}],
                [{text: "My Raspberry Pi Uptime", callback_data:"uptime"}],
                [{text: "Kipas", callback_data:"kipas"},{text: "Lampu", callback_data:"lampu"}],
                [{text: "GPIO Status", callback_data:"status"}]
            ]
        }
    })
})

bot.action('kipas', (ctx)=>{
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Due to this is DEMO only, relay just turn on for 1 sec only')
    ctx.telegram.sendMessage(ctx.chat.id, 'Please select your request :-',
    {
        reply_markup:{
            inline_keyboard: [
                [{text: "ON", callback_data:"k_on"},{text: "Off", callback_data:"k_off"}],
                [{text: "Menu", callback_data:"bot_menu"}]
            ]
        }
    })
})

bot.action('lampu', (ctx)=>{
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Due to this is DEMO only, relay just turn on for 1 sec only')
    ctx.telegram.sendMessage(ctx.chat.id, 'Please select your request :-',
    {
        reply_markup:{
            inline_keyboard: [
                [{text: "ON", callback_data:"l_on"},{text: "Off", callback_data:"l_off"}],
                [{text: "Menu", callback_data:"bot_menu"}]
            ]
        }
    })
})

bot.action('k_on', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('python kipas_on.py', { encoding: 'utf-8'});
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : ipScript, Receive :", oL)
    sleep(500).then(() => {
       ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('k_off', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('python kipas_off.py', { encoding: 'utf-8'});
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : ipScript, Receive :", oL)
    sleep(500).then(() => {
       ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('l_on', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('python lampu_on.py', { encoding: 'utf-8'});
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : ipScript, Receive :", oL)
    sleep(500).then(() => {
       ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('l_off', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('python lampu_off.py', { encoding: 'utf-8'});
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : ipScript, Receive :", oL)
    sleep(500).then(() => {
       ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('ipScript', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('hostname -I | cut -d" " -f1', { encoding: 'utf-8'});
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, 'Your Raspberry Pi IP is :')
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : ipScript, Receive :", oL)
    sleep(500).then(() => {
         ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('uptime', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('uptime -p', { encoding: 'utf-8' });
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, 'Your Raspberry Pi Uptime is :')
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : uptime, Receive :", oL)
    sleep(500).then(() => {
         ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.action('status', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('python read.py', { encoding: 'utf-8' });
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, 'Your ultility status is :')
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.chat.first_name,"send command : uptime, Receive :", oL)
    sleep(500).then(() => {
         ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})


bot.launch()
