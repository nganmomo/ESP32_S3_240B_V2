#include <plcsvg.h>
char mqtt_port_char[32];
char MYTOPIC[5][32];  // = "mytopic2";
char MQTT_BROKER[32];  
//char Rxpin2topic[16][2]; //topic index
//char Txpin2topic[16][2];
char mqtt_RX[18];
byte RelayOnOffstate[16];
int  rxpin;
int  rxvalue[18];
byte rxmode[18];
byte txpinU=0;  
byte txpinD=0;  
byte Mqtteached;
byte Mqttapired;
byte MqttDisplay[16];

byte mqttcellinstate;
byte mqttconnected=0;
byte wificonnected=0;
byte phpublish=0;
char *topicpt;

extern "C" {
	#include "freertos/FreeRTOS.h"
	#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
//#define WIFI_SSID "ATT3jl8Tns"
//#define WIFI_PASSWORD "6mrk8iyf26vn"
//#define MQTT_HOST IPAddress(192, 168, 1, 79)
//#define MQTT_PORT 1883
int16_t   MQTT_PORT=1883; 
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer; 
//////////////////
//IPAddress staticIP(192, 168, 1, 186); 
//IPAddress gateway(192, 168, 1, 1);   // Replace this with your gateway IP Addess
//IPAddress subnet(255, 255, 0, 0);  // Replace this with your Subnet Mask
//IPAddress dns(192, 168, 1, 1);  
///////////////////
void connectToWifi() { 
////////////////////
  //if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
  // Serial.println("Configuration failed.");
  //}
//////////////////// 
  WiFi.begin(&mssid[0],&mpass[0]);
}

void connectToMqtt() {
  mqttconnected=0;
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {   
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        if(mqtten)
          connectToMqtt();
        wificonnected=1;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }    
}

void onMqttConnect(bool sessionPresent) { 
  mqttconnected=1;
  Serial.println("Connected to MQTT.");
  mqttClient.subscribe(&MYTOPIC[0][0],0);
  /*
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  */
  //mqttClient.subscribe(&MYTOPIC[0][0],0);
  //mqttClient.subscribe(&MYTOPIC[1][0],0);
  //mqttClient.subscribe(&MYTOPIC[2][0],0);
  //mqttClient.subscribe(&MYTOPIC[3][0],0);
  //mqttClient.subscribe("PHONE",0);
  /*
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  mqttClient.publish("test/lol", 0, true, "test 1");
  Serial.println("Publishing at QoS 0");
  uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
  Serial.print("Publishing at QoS 1, packetId: ");
  Serial.println(packetIdPub1);
  uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetIdPub2);
  */
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {  
  Serial.println("Subscribe acknowledged.");
}

void onMqttUnsubscribe(uint16_t packetId) {
}

//callback
//all parameter will not take effect in Mqtt mode
//payload format: start at 0= '*'=on '%'=off, switch no. in position 1-2, value at 3-5, mode at 6, 'p','t','v', for pulse toggle & value 
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
byte t,j;
byte y=0;
for(t=0;t<8;t++)    //2D array to 1D array
  {for(j=0;j<4;j++)
    {RxAnaValue[t][j]=payload[y++]-0x30;       
    }
  }  
for(t=0;t<8;t++)    
  RxIOpinit[t]=payload[y++]-0x30; //and cnange to ascii  
//Serial.print("payload=");   
//Serial.println(payload);   
}
/*
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
*/
void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}
//setup
void clientsetup(byte mqtt) {
  if(mqtt)
  { Serial.println("mattsetup");
    getdata(0,0,MQTT_BROKER);  
    getdata(1,0,mqtt_port_char);
    Serial.print("MQTT_BROKER :"); 
    Serial.println(MQTT_BROKER);   
    Serial.print("mqtt_port_char :"); 
    Serial.println(mqtt_port_char);   
    int mqtt_port=(mqtt_port_char[0]-0x30)*1000+(mqtt_port_char[1]-0x30) //??
    *100+(mqtt_port_char[2]-0x30)*10+(mqtt_port_char[3]-0x30);          
    for(int t=0;t<5;t++)       
      {getdata(0,t+1,&MYTOPIC[t][0]);    
      Serial.println(&MYTOPIC[t][0]);      
      }    
    //getdata(0,1,&MYTOPIC[0][0]);    
    //Serial.println(&MYTOPIC[0][0]);      
////////////////////////////////////////////
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt)); 
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  //mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  MQTT_PORT=(mqtt_port_char[0]-0x30)*1000+(mqtt_port_char[1]-0x30)*100+(mqtt_port_char[2]-0x30)*10+(mqtt_port_char[3]-0x30);
  Serial.print("MQTT_PORT:");  
  Serial.println(MQTT_PORT);  
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  }
  //connectToMqtt();
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  WiFi.onEvent(WiFiEvent);
  connectToWifi();
}

void disconnectmqtt()
{
mqttClient.disconnect();  
}

void clientTX(byte topic) {
byte t,j;
byte y=0;
//Serial.print("MYTOPIC=");  
//Serial.println(&MYTOPIC[topic+1][0]);  
char pay_load[41];
for(t=0;t<8;t++)    //2D array to 1D array
  {for(j=0;j<4;j++)
    {pay_load[y++]=TxAnaValue[topic][t][j];       
    }
  }  
for(t=0;t<8;t++)    
  pay_load[y++]=TxIOpinit[topic][t]; //and cnange to ascii
for(t=0;t<40;t++)    
  pay_load[t]=pay_load[t]+0x30;
  pay_load[40]='\0';
  if(mqttClient.publish(&MYTOPIC[topic+1][0],0,false,pay_load,41)>0)
    txpinU=0;                
}  

void clientloop(char *topic,char *payload,byte length) {
}
