/**
 * WiFiManager advanced demo, contains advanced configurartion options
 * Implements TRIGGEN_PIN button press, press for ondemand configportal, hold for 3 seconds for reset settings.
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
//add By Me.
#include <WiFiClientSecure.h>
#include <time.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
//uptime library
#include "uptime_formatter.h"

const int relayPin = 0;
const int ledPin = 2;
const int onDuration = 10 * 60 * 1000; // 10 minutes in milliseconds
const int timeZone = 8 * 3600; // GMT+8:00 for Malaysia

//


//#define TRIGGER_PIN 1


// Initialize Telegram BOT
#define BOTtoken "teleToken"  // your Bot Token (Get from Botfather)


// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
//getid Your own ID is: XXXXXXX /getgroupid
#define CHAT_ID "-xsxsxsxs" //skgambang Group


#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//const int ledPin = 0; //relay gpio
bool ledState = HIGH;


// wifimanager can run in a blocking mode or a non blocking mode
// Be sure to know how to process loops with no delay() if using non blocking
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

void setSystemTime() {
  configTime(timeZone, 0, "pool.ntp.org", "time.nist.gov");

  while (!time(nullptr)) {
    delay(1000);
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, LOW); // Turn on relay initially
  delay(5000); // Wait for 5 seconds
  digitalWrite(relayPin, HIGH); // Turn off relay
  Serial.println("Done Relay Test.");
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  Serial.begin(115200);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Starting");

  //pinMode(TRIGGER_PIN, INPUT);
  
  // wm.resetSettings(); // wipe settings

  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  // add a custom input field
  /*int customFieldLength = 40;

  new (&custom_field) WiFiManagerParameter("token", "Token", token.c_str(), customFieldLength);
  wm.addParameter(&custom_field);*/
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");

  wm.setConfigPortalTimeout(30); // auto close configportal after n seconds

  bool res;

  res = wm.autoConnect("kakiGodek","P@ssw0rd123"); // password protected ap

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif
  
  if(!res) {
    Serial.println("Failed to connect or hit timeout");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }
  setSystemTime();
}

//comment : if want to use this function please mod the relay board.
/*void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("kakiGodek-CFG","P@ssw0rd")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        bot.sendMessage(CHAT_ID, "Connected.", "");
      }
    }
  }
}
*/

String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

/*void reconnectWiFiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost. Reconnecting...");
    digitalWrite(ledPin, HIGH); // Blink LED while reconnecting
    connectToWiFi();
    digitalWrite(ledPin, LOW); // LED solid on when reconnected
  }
}*/

void loop() {
  if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  //checkButton();
  // put your main code here, to run repeatedly:
   // reconnectWiFiIfNeeded();

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int currentHour = timeinfo.tm_hour;
  int currentMinute = timeinfo.tm_min;

  // Check if it's time to turn on the relay
  if ((currentHour == 7 && currentMinute == 0) ||
      (currentHour == 12 && currentMinute == 0) ||
      (currentHour == 17 && currentMinute == 0)) {
      //(currentHour == 19 && currentMinute == 5)) {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay turned ON");
    bot.sendMessage(CHAT_ID, "Schedule pump is running now. Pump ON.", "");
    delay(onDuration);
    //delay(10000);
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay turned OFF");
    bot.sendMessage(CHAT_ID, "Schedule pump is Stop now. Pump OFF.", "");

    }
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(CHAT_ID, "Bot hanya menerima arahan dari Group Rasmi sahaja...", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    
   String from_name = bot.messages[i].from_name;
   if (from_name == "")
            from_name = "Guest";
        String msg = ""; // init a message string to use

    if (text == "/start") {
      // Now we can UPDATE the message, lets prepare it for sending:
      msg = "Hai " + from_name + "! Saya Amirul, Bot penjaga SmartGarden di sekolah ini..\n\n";
      msg += "Pokok sayur akan di siram sebanyak 3 kali sehari. Waktu siraman adalah pada jam 7 pagi, 12 tengahari dan 5 petang.!\n";
      msg += "Sila tekan butang di bawah untuk mendapatkan informasi dan demo kepada sistem ini.\n\n";
      String keyboardJson = "[[{ \"text\" : \"Demo Pump\", \"callback_data\" : \"/demopump\" }],[{ \"text\" : \"Uptime\", \"callback_data\" : \"/uptime\" }]]";
        
     // Now send this message including the current message_id as the 5th input to UPDATE that message
     bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
     }
     else
     {
      // echo back callback_query which is not handled above
      bot.sendMessage(chat_id, text, "Markdown");
            
    }

    if (text == "/demopump") {
      bot.sendMessage(CHAT_ID, "Turn on pump for a few second.", "");
      ledState = LOW;
      digitalWrite(relayPin, LOW);
      delay(50000); //5 second delay.
      //bot.sendMessage(chat_id, "Turn off pump now. Time at "+GetTime+" .", "");
      bot.sendMessage(CHAT_ID, "Turn off pump now.", "");
      ledState = HIGH;
      digitalWrite(relayPin, HIGH);
    }


    if (text == "/uptime") {
      bot.sendMessage(chat_id, "ESP01 is up and running for : "+ uptime_formatter::getUptime() +"\n", "");
      //containing the total device uptime since startup in days, hours, minutes and seconds
      Serial.println("up " + uptime_formatter::getUptime());
    }

    if (text == "/toggle")
            {

                // Toggle the ledState and update the LED itself
                ledState = !ledState;
                digitalWrite(ledPin, ledState);

                // Now we can UPDATE the message, lets prepare it for sending:
                msg = "Hi " + from_name + "!\n";
                msg += "Notice how the LED state has changed!\n\n";
                msg += "Try it again, see the button has updated as well:\n\n";

                // Prepare the buttons
                String keyboardJson = "["; // start Json
                keyboardJson += "[{ \"text\" : \"The LED is ";
                if (ledState)
                {
                    keyboardJson += "ON";
                }
                else
                {
                    keyboardJson += "OFF";
                }
                keyboardJson += "\", \"callback_data\" : \"/toggle\" }]";
                keyboardJson += ", [{ \"text\" : \"Send text\", \"callback_data\" : \"This text was sent by inline button\" }]"; // add another button
                //keyboardJson += ", [{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }]"; // add another button, this one appears after first Update
                keyboardJson += "]"; // end Json

                // Now send this message including the current message_id as the 5th input to UPDATE that message
                bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
            }

            else
            {
                // echo back callback_query which is not handled above
                bot.sendMessage(chat_id, text, "Markdown");
            }
    

  }
}
