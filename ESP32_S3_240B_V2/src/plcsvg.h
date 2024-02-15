#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
/*testgit1*/
/*testgit2*/
#define SERIAL1_TX_PIN 47  // GPIO 4 for TX
#define SERIAL1_RX_PIN 45  // GPIO 5 for RX

//#define DEBUG
#define ipcworld
#define MQTTACTIVE
//#define SWITCHTOBROKER
//#define BYPASSSETUP
#define ver2    

#define inputoff    1
#define analogoff   8
#define outputoff   33

#define writeonly   0
#define readwrite   1

#define ON 1
#define OFF 0
#define CLOSE   3 //not for switches
#define OPEN    2
#define xcount 6
#define ycount 40
#define eessid      0           //50
#define adddataz0   50          //240       8 bit            
#define adddataz1   290         //240       8 bit shif 8
#define adddataz2   530         //240       8 bit shit 8
#define adddataz3   770         //240       8 bit 
#define adddataz4   1010        //480       16 bit
#define MQSTdata    1490        //500
#define row5Data    2000        //700       All
#define keychar     2700        //300
#define EEPROM_SIZE 3072        //

#ifdef ver1
#define Pin1 47
#define Pin2 48
#define Pin3 45
#define Pin4 35
#define Pin5 35
#define Pin6 36
#define Pin7 37   
#define Pin8 38

#define Pout1 8
#define Pout2 18
#define Pout3 17
#define Pout4 16
#define Pout5 15
#define Pout6 7
#define Pout7 6   
#define Pout8 5
#define Ain0 0
#define Ain1 9
#define Ain2 10
#define Ain3 11
#define Ain4 12

#define setuppin  13
#endif
#ifdef ver2
#define Pin1 7
#define Pin2 15
#define Pin3 16
#define Pin4 17
#define Pin5 18
#define Pin6 8
#define Pin7 46   
#define Pin8 13

#define Pout1 42
#define Pout2 41
#define Pout3 40
#define Pout4 39
#define Pout5 37
#define Pout6 36
#define Pout7 35   
#define Pout8 45

#define Ain0 0
#define Ain1 4
#define Ain2 5
#define Ain3 3
#define Ain4 6

#define Aout0 0
#define Aout1 9
#define Aout2 10
#define Aout3 11
#define Aout4 12

#define setuppin  1         //SETUP BUTTON
#define SETUPSTATE 1
#define Enmqttclient  2           //RUN OR SIM BUTTON    
//#define mqttbrokenpin  37
//#define mqttclientpin  38
#endif

#ifdef ver3             //with circuit watch inside//
#define Pin1 37
#define Pin2 38
#define Pin3 39
#define Pin4 40
#define Pin5 41
#define Pin6 42
#define Pin7 2   
#define Pin8 1

#define Pout1 8
#define Pout2 18
#define Pout3 17
#define Pout4 16
#define Pout5 15
#define Pout6 7
#define Pout7 6
#define Pout8 5   

#define Ain0 0
#define Ain1 14
#define Ain2 13
#define Ain3 12
#define Ain4 11

#define Aout0 0
#define Aout1 9
#define Aout2 10
#define Aout3 35
#define Aout4 36

#define COOLBOOT    4
#define setuppin    21  //work with SETUPSTATE
#define SETUPSTATE  1
#define Enmqttclient  45           //RUN OR SIM BUTTON  
//#define mqttbrokenpin  37
//#define mqttclientpin  48
#endif

extern char mssid[20];
extern char mpass[20];

extern byte SerialMode;

extern void    rwswiches(int wonly);
extern void    runplc(uint32_t realtimeloop);
extern void    initplc();
extern byte OnOffdevice[61];         //check table
extern byte Deviceinit;
struct boxstate
{
byte DeviceState;
byte Linknum;
};
extern boxstate BoxState[6][40];
//netanalysis
extern byte netnum[6][40];
extern void updatestate();
extern void netanalysis();
extern byte GetlastScan();
extern byte digstate[244];
extern byte anastate[244];
//plc submain//
extern byte asxii2int(char *payload,byte offset); 
extern int numchartowrite;
extern char keyvalue[20];
extern char variable[1600];
extern byte Rtimersetflag[9];
extern size_t   buf_len;  
extern int8_t   mqttrun;
extern void receivedplcdata();    
extern void receivedplc(int eedata,int maxlen,char rw,int offset);     
extern byte cellkeyboard();   
extern void switchsimulate(byte stateonly);    
extern void systemsetup(); 
extern void receivedplcTable();    
extern byte getdata100(int gx,int gy,char* rebuf,int length,char *pt,char endsign=0);
extern byte getdata(byte gx,byte gy,char* rebuf,byte sat,byte leng);
extern void saveint16(char *variable,uint16_t (*dataz)[40],byte start,byte end);
extern void readinput(uint32_t realtimeloop,byte Lasty);
extern void updateinstate(uint32_t realtimeloop,byte lasty);
extern void ioinit(byte type);
///////MQTT//////////
//void brokersetup(int port);
//void brokerloop();
extern void clientsetup(byte mqtt);
extern char *topicpt;
//void clientloop();
extern char MYTOPIC[5][32];  // = "mytopic2";
extern void clientloop(char *topic,char *payload,byte length=0); 
extern void disconnectmqtt();
extern  const char* ssid;
extern  const char* password;
//EEPROM
extern void eepromread(int offset,uint16_t (*dataz)[40],byte type);
extern void eepromwite(int offset,uint16_t (*dataz)[40],byte type);
extern void eewClear(int offset,char keyc,int length);
extern void eewbyte(int offset,char* keyc,int length);
extern void eerbyte(int offset,char* keyc,int length); //add end sign later
extern void eewpassid(int offset,char* iddata,char* pwdata);
extern void eerpassid(int offset,char* iddata,char* pwdata);
extern void eewint16(int offset,int16_t data);
extern int16_t eerint16(int offset);
///////////////////
//for phone
extern char displayvalue[14];
//for hardware//
extern byte Pinout[8];
extern byte Pinin[8];
extern byte analogpin[5];
extern byte analogpout[5];
extern byte inreadstate[9];
extern byte outstatus[9];
extern byte SerachFlag;
extern byte buttonnum;          //xhttp control button
extern byte buttonmode;
//MQTT
extern byte getdata(byte gx,byte gy,char* rebuf,byte sat=0,byte leng=32);
extern void clientTX(byte topic);
extern byte clientTXStatus[4];

extern char RxAnaValue[8][4];   
extern byte RxIOpinit[8]; 
extern char TxAnaValue[4][8][4];   
extern byte TxIOpinit[4][8];

extern char mqtt_RX[18];
extern int  rxvalue[18];
extern byte rxmode[18];
extern byte RelayOnOffstate[16];
extern int rxpin;
extern byte txpinU;  
extern byte txpinD;  
extern byte Mqtteached;
extern byte Mqttapired;
extern byte MqttDisplay[16];
extern byte INKeyreached;
extern byte OUTKeyreached;
extern byte PhoneKeyIn;
extern byte mqttcellinstate;
extern byte mqttconnected;
extern byte wificonnected;
extern byte mqtten;
extern byte checkmqtt();
////////////////////
extern uint16_t dataz0[6][40];//0  sign 16
extern uint16_t dataz1[6][40];//1  id    32
extern uint16_t dataz2[6][40];//2  link  32
extern uint16_t dataz3[6][40];//3  line  16
extern uint16_t dataz4[6][40];//4  set   32
extern char   dataz100g6[100];//4  set   32
extern char   dataz100g7[100];//4  set   32
extern uint32_t timer[2][8];
extern byte beforestate[16];  //link cycle for 16 link 
extern unsigned long counter[16];
extern int16_t d2abuf[9];
extern byte    d2aupedge[9];
extern byte    d2adownedge[9];
extern byte    deboun44[9];
///run mode/////////////
extern int8_t  Runmode;
extern int8_t  Endigitalread;

extern void anykey();

/*
void anykey(int test,int point1,int point2);
Serial.println("anykey");  
while (!Serial.available());
Serial.read();
*/