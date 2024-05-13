#include "DHT.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <WiFiManager.h>
#include <EEPROM.h> // Include the EEPROM library


#define DHTPIN 3 //change from pin 2 to 3 for mod board
#define DHTTYPE DHT11
#define TRIGGER_PIN 0  //add trigger pin to reset   

#define CHECKER_LENGTH 1 // Maximum length of the token
#define CHECKER_ADDRESS 0 // Address in EEPROM to store the token

#define HOST_LENGTH 16 // Maximum length of the token
#define HOST_ADDRESS 1 // Address in EEPROM to store the token

#define PORT_LENGTH 6 // Maximum length of the token
#define PORT_ADDRESS 17 // Address in EEPROM to store the token

#define LOC_LENGTH 16 // Maximum length of the token
#define LOC_ADDRESS 23 // Address in EEPROM to store the token

DHT dht(DHTPIN, DHTTYPE);

// Variables to hold sensor readings
float temp;
float hum;

//EEPROM Data Checker.
char esp_eeprom_check[1]; //0 not format || 1 already format

char mqtt_host[16];
char mqtt_port[6] = "1883"; // Manually set MQTT port
char mqtt_loc[11] = "DRC"; // Manually set MQTT location  add on v2
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiManager wifiManager;

WiFiManagerParameter mqtt_host_param("host", "MQTT Host", mqtt_host, 16);
WiFiManagerParameter mqtt_port_param("port", "MQTT Port", mqtt_port, 6);
WiFiManagerParameter mqtt_loc_param("loc", "Sensor Location", mqtt_loc, 11);

unsigned long previousMillis = 0;
const long interval = 10000;

//Write EEPROM

// String check_val = "0";
// char esp_eeprom_check[2]; // Make sure it has enough space to hold the value and the null terminator
// strcpy(esp_eeprom_check, check_val.c_str());

void saveHostToEEPROM() {
  EEPROM.begin(512); // Re-initialize EEPROM
  String mqtt_host_val = mqtt_host_param.getValue();
  strcpy(mqtt_host, mqtt_host_val.c_str());
  EEPROM.put(1, mqtt_host);
  // EEPROM.put(HOST_ADDRESS, mqtt_host_param.getValue());
  Serial.println("Print Host EPROM");
  Serial.println(mqtt_host_param.getValue());
  EEPROM.commit(); // Save changes to EEPROM
  EEPROM.end();
}

void savePortToEEPROM() {
  EEPROM.begin(512); // Re-initialize EEPROM
  String mqtt_port_val = mqtt_port_param.getValue();
  strcpy(mqtt_port, mqtt_port_val.c_str());
  EEPROM.put(17, mqtt_port);
  EEPROM.commit(); // Save changes to EEPROM
  EEPROM.end();
}

void saveLocToEEPROM() {
  EEPROM.begin(512); // Re-initialize EEPROM
  String mqtt_loc_val = mqtt_loc_param.getValue();
  strcpy(mqtt_loc, mqtt_loc_val.c_str());
  EEPROM.put(23, mqtt_loc); 
  EEPROM.commit(); // Save changes to EEPROM
  EEPROM.end();
}

//read EEPROM

void readMQTTSettings() {
  EEPROM.begin(512); // Initialize EEPROM
  EEPROM.get(0, esp_eeprom_check);
  EEPROM.get(1, mqtt_host);
  EEPROM.get(17, mqtt_port);
  EEPROM.get(23, mqtt_loc); 
  EEPROM.end();
}

void readHostFromEEPROM() {
  EEPROM.begin(512); // Initialize EEPROM
  EEPROM.get(1, mqtt_host);
  Serial.print("Call Back EPROM");
  Serial.println(mqtt_host);
  EEPROM.end();
}

String readCheckerFromEEPROM() {
  String checker = "";
  for (int i = 0; i < CHECKER_LENGTH; i++) {
    char c = EEPROM.read(CHECKER_ADDRESS + i);
    if (c == 0) {
      break; // Reached the end of the token
    }
    checker += c;
  }
  return checker;
}




void eraseEEPROMCallback() {
  EEPROM.begin(512);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  strcpy(esp_eeprom_check, "0");
  EEPROM.put(0, esp_eeprom_check);
  EEPROM.commit();
  delay(2000);
  ESP.restart();
}



void connectToMqtt() {
  Serial.println("Connecting to MQTT...x00");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}


void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    Serial.println("Attempting to reconnect to MQTT...");
    connectToMqtt(); // Reconnect to MQTT connectToMqtt
  } else {
    Serial.println("Lost Wi-Fi connection. Entering Config Portal...");
    wifiManager.setConfigPortalTimeout(180); // Set Config Portal timeout
    wifiManager.startConfigPortal(); // Start Config Portal
  }
}

void connectToWiFiManager() {
  Serial.println("Starting WiFiManager.");
  wifiManager.startConfigPortal("AutoConnectAP");
}

//add new fit3
void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  // Initialize the EEPROM
  EEPROM.begin(512);
  // Read MQTT settings from EEPROM
  Serial.println("Read EEPROM Data...");
  readMQTTSettings();


//  Wire.begin();
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  String eFormat = esp_eeprom_check;
   if(eFormat == "0"){
    Serial.println("Already Format..");
    Serial.println(eFormat);
    //eraseEEPROMCallback();
  }else{
    Serial.println("Commit to clear eeprom...");
    Serial.println(eFormat);
    eraseEEPROMCallback();
  }

  Serial.println("Reading....");
  Serial.println(mqtt_host);
  Serial.println(mqtt_port);

  dht.begin();

  wifiManager.setSaveParamsCallback(saveParamCallback); //add to save data to eeprom
  wifiManager.addParameter(&mqtt_host_param);
  wifiManager.addParameter(&mqtt_port_param);
  wifiManager.addParameter(&mqtt_loc_param);

  wifiManager.autoConnect("IoT-DiTec-Sensor"); //broadcast SSID

  
  //  String mqttHost = mqtt_host_param.getValue(); // Get MQTT Host value as String
  //  String mqttPort = mqtt_port_param.getValue(); // Get MQTT Port value as String
 String mqttHost = mqtt_host; // Get MQTT Host value as String
 String mqttPort = mqtt_port; // Get MQTT Port value as String
  
  

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish); //add new fit3
  // Set the MQTT server using the obtained host and port values
  mqttClient.setServer(mqttHost.c_str(), atoi(mqttPort.c_str())); // Set the MQTT server with the obtained host and port

//  connectToMqtt();
  connectToWifi();


  Serial.println(WiFi.localIP());
  // Get the obtained IP address
  IPAddress ip = WiFi.localIP();

  // Convert IP address to string
  String ipAddress = ip.toString();
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  // Check if WiFi is connected
  if (WiFi.isConnected()) {
   
    Serial.println("reTry wifi connected");
    connectToMqtt();
  } 
}

String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wifiManager.server->hasArg(name)) {
    value = wifiManager.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");

  const char* mqtt_host = mqtt_host_param.getValue();
  const char* mqtt_port = mqtt_port_param.getValue();
  const char* mqtt_loc = mqtt_loc_param.getValue();

  Serial.print("PARAM hostId = ");
  Serial.println(mqtt_host);
  Serial.print("PARAM portId = ");
  Serial.println(mqtt_port);
  Serial.print("PARAM locId = ");
  Serial.println(mqtt_loc);


  saveHostToEEPROM(); // Save MQTT host to EEPROM
  savePortToEEPROM(); // Save MQTT host to EEPROM
  saveLocToEEPROM(); // Save MQTT host to EEPROM

}

void checkButton(){
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
        wifiManager.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wifiManager.setConfigPortalTimeout(120);
      
      if (!wifiManager.startConfigPortal("kakiGodek-CFG","P@ssw0rd")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}

void loop() {
  checkButton(); //check reset button
  IPAddress ip = WiFi.localIP();
  readMQTTSettings();

  String mqttLoc = mqtt_loc; // Get MQTT Host value as String
  String mqttLoc = mqtt_loc_param.getValue(); // Get MQTT Host value as String
  String mqttPubTemp = "ditec/" + mqttLoc + "/temperature";
  String mqttPubHum = "ditec/" + mqttLoc + "/humidity";
  String mqttPubIP = "ditec/" + mqttLoc + "/ip";

  String ipAddress = ip.toString();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    // Dummy temperature and humidity values
  //  float temp = 25.0; // Example: 25.0°C
  //  float hum = 50.0;  // Example: 50.0% humidity
    
    //uint16_t packetIdPub1 = mqttClient.publish("esp/noc/temperature", 1, true, String(temp).c_str());                            
    uint16_t packetIdPub1 = mqttClient.publish(mqttPubTemp.c_str(), 1, true, String(temp).c_str());                            
    //Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", mqtt_temp_param.getValue(), packetIdPub1);
    //Serial.printf("Message: %.2f \n", temp);


    //uint16_t packetIdPub2 = mqttClient.publish("esp/noc/humidity", 1, true, String(hum).c_str());                            
    uint16_t packetIdPub2 = mqttClient.publish(mqttPubHum.c_str(), 1, true, String(hum).c_str());   
    //Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ",mqtt_hum_param.getValue(), packetIdPub2);
    //Serial.printf("Message: %.2f \n", hum);

    //ipAddress
    uint16_t packetIdPub3 = mqttClient.publish(mqttPubIP.c_str(), 1, true, ipAddress.c_str());

  }
}
