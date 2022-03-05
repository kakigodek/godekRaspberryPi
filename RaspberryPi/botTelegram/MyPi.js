//web     : kakigodek.my
//youtube : youtube.com/c/kakigodek
//TikTok  : https://www.tiktok.com/@kakigodek?lang=en
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
    console.log(ctx.from)
    ctx.deleteMessage();
    ctx.telegram.sendMessage(ctx.chat.id, 'Please select your request :-',
    {
        reply_markup:{
            inline_keyboard: [
                [{text: "My Raspberry Pi IP Adress", callback_data:"ipScript"}],
                [{text: "My Raspberry Pi Uptime", callback_data:"uptime"}]
            ]
        }
    })
})

bot.action('ipScript', (ctx)=>{
    ctx.deleteMessage();

    const output = execSync('hostname -I | cut -d" " -f1', { encoding: 'utf-8' });
    let oL = output;
    ctx.telegram.sendMessage(ctx.chat.id, 'Your Raspberry Pi IP is :')
    ctx.telegram.sendMessage(ctx.chat.id, oL)
    console.log(ctx.from, oL)
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
    console.log(ctx.from, oL)
    sleep(500).then(() => {
         ctx.telegram.sendMessage(ctx.chat.id, 'Type /help to get menu')
    });

})

bot.launch()
