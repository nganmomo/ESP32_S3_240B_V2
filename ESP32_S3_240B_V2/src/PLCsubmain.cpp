#include "plcsvg.h"
void saveint16Clear(uint16_t (*dataz)[40]);
char keyvalue[20];
  void receivedplcdata()
  {numchartowrite=6; 
  if(variable[0]=='t' && variable[1]=='x')  
    {if(variable[2]=='A')               //is sign
        {saveint16(variable,dataz0,0,20);  
        saveint16Clear(dataz0);      
        }        
    if(variable[2]=='B')                //is id
        {saveint16(variable,dataz0,20,40);                                  
        }
    if(variable[2]=='C')                //is link  
        {saveint16(variable,dataz1,0,20);    
        saveint16Clear(dataz1);          
        }
    if(variable[2]=='D')                //is link
        {saveint16(variable,dataz1,20,40);           
        }
    if(variable[2]=='E')                //is set
        {saveint16(variable,dataz2,0,20);              
        saveint16Clear(dataz2); 
        }                      
    if(variable[2]=='F')               //is sign
        {saveint16(variable,dataz2,20,40);       
        }        
    if(variable[2]=='G')                //is id
        {saveint16(variable,dataz3,0,20);          
        saveint16Clear(dataz3); 
        }
    if(variable[2]=='H')                //is link  
        {saveint16(variable,dataz3,20,40);              
        }
    if(variable[2]=='J')                //is link
        {saveint16(variable,dataz4,0,20);                
        //k uncondition//
        }
    if(variable[2]=='K')                //is set
        {saveint16(variable,dataz4,20,40);                                       
        eepromwite(adddataz0,dataz0,0);
        eepromwite(adddataz1,dataz1,0);
        eepromwite(adddataz2,dataz2,0);
        eepromwite(adddataz3,dataz3,0);
        eepromwite(adddataz4,dataz4,2);                       
        }            
    }          
  }     
     
void receivedplc(int eedata,int maxlen,char rw,int offset)     
{     //just store for comment topic payload   
  eedata=eedata+offset;           
  if(rw=='U')
    {numchartowrite=6;     
    eewbyte(eedata,&variable[5],maxlen);       
    }   
  if(rw=='D')           
    {numchartowrite=maxlen+6;
    eerbyte(eedata,&variable[5],maxlen);
    variable[3]='D';   
    }            
}                
char renum;
char remode;
byte cellkeyboard()           
{//update keypad character if key number =99 //mobile control mobo mobi
  int t=0; 
  if(variable[4]>0x30)
    {renum=variable[4];
    remode=variable[5]; 
    }
  buttonnum=variable[4]-0x30; //0-8
  buttonmode=variable[5]-0x30; //0-5
  if(buttonmode<5)  //for output value
  {if(buttonnum>=0 && buttonnum<=8) //next step to 50
    {PhoneKeyIn=buttonnum;        
    }         
  }
  if(buttonmode==5) //for dispaly value
  {if(buttonnum>=0 && buttonnum<=8)
    {OUTKeyreached=2;     
    }         
  }
  if(buttonmode==8)
    {if(remode=='5') 
      {for(t=0;t<14;t++)
        variable[t+4]=displayvalue[t];
      variable[3]='d';  //return 'mobd' 
      buttonmode=10; //disp once          
      }
    else
      {for(t=0;t<14;t++)
        variable[t+5]=' ';
      variable[4]='O';
      variable[5]='K';
      variable[7]=renum;     
      variable[3]='d'; 
      }  
    }  
  return   t;
}


      
void switchsimulate(byte stateonly)   //switch simulation 
{//switch update       
  int x,y,z,t;
  byte lasty;
  byte realtimeloop;  
  if(stateonly==0)    
    {if(variable[3]=='i')
      {Endigitalread=0;      //switch emulate
      x=variable[4]-0x30; //x
      x=variable[4]-0x30; //x
      y=variable[5]-0x30; //Y               
      z=variable[6]-0x30; //on or off
      t=variable[7]-0x30; //edge or static       
      BoxState[x][y].DeviceState=z; //on off control      
      for(int j=0;j<2;j++)  //do 3 times
      lasty=GetlastScan();
      updateinstate(0,lasty);   //execute simulate once          
      if(t==1) //edge or static
        {BoxState[x][y].DeviceState=0; 
        updateinstate(0,lasty);     //execute simulate once               
        }            
        rwswiches(writeonly);  
      }
    }    
for(y=0;y<ycount;y++)  //30 convert digital to ascii by add 0x30
  {byte type,pin;
  for(x=0;x<xcount;x++)    //240      
    {if(anastate[netnum[x][y]]>0)      //if > 0 means analog
        variable[y*6+x+10]='2';        //net color orange 
    else
        variable[y*6+x+10]=digstate[netnum[x][y]]+0x30;   //net type color 10 is offset   
    variable[y*6+x+250]=BoxState[x][y].DeviceState+0x30;   //device state   70  
    pin=dataz1[x][y];      
    if(type=dataz0[x][y]==40)
      {   //show D?A output
      variable[y*6+x+490]=(d2abuf[pin]/100)+0x30;//BoxState[x][y].status1+0x30;      //not use 13
      variable[y*6+x+730]=(d2abuf[pin]/10)%10+0x30;//BoxState[x][y].status2+0x30;    //not use 19
      variable[y*6+x+970]=(d2abuf[pin]%10)+0x30;//BoxState[x][y].status3+0x30;       //not use 25
      variable[y*6+x+1210]='A';//BoxState[x][y].status4+0x30;      //not use          31
      }    
    if(type=dataz0[x][y]==38)
      {   //show count
      variable[y*6+x+490]=(counter[pin]/100)+0x30;//BoxState[x][y].status1+0x30;      //not use
      variable[y*6+x+730]=(counter[pin]/10)%10+0x30;//BoxState[x][y].status2+0x30;    //not use
      variable[y*6+x+970]=(counter[pin]%10)+0x30;//BoxState[x][y].status3+0x30;       //not use
      variable[y*6+x+1210]='A';//BoxState[x][y].status4+0x30;      //not use          
      }
    else  //show analog value
      {variable[y*6+x+490]=(anastate[netnum[x][y]]/100)+0x30;//BoxState[x][y].status1+0x30;      //not use
      variable[y*6+x+730]=(anastate[netnum[x][y]]/10)%10+0x30;//BoxState[x][y].status2+0x30;     //not use
      variable[y*6+x+970]=(anastate[netnum[x][y]]%10)+0x30;//BoxState[x][y].status3+0x30;        //not use
      variable[y*6+x+1210]='A';//BoxState[x][y].status4+0x30;      //not use          
      }
    }       
  }     
variable[0]='S';variable[1]='W';variable[2]='U';  //switch update
numchartowrite=1211;       
}   

void systemsetup()
{int Length;           
  mqttrun=0; 
  if(variable[3]=='U')
    {numchartowrite=6;    
    eewbyte(MQSTdata,variable,500);   
    variable[3]='U';        
    }
  if(variable[3]=='D')  
    {numchartowrite=500;
    eerbyte(MQSTdata,variable,500);
    variable[3]='D';
    }        
variable[0]='m';   
mqttrun=1;      
}    

void receivedplcTable() //RET
  {uint16_t tdata,t=4;
  char idx=variable[3]-0x30;
  byte y,x;
  for(y=0;y<ycount;y++)
    {for(x=0;x<xcount;x++)    
        {if(idx==0)     tdata=dataz0[x][y];
        else if(idx==1)  tdata=dataz1[x][y];
        else if(idx==2)  tdata=dataz2[x][y];
        else if(idx==3)  tdata=dataz3[x][y];
        else if(idx==4)  tdata=dataz4[x][y];                
        variable[t++]='%';                       
        if(tdata>10000) variable[t++]=(tdata/10000)+0x30;  
        if(tdata>1000) variable[t++]=(tdata/1000)%10+0x30;                 
        if(tdata>100) variable[t++]=(tdata/100)%10+0x30;  
        if(tdata>10) variable[t++]=(tdata/10)%10+0x30;  
        variable[t++]=tdata%10+0x30;                 
        }
    }
  variable[t++]='%'; 
  variable[t++]='*';      
  numchartowrite=t;        
  }    

void saveint16Clear(uint16_t (*dataz)[40])
  {
  for(int t=20;t<ycount;t++) //fill the rest
    {for(int s=0;s<xcount;s++)
      dataz[s][t]=0;
    }
  }


void saveint16(char *variable,uint16_t (*dataz)[40],byte start,byte end)
  {int16_t gt[4];
  byte k=0;
  byte i,t;
  byte first=1;
  byte x=0;  
  byte y=start;
  if(start==0)
    {for(t=start;t<ycount;t++) //fill the rest
      {for(int s=0;s<xcount;s++)
        dataz[s][t]=0;
      }
    }  
    for(i=0;i<1200;i++)
    {if(variable[i]=='*')
        break;
    if(variable[i]==',') 
        {if(first==1)
          {first=0;  continue;}
        if(k==4)
          dataz[x][y]=gt[0]*1000+gt[1]*100+gt[2]*10+gt[3];           
        if(k==3)
          dataz[x][y]=gt[0]*100+gt[1]*10+gt[2];
        if(k==2)
          dataz[x][y]=gt[0]*10+gt[1];  
        if(k==1)  
          dataz[x][y]=gt[0];
        if(k==0)  
          dataz[x][y]=0;
        k=0;                 
        x++;        
        if(x>=xcount) {y++; x=0;}
        if(y>=end) y=start; 
        }
    else          
        {if(variable[i]>0x39 || variable[i]<0x30)      continue;      //non figure continue 
          gt[k++]=variable[i]-0x30;
        }
    }      
  //for(t=start;t<end;t++)
  //  {for(int s=0;s<xcount;s++)
  //    {Serial.print(dataz[s][t],DEC);   
  //    Serial.print("<");    
  //    }
  //  }        
  }

//byte leng length of string sat number or char for ||mqtt only||
byte getdata(byte gx,byte gy,char* rebuf,byte sat,byte leng)
{for(int j=0;j<leng;j++) //fill table default value
    {if(j<sat)
    rebuf[j]='0'; //ascii
    else
    rebuf[j]=0;   //end sign 
    }
char tyre[500];
eerbyte(MQSTdata,tyre,500);
int t,x=0,y=0;  
for(int t=4;t<300;t++)
  {if(tyre[t]=='%') //&& tyre[t+1]=='%')
    {if(x++>=3)         //x 5   the table has 3 col
      {x=0;y++;
      if(y>=22) y=0;    //y     the table has 24 row  
      }
    //t++;     
    }
  else
    {if(gy==y && gx==x)                
      {rebuf[sat++]=tyre[t];  
      if(sat>=leng)    
        break;                                  
      }
    }  
  }
return sat;
}

byte getdata100(int gx,int gy,char* rebuf,int length,char *pt,char endsign)
{int tc=0,x=0,y=0;
int count=0,ti=0;
int tgetp=gy*6+gx;
for(int j=0;j<12;j++) //fill table default value   
    rebuf[j]=endsign;   //end sign    
for(tc=4;tc<length;tc++)
  {//if(pt[tc]=='^'&& pt[tc+1]=='%')   
  if(pt[tc]=='%')
    {if(count++>=tgetp)  
      {if(pt[tc+2]!='%') 
        {tc=tc+2;          
        for(ti=0;ti<12;ti++)
            {if(pt[tc+ti]=='%')
              break;
            rebuf[ti]=pt[tc+ti];                   
            }       
        break;         
        }
      } 
    }     
  }   
return ti;     
}
 
byte checkmqtt()
{byte x,y;
byte mqttexist=0;
for(x=0;x<xcount;x++)
  for(y=0;y<ycount;y++)
    {if(dataz0[x][y]==44 || dataz0[x][y]==45)
      {mqttexist=1;
        break;
      }
    }
if(mqttexist==1 && (mqtten==0 || mqttconnected==0)) 
  return 1; //fault return
else
  return 0;  
} 
