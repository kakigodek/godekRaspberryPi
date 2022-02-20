const {Telegraf} = require('telegraf')
const {execSync} = require('child_process')

const bot = new Telegraf('ReplaceWithYourTokenHere')

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

console.log("MyNodeJS has started. Start conversations in your Telegram.");

bot.start((ctx) => ctx.reply('Hello there! I am Kakigodek bot Telegram. Type /info to get Raspberry Pi information'))

bot.command('info', (ctx)=>{

    const output = execSync('uptime -p', { encoding: 'utf-8' });

    ctx.telegram.sendMessage(ctx.chat.id, 'Your Raspberry Pi Uptime is :')
    ctx.telegram.sendMessage(ctx.chat.id, output)
    console.log(ctx.from, output)
    sleep(500).then(() => {
         ctx.telegram.sendMessage(ctx.chat.id, 'Type /info to get menu')
    });
    ctx.deleteMessage();

})
