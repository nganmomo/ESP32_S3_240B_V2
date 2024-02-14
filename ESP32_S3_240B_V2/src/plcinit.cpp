#include "plcsvg.h"

#define   LANPulse  1
#define   LANtoggle 2
#define   LANhold   3
#define   MQTTPulse  4
#define   MQTTtoggle 5
#define   MQTThold   6

#define   Logic      1
#define   Value      2
  
#define   edgedone    3
#define   offstate    0
#define   onstate     1
#define   falling     1
#define   rising      0
#define   pulseback   2

unsigned long timerlu[12]={0,1000,1,1000,1,1000,1,1000,1,1000,1,1000};
void HandleDevice(byte type,byte x,byte y,uint32_t realtimeloop);
void readinput(uint32_t realtimeloop,byte Lasty);
int  doit;  //phone mode
byte txtopic=0;
byte prviousPos[9];
byte prviousNeg[9];
byte inreadstate[9];
byte outstatus[9];
char displayvalue[14];
byte beforestate[16];  //link cycle for 16 link 
int16_t d2abuf[9];
byte    d2aupedge[9];
byte    d2adownedge[9];
byte    deboun44[9]; 
//for counter
unsigned long counter[16];
byte countstatus[9];

//for 44 phone
byte clientTXStatus[4];
byte PHONEKEYMODE[16];
byte OUTKeyreached;
byte PhoneKeyIn;
byte mqttuprdate;
//for timer
unsigned long startTime[9];
unsigned long elapsedTime[9];
unsigned long timeset[9];
byte timersetflag[9]; //timer set flag
byte TimerRetOnce[9];
//for rtimer
unsigned long startRTime[9];
unsigned long elapsedRTime[9];
unsigned long Rtimeset[9];
byte Rtimersetflag[9]; //timer set flag
//byte RTimerRetOnce[9];

char RxAnaValue[8][4];   
byte RxIOpinit[8]; 
byte GetAnaValue(byte port)
{byte value;
port=port-1;
RxAnaValue[port][0]=port;
value=RxAnaValue[port][1]*100+
RxAnaValue[port][2]*10+RxAnaValue[port][3]; 
return value;
}

char TxAnaValue[4][8][4];   
byte TxIOpinit[4][8]; 
void SetAnaValue(byte topic,byte port,byte value)
{port=port-1;
TxAnaValue[topic][port][0]=port;
TxAnaValue[topic][port][1]=((value/100)%10);
TxAnaValue[topic][port][2]=((value/10)%10);
TxAnaValue[topic][port][3]=(value%10); 
}



void Int2ascii(char *asciibuf,uint16_t value)
  { 
    asciibuf[0]='v';
    asciibuf[1]='A'; 
    asciibuf[2]='L';
    asciibuf[3]=((value/100)%10)+0x30;
    asciibuf[4]=((value/10)%10)+0x30;
    asciibuf[5]=(value%10)+0x30; 
    asciibuf[6]='*';    
  }
 
byte asxii2int(char *payload,byte offset) 
{byte ibuf;
if(payload[offset+1]<'0' || payload[offset+1]>'9')        
ibuf=payload[offset]-0x30;     //check digi number after control                                             
else if(payload[offset+2]<'0' || payload[offset+2]>'9')   //two digi
{ibuf=((payload[offset]-0x30)*10)+(payload[offset+1]-0x30);}   //if non digi number 
else if(payload[offset+3]<'0' || payload[offset+3]>'9')   //three digi
{ibuf=((payload[offset]-0x30)*100)+((payload[offset+1]-0x30)*10)+(payload[offset+2]-0x30);} 
return ibuf;
}

void rwswiches(int rw)
{byte h;
for(h=1;h<9;h++)
  {if(inreadstate[h]==0)
    {
    if(prviousNeg[h]++>10) prviousNeg[h]=10;
    prviousPos[h]=0;
    }
  else  
    {
    if(prviousPos[h]++>10) prviousPos[h]=10;
    prviousNeg[h]=0;
    }    
  if(rw)  
    {  
#ifdef ver1    
    digitalWrite(Pinout[h-1],outstatus[h]);  
    inreadstate[h]=digitalRead(Pinin[h-1]);  
#else
    digitalWrite(Pinout[h-1],(~outstatus[h])&1); 
    inreadstate[h]=(~digitalRead(Pinin[h-1]))&1;  
#endif
    }
  else
    {
#ifdef ver1    
     digitalWrite(Pinout[h-1],outstatus[h]);   
#else
     digitalWrite(Pinout[h-1],(~outstatus[h])&1);     
#endif
    }      
  }
}


void initplc()
{for(byte h=0;h<9;h++)  
  {outstatus[h]=0;
  timersetflag[h]=0;
  countstatus[h]=0; 
  d2abuf[h]=0;
  d2aupedge[h]=0;
  d2adownedge[h]=0;
  Rtimersetflag[h]=0;  
  beforestate[h]=0;
  deboun44[h]=0;
  for(byte t=0;t<240;t++)
    anastate[t]=0;
  } 
  mqttuprdate=0;
}

void runplc(uint32_t realtimeloop)
{static byte lasty;
if(realtimeloop==0xffff)
  {lasty=GetlastScan();
  //Serial.println("lasty");
  //Serial.println(lasty);
  }

char myTopic4[9] ={'m','y','T','o','p','i','c','4','\0'}; 
rwswiches(readwrite); //read hardware switches;  
if((realtimeloop&0x7fff)==0x7fff && mqttuprdate==1 && mqtten==1)   //mqtten mqtt sw
  {for(byte ct=0;ct<4;ct++)
    {if(clientTXStatus[ct]>0)
      {clientTX(ct);       
      clientTXStatus[ct]=0; 
      }
    }
  }
updateinstate(realtimeloop,lasty);  //update state and handle dvice icon;     
readinput(realtimeloop,lasty);  //read switch state and action;  
Deviceinit=0;
}

void readinput(uint32_t realtimeloop,byte Lasty)  //required table dataz0 update DeviceState
{//Serial.println("IO init 7");   
int pin,linkp;
int x;
int y;
int error=0; 
int goodcount=0; 
byte type;
uint16_t analogvalue;
String Error = "Error "; 
for(x=0;x<xcount;x++)  
  {for(y=0;y<Lasty;y++)  
    {type=dataz0[x][y];         
    if(type>0)
      {pin=dataz1[x][y];            
        if(type==25 && (realtimeloop&0x3f)==0x3f)         //1/16 analog in
          {analogvalue=analogRead(analogpin[pin]);                            
          anastate[netnum[x+1][y]]=analogvalue>>4;                                                   
          }        
        if(type==26)         //1/16 analog in
          {if(dataz2[x][y]>255) dataz2[x][y]=255;                            
          anastate[netnum[x+1][y]]=dataz2[x][y];                                                   
          }                   
        if(type==23)
          {byte linkmode=dataz4[x][y]>>8;                                     
          if(linkmode==1) //non invert
              BoxState[x][y].DeviceState=digstate[BoxState[x][y].Linknum];            
          else if(linkmode==2) //invert
              {BoxState[x][y].DeviceState=~digstate[BoxState[x][y].Linknum]&1;              
              }
          else if(linkmode>2)   //normal state
            {if(digstate[BoxState[x][y].Linknum]!=beforestate[pin])
              {if(linkmode==3 && digstate[BoxState[x][y].Linknum]>beforestate[pin])  //R F
                {BoxState[x][y].DeviceState=ON;                         
                }
              if(linkmode==4 && digstate[BoxState[x][y].Linknum]<beforestate[pin])   //F F
                {BoxState[x][y].DeviceState=ON;             
                }  
              if(linkmode==5 && digstate[BoxState[x][y].Linknum]>beforestate[pin])  //R R 
                {BoxState[x][y].DeviceState=OFF;                      
                }
              if(linkmode==6 && digstate[BoxState[x][y].Linknum]<beforestate[pin])   //F R
                {BoxState[x][y].DeviceState=OFF;                           
                }              
              beforestate[pin]=digstate[BoxState[x][y].Linknum];  
              //anykey();
              }         
            else        //when state change
              {if(linkmode==3 || linkmode==4 && BoxState[x][y].DeviceState==ON)  //R R)  //R R
                {BoxState[x][y].DeviceState=OFF;               
                }    
              if(linkmode==5 || linkmode==6 && BoxState[x][y].DeviceState==OFF)  //R R
                {BoxState[x][y].DeviceState=ON;              
                }                                 
              }                
            }
          }          
        if(type>=28 && type<=36) 
        {anastate[netnum[x+1][y]]=0; //????//
        switch(type)
          {                               
          case  28:           //rising                                        
            if(prviousNeg[pin]>=1 && inreadstate[pin]==1)
              BoxState[x][y].DeviceState=CLOSE; //after trigger all devices will return to 0                     
            break;   
          case  29:           //falling           
            if(prviousPos[pin]>=1 && inreadstate[pin]==0)
              BoxState[x][y].DeviceState=CLOSE; //after trigger all devices will return to 0          
            break;           
          case  30:           //normal open          
            BoxState[x][y].DeviceState=inreadstate[pin]&1; //read form hardware 
            if(BoxState[x][y].DeviceState>=1)
              BoxState[x][y].DeviceState=CLOSE;                                   
            break;
          case  34:           //rising           
            if(prviousNeg[pin]>=1 && inreadstate[pin]==1)       
              BoxState[x][y].DeviceState=CLOSE;          
            break;   
          case  35:           //falling           
            if(prviousPos[pin]>=1 && inreadstate[pin]==0)      
              BoxState[x][y].DeviceState=CLOSE;          
            break;      
          case  36:           //normal close
            BoxState[x][y].DeviceState=(~inreadstate[pin]&1); //read form hardware                    
            if(BoxState[x][y].DeviceState>=1)
              {BoxState[x][y].DeviceState=CLOSE;                    
              }
            break; 
          }
        }
        else //if((realtimeloop&0x4)>0) //1/4 times        
        {switch(type)   //repeat timer
          {case  60:           //RTimer     clock generate                                     
            if(Rtimersetflag[pin]==0)
              {byte mode=dataz2[x][y];  
              BoxState[x][y].DeviceState=OFF;
              switch(mode)
              {case 1:                      //immediate action when trigger 
                Rtimeset[pin]=dataz4[x][y]*1000000;
                break;
              case  2:  
                Rtimeset[pin]=dataz4[x][y]*1000;  
                break;
              case  3:  
                Rtimeset[pin]=dataz4[x][y]*1;                
                break;
              default:
                Rtimeset[pin]=1000;
                break;  
              }
              startRTime[pin]=millis();                    //get present time                           
              Rtimersetflag[pin]=1;           
              }
            if(Rtimersetflag[pin]==1)  //condition for timer running
              {if(((dataz3[x][y+1]&0x8)==0x8 && digstate[netnum[x][y+1]]==0)||((dataz3[x][y+1]&0x8)!=0x8))
                {elapsedRTime[pin]=millis()-startRTime[pin];
                if(elapsedRTime[pin]>=Rtimeset[pin]) 
                  {BoxState[x][y].DeviceState=ON;                
                  Rtimersetflag[pin]=0;
                  elapsedRTime[pin]=0;                    
                  }
                }   
              }                 
            break;                   
          case  45:           //MQTT from user for API phone also, 
            if((realtimeloop&0xfff)==0xfff)
            {byte targetpin=dataz1[x][y];                        
            anastate[netnum[x+1][y]]=GetAnaValue(targetpin);  //analog
            digstate[netnum[x+1][y]]=RxIOpinit[targetpin-1];    //IO               
            BoxState[x][y].DeviceState=RxIOpinit[targetpin-1];           
            }       
            break;       
          case  51:
            if(Mqtteached>0 || Mqttapired>0)//From both Mqtt tested              
            {if(type==51)  Mqttapired=0;                                       
            }
            break;         
          case  50:            //phone  from user                
            if(buttonmode==3 && BoxState[x][y].DeviceState==ON)  
                BoxState[x][y].DeviceState=OFF;       //pulse off control                                                                     
            if(PhoneKeyIn==pin)  //Key number must match select maximum 8 key  
              {PhoneKeyIn=0x10;  //reset                                     
              PHONEKEYMODE[y]=buttonmode;               
                switch(buttonmode)
                {case 1:  //on                
                  BoxState[x][y].DeviceState=ON;  
                  //digstate[netnum[x+1][y]]=1;                                                                                                                
                  break;
                case 2:   //off
                  BoxState[x][y].DeviceState=OFF; 
                  //digstate[netnum[x+1][y]]=0;
                  //INKOFF=buttonnum;                                     
                  break;
                case 3:   //pulse
                  BoxState[x][y].DeviceState=ON;
                  //digstate[netnum[x+1][y]]=1;      
                  //INVPULSE=buttonnum;              
                  break;  
                case 4:   //value                                                                            
                  anastate[netnum[x+1][y]]=asxii2int(variable,6);        
                  //INVALUE=buttonnum;//for value for mqtt                  
                  getdata100(x,y,displayvalue,100,dataz100g6);                            
                  break;            
                case 5:                      
                  getdata100(x,y,displayvalue,100,dataz100g6);    //any one pass can get echo          
                  break;
                }
              getdata100(x,y,displayvalue,100,dataz100g6);     //any one pass can get echo    
              }//end of 50                       
            break;                                    
          }              
        }
      }
    }   
  }
}


void updateinstate(uint32_t realtimeloop,byte lasty)//update instate
{//int col3out;
int x;
int y;   
byte type=0;  
  for(byte t=0;t<240;t++)
    digstate[t]=0;    //off state
  updatestate();       
  {for(y=0;y<lasty;y++)  //lasty
    {for(x=0;x<xcount;x++)         
      {type=dataz0[x][y];     
      if(type>0)            
        HandleDevice(type,x,y,realtimeloop);                                 
      if(type==28||type==29||type==34||type==35)  //pulse type
        {BoxState[x][y].DeviceState=OPEN;           
        }
      }              
    }                                
  }      
}
    
      
        
//////handle device//////
void HandleDevice(byte type,byte x,byte y,uint32_t realtimeloop)
{//if(realtimeloop&0xfffff==0xfffff)       
 // Serial.println("IO init 6");     
  byte pin=dataz1[x][y];  
  byte targetpin;   
  char topbuf[14];         
  char Payload[18]; 
  char Sendpayload[18];  
  switch(type)
 {case 37:
    if(dataz4[x][y]>0)  // && (realtimeloop&0x1)>0)   //timer 1/2
      {byte mode=dataz2[x][y];                              
      //for mode 3-4 9 normal high before time start             7-8 high after time start
      if(mode>=9 && timersetflag[pin]==3)                     //3 off pulse          
        {BoxState[x][y].DeviceState=OFF;                           
        timersetflag[pin]=0;         //clear delay time flag  
        elapsedTime[pin]=0;          //Clear timeset
        }  
      else
         ///////////////////////////////////init state            
        {/////////////////////////////////////reset trigger                  
          if(digstate[netnum[x][y+1]]==1 && (dataz3[x][y+1]&8)==8 && TimerRetOnce[pin]==1)         //reset trigger
            {if(mode==3 || mode==4 || mode==7 || mode==8) 
              BoxState[x][y].DeviceState=OFF;          //active high 
            else
              BoxState[x][y].DeviceState=ON;         //active low      
            TimerRetOnce[pin]=0;   
            elapsedTime[pin]=0; 
            timersetflag[pin]=0;                             
            }                  
          /////////////////////////////////////Start trigger/////can retrigger ////////////////                         
          if(digstate[netnum[x][y]]==1)                 //when enable
            {       
            timersetflag[pin]=1;                       //set timer flag
              {timeset[pin]=dataz4[x][y]*timerlu[mode]; //get delay timer     
              TimerRetOnce[pin]=1;
              }
            if(mode==1 || mode==2) //immediate action when trigger 
              BoxState[x][y].DeviceState=OFF;           //From mode 1 2 3 & 4   
            if(mode==3 || mode==4 || mode==5 || mode==6)  
              BoxState[x][y].DeviceState=ON;
            startTime[pin]=millis();                    //get present time                 
            }
          //////////////////////////////////////Start count time////////////////////////////  
          if(timersetflag[pin]==1)                    //start count time
          {elapsedTime[pin]=millis()-startTime[pin];  
          //action when time reached        
          if(elapsedTime[pin]>=timeset[pin])          //handle delay on
            {if(mode==3 || mode==4 || mode==5 || mode==6)  //active high
              {BoxState[x][y].DeviceState=OFF;                                                                     
              }
            else                                                        //active low
              BoxState[x][y].DeviceState=ON;                                                                                                                       
            if(mode>8)  //9 and 10 for pulse
              {timersetflag[pin]=3;         //pulse off                             
              BoxState[x][y].DeviceState=ON;
              }
            elapsedTime[pin]=0;                                                                                
            } 
          else
            {if(mode==5 || mode==6) 
                BoxState[x][y].DeviceState=ON;       
            if(mode==7 || mode==8) 
                BoxState[x][y].DeviceState=OFF;       
            }                                                   
          }             
        }
      if(Deviceinit==1)
        {if(mode==3 || mode==4 || mode==7 || mode==8)  //active high
            BoxState[x][y].DeviceState=ON;  
        }
        ////////////////////////////////////                       
      }//end of 37    
    break;
  case  38: //counter  
    {byte mode=dataz2[x][y];     
    if(mode>=5 && BoxState[x][y].DeviceState==ON)         
        {BoxState[x][y].DeviceState=OFF;  //end pulse
        counter[pin]=0;              
        }
    if(dataz4[x][y]>0)          //count up
      {if(digstate[netnum[x][y]]==1 && countstatus[pin]==0 && counter[pin]<=dataz4[x][y])   //count up
          {if(mode==1 || mode==3 || mode==5)          
            countstatus[pin]=10; //debounsing
          else
            countstatus[pin]=1; //debounsing  
          //digstate[netnum[x][y]]=0;                   
          if(counter[pin]++>=dataz4[x][y]-1) //-1 adjust
            {if(mode==3 || mode==4)
              BoxState[x][y].DeviceState=OFF;    
            else  
              BoxState[x][y].DeviceState=ON;                                                       
            }
          }
      else                              
          {if(digstate[netnum[x][y]]==0 && counter[pin]<=dataz4[x][y])  //prevent double count
            {if(countstatus[pin]>0)            
                countstatus[pin]--;
            }
          }              
      if(digstate[netnum[x][y+1]]==1 && (dataz3[x][y+1]&8)==8)//reset
          {
          counter[pin]=0;
          countstatus[pin]=0;
          if(mode==3 || mode==4)
            BoxState[x][y].DeviceState=ON;   
          else
            BoxState[x][y].DeviceState=OFF;                
          }
      }  
    if(Deviceinit==1)
      {if(mode==3 || mode==4)  //active high
        BoxState[x][y].DeviceState=ON;  
      }          
    }
    break;
  case  39:      //flip flop set F.F.                                      
    {
      if(digstate[netnum[x][y]]==1)
        BoxState[x][y].DeviceState=ON;                                  
    if(digstate[netnum[x][y+1]]==1 && (dataz3[x][y+1]&8)==8)
        BoxState[x][y].DeviceState=OFF;          
    }      
    break;
  case  40:  //D/A convert u/d  8 is exist        
    {if((dataz3[x][y+1]&8)==8)  // && (realtimeloop&0x1f)==0x1f)                //wait for off
      {int step=dataz2[x][y];      
      if(digstate[netnum[x][y]]==1)   
        {if(d2aupedge[pin]==0)     
          {if(d2abuf[pin]<255)
            d2abuf[pin]=d2abuf[pin]+step;
          if(d2abuf[pin]>=255)   
            d2abuf[pin]=255;                                       
          d2aupedge[pin]=5;  
          }
        anastate[netnum[x+1][y]]=d2abuf[pin];
        }    
      else
        {if(d2aupedge[pin]>0)      //debounsing 
          d2aupedge[pin]--;     
        }  
      
      if(digstate[netnum[x][y+1]]==1)   
        {if(d2adownedge[pin]==0)               
          {if(d2abuf[pin]>step)                     //down
            d2abuf[pin]=d2abuf[pin]-step;
          else   
            d2abuf[pin]=0;            
          d2adownedge[pin]=5;
          }
        anastate[netnum[x+1][y]]=d2abuf[pin];
        }    
      else
        {if(d2adownedge[pin]>0)      //debounsing 
          d2adownedge[pin]--;       
        }  
      }                                    
    }      
    break;
  case  41:      //A/D out                                
    {uint8_t inpin=anastate[netnum[x][y]];
    uint8_t downpin2=dataz2[x][y];   
    uint8_t downpin4=dataz4[x][y];                            
    if(inpin>(downpin2+downpin4))           
      BoxState[x][y].DeviceState=ON;                                  
    if(inpin<(downpin2-downpin4))           
      BoxState[x][y].DeviceState=OFF;                                            
    } 
    break;  
  case  42:     //compare out                     
    {uint8_t inpin=anastate[netnum[x][y]];
    uint8_t downpin=anastate[netnum[x][y+1]];                                                           
    if(inpin-downpin>dataz2[x][y])           
      BoxState[x][y].DeviceState=ON;                                
    else if(downpin-inpin>dataz4[x][y])           
      BoxState[x][y].DeviceState=OFF;                                                                    
    }
    break;  
 case  48:     //analog Math                     
    if((realtimeloop&0x3f)==0x3f) 
    {byte mode=dataz2[x][y];  
    byte inpin=anastate[netnum[x][y]];
    byte downpin=anastate[netnum[x][y+1]];       
    u_int16_t sum;    
    switch(mode)             
      {case 1:  //A NSW
        {if(digstate[netnum[x][y+1]]==1)             
            anastate[netnum[x+1][y]]=inpin;                                      
        else
            anastate[netnum[x+1][y]]=0;                                           
        }
        break;
      case 2:  //A RSW
        {if(digstate[netnum[x][y+1]]==1)             
            anastate[netnum[x+1][y]]=255-inpin;                                      
        else
            anastate[netnum[x+1][y]]=255;                                           
        }
        break;    
      case 3:  //A+B
        sum=inpin+downpin;
        if(sum>255) sum=255;
        anastate[netnum[x+1][y]]=sum;    
        break;
      case 4:  //A-B
        if(inpin>downpin)       
          sum=inpin-downpin;
        else
          sum=0;         
        anastate[netnum[x+1][y]]=sum;    
        break; 
      case 5:  //B-A
        if(downpin>inpin)       
          sum=downpin-inpin;
        else
          sum=0;         
        anastate[netnum[x+1][y]]=sum;    
        break;  
      case 6:  //A*B       
        sum=downpin*inpin;       
        if(sum>255)
          sum=255;
        anastate[netnum[x+1][y]]=sum;    
        break;    
      case 7:  //A/B       
        if(inpin>downpin)
          {sum=inpin/downpin;               
          anastate[netnum[x+1][y]]=sum;
          }
        else
          anastate[netnum[x+1][y]]=0;
        break;    
      case 8:  //B/A       
        if(downpin/inpin)
          {sum=downpin/inpin;               
          anastate[netnum[x+1][y]]=sum;                
          }
        else
          anastate[netnum[x+1][y]]=0;  
        break;                            
      }
    }
    break;
  case  43:       //Relay    check type before and get direct state for type before        
    anastate[netnum[x][y]]=0;   
    if(digstate[netnum[x][y]]==1)    
      {BoxState[x][y].DeviceState=ON;       
      outstatus[pin]=1; //enable hardware output                                   
      }
    else
      {BoxState[x][y].DeviceState=OFF;           
      outstatus[pin]=0; //enable hardware output    
      }
    break;       
  case  56: //API PHONE 
    if(pin==rxpin && MqttDisplay[pin]>0)// && realtimeloop&0x8>0)    //mqtt output test chance state
      {if(type==56)  
        {topbuf[0]='p';topbuf[1]='h';topbuf[2]='o';topbuf[3]='n';topbuf[4]='e';topbuf[5]=0;                             
        if(MqttDisplay[pin]==1)                     
          {    
          Int2ascii(Payload,anastate[netnum[x][y]]);                                                 
          Payload[0]='d';
          clientloop(topbuf,Payload);   
          MqttDisplay[pin]=0;
          }
        if(MqttDisplay[pin]==2)  
          {
          Payload[3]=digstate[netnum[x][y]]+0x30;
          Payload[4]=BoxState[x][y].DeviceState+0x30;
          Payload[0]='c';
          clientloop(topbuf,Payload);   
          MqttDisplay[pin]=0;
          }                                                                                                         
        }
      }  
      break;   
  case  44: //MQTT TX  
    txtopic=dataz2[x][y]-1;        
    if(dataz0[x-1][y]==25 && (dataz3[x][y+1]&8)==8 && digstate[netnum[x][y+1]]==1)  //analog in             
      {SetAnaValue(txtopic,dataz1[x][y],anastate[netnum[x][y]]);  
      mqttuprdate=1;    
      clientTXStatus[txtopic]=1;
      }  
    if((realtimeloop&0xfff)==0xfff)
    {mqttuprdate=0;
    if(dataz0[x-1][y]==25 && (dataz3[x][y+1]&8)!=8)  //analog in      Ain=25       
      {SetAnaValue(txtopic,dataz1[x][y],anastate[netnum[x][y]]);  
      mqttuprdate=1;    
      clientTXStatus[txtopic]=1;
      }    
    else if(dataz0[x-1][y]==50)  //phone in
      {if(PHONEKEYMODE[y]==4) //value mode
        SetAnaValue(txtopic,dataz1[x][y],anastate[netnum[x][y]]);     
      if(PHONEKEYMODE[y]==1) //on mode
        {BoxState[x][y].DeviceState=ON;
        targetpin=dataz1[x][y];  
        TxIOpinit[txtopic][targetpin-1]=1;      
        }
      if(PHONEKEYMODE[y]==2) //on mode
        {BoxState[x][y].DeviceState=OFF;
        targetpin=dataz1[x][y];  
        TxIOpinit[txtopic][targetpin-1]=0;       
        }
      mqttuprdate=1;  
      clientTXStatus[txtopic]=1;
      }
    else if(dataz0[x-1][y]>25 && dataz0[x-1][y]<37)    //switch in
      {targetpin=dataz1[x][y];  
        if(digstate[netnum[x][y]]==1)          
          TxIOpinit[txtopic][targetpin-1]=1;                  
        else         
          TxIOpinit[txtopic][targetpin-1]=0;          
      mqttuprdate=1;    
      clientTXStatus[txtopic]=1;
      }
    }  
    break;           
  case  24:      //analog out
    if((realtimeloop&0x3f)==0x1f) 
      analogWrite(Pinout[pin-1],anastate[netnum[x][y]]);         
    break;
  case  55:      //mobile out for LAN only old=49  
    if(OUTKeyreached>0 && pin==buttonnum)
      {int t=0;                     
      //getdata100(x,y,displayvalue,100,dataz100g6);     //echo 
      doit=dataz2[x][y];                                                                    
      //if(doit==EchoVal||doit==EchoLog)  //add echo              
      //  {for(t=0;t<14;t++)
      //    {if(displayvalue[t]<=0x20)
      //      {displayvalue[t++]='=';
      //      break;
      //      }
      //    } 
      //  }        
      for(t=0;t<14;t++)
        displayvalue[t]=' ';
      if(doit==Value)                      
        {t=0;
        if(anastate[netnum[x][y]]>100)
          {displayvalue[t++]=(anastate[netnum[x][y]]/100)+0x30;
          displayvalue[t++]=((anastate[netnum[x][y]]/10)%10)+0x30;
          displayvalue[t++]=(anastate[netnum[x][y]]%10)+0x30;
          }
        else if(anastate[netnum[x][y]]>10)  
          {displayvalue[t++]=(anastate[netnum[x][y]]/10)+0x30;
          displayvalue[t++]=(anastate[netnum[x][y]]%10)+0x30;
          }
        else
          displayvalue[t++]=(anastate[netnum[x][y]]%10)+0x30;        
        }                                          
      else if(doit==Logic)  //for logic
        {if(digstate[netnum[x][y]]==1) 
          {displayvalue[2]='H';displayvalue[3]='I';displayvalue[4]='G';displayvalue[5]='H';}
        else
          {displayvalue[2]='L';displayvalue[3]='O';displayvalue[4]='W';}  
        }
      else  
         {displayvalue[0]='e'; displayvalue[1]='r'; displayvalue[2]='r'; }  //show error 
      OUTKeyreached=0;           
      }                                                             
    break;  
  }
}
    //////handle device//////
      
void anykey()
{
Serial.println("ANYKEY");  
while (!Serial.available());
Serial.read();
}