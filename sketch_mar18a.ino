/*  NETPIE ESP32 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <WiFi.h>
#include <MicroGear.h>

const char* ssid     = "CMMC_Sinet_2.4G";
const char* password = "zxc12345";

#define APPID   "DoorSystem"
#define KEY     "EFc5aTLIKNWhK9u"
#define SECRET  "f5KBNYvnar9iUYNwtfsroewP2"
#define ALIAS   "esp32"

WiFiClient client;

int timer = 0;
int relay_Pin_1 = 26;
MicroGear microgear(client);
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);

    char strMsg[msglen];
    for (int i = 0; i < msglen; i++){
      strMsg[i] = (char)msg[i];
    }
    String msgStr = String(strMsg).substring(0,msglen);
    String strTopic = (char *)topic;

   // Serial.print("Topic: ");
   // Serial.println(strTopic);
    //Serial.print("Msg: ");
    //Serial.println(msgStr);

    if(strTopic == "/DoorSystem/gearname/esp32"){
      if (msgStr == "ON"){
        Serial.println("ON");
        digitalWrite(relay_Pin_1,HIGH);
        delay (5000);
        digitalWrite(relay_Pin_1,LOW);
      }         
      }
    }

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}


void setup() {
    /* Add Event listeners */
  pinMode(relay_Pin_1, OUTPUT);
    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            microgear.chat(ALIAS,"Connected");        
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
        ESP.restart();
        }
        else timer += 100;
    }
    delay(100);
}
