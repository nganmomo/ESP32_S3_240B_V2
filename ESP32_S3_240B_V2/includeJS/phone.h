static const char PROGMEM INDEX_HTML_phone[] = R"rawliteral(
<html>
<head><link href='http://ipcworld.online/views/image/favicon.ico' type='image/x-icon' rel='shortcut icon'/></head>
<title>ESP32-CAM Robot</title>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<body>
<h1>MAXTRON PLC</h1>
<h5>Desktop only</h5>
<!--textarea id='disp' class='disp' name='display' rows='1' cols='16'></textarea-->
<input  type='text' id='disp' onclick='inputclick()' class='disp' minlength='4' maxlength='14' size='14'></input>
<table>
<tr><td align='center'>
<button class='button but' id='rmt1' onclick='toggleCheckbox(0);'>ON</button>
<button class='button but' id='rmt2' onclick='toggleCheckbox(0);'>OFF</button>
<button class='button but' id='rmt3' onclick='toggleCheckbox(0);'>PULSE</button>
<button class='button but' id='rmt4' onclick='toggleCheckbox(0);'>VAL</button>
<button class='button but' id='rmt5' onclick='toggleCheckbox(0);'>DISP</button>
</td></tr>
<tr><td align='center'>
<button class='button but1' id='rmt11' onclick='toggleCheckbox(11);'>1</button>
<button class='button but1' id='rmt12' onclick='toggleCheckbox(12);'>1</button>
<button class='button but1' id='rmt13' onclick='toggleCheckbox(13);'>1</button>
<button class='button but1' id='rmt14' onclick='toggleCheckbox(14);'>1</button>
<button class='button but1' id='rmt15' onclick='toggleCheckbox(15);'>1</button>
</td></tr>
<tr><td align='center'>
<button class='button but2' id='rmt21' onclick='toggleCheckbox(21);'>2</button>
<button class='button but2' id='rmt22' onclick='toggleCheckbox(22);'>2</button>
<button class='button but2' id='rmt23' onclick='toggleCheckbox(23);'>2</button>
<button class='button but2' id='rmt24' onclick='toggleCheckbox(24);'>2</button>
<button class='button but2' id='rmt25' onclick='toggleCheckbox(25);'>2</button>
</td></tr>
<tr><td align='center'>
<button class='button but3' id='rmt31' onclick='toggleCheckbox(31);'>3</button>
<button class='button but3' id='rmt32' onclick='toggleCheckbox(32);'>3</button>
<button class='button but3' id='rmt33' onclick='toggleCheckbox(33);'>3</button>
<button class='button but3' id='rmt34' onclick='toggleCheckbox(34);'>3</button>
<button class='button but3' id='rmt35' onclick='toggleCheckbox(35);'>3</button>
</td></tr>
<tr><td align='center'>
<button class='button but4' id='rmt41' onclick='toggleCheckbox(41);'>4</button>
<button class='button but4' id='rmt42' onclick='toggleCheckbox(42);'>4</button>
<button class='button but4' id='rmt43' onclick='toggleCheckbox(43);'>4</button>
<button class='button but4' id='rmt44' onclick='toggleCheckbox(44);'>4</button>
<button class='button but4' id='rmt45' onclick='toggleCheckbox(45);'>4</button>
</td></tr>
<tr><td align='center'>
<button class='button but5' id='rmt51' onclick='toggleCheckbox(51);'>5</button>
<button class='button but5' id='rmt52' onclick='toggleCheckbox(52);'>5</button>
<button class='button but5' id='rmt53' onclick='toggleCheckbox(53);'>5</button>
<button class='button but5' id='rmt54' onclick='toggleCheckbox(54);'>5</button>
<button class='button but5' id='rmt55' onclick='toggleCheckbox(55);'>5</button>
</td></tr>
<tr><td align='center'>
<button class='button but6' id='rmt61' onclick='toggleCheckbox(61);'>6</button>
<button class='button but6' id='rmt62' onclick='toggleCheckbox(62);'>6</button>
<button class='button but6' id='rmt63' onclick='toggleCheckbox(63);'>6</button>
<button class='button but6' id='rmt64' onclick='toggleCheckbox(64);'>6</button>
<button class='button but6' id='rmt65' onclick='toggleCheckbox(65);'>6</button>
</td></tr>
<tr><td align='center'>
<button class='button but7' id='rmt71' onclick='toggleCheckbox(71);'>7</button>
<button class='button but7' id='rmt72' onclick='toggleCheckbox(72);'>7</button>
<button class='button but7' id='rmt73' onclick='toggleCheckbox(73);'>7</button>
<button class='button but7' id='rmt74' onclick='toggleCheckbox(74);'>7</button>
<button class='button but7' id='rmt75' onclick='toggleCheckbox(75);'>7</button>
</td></tr>
<tr><td align='center'>
<button class='button but8' id='rmt81' onclick='toggleCheckbox(81);'>8</button>
<button class='button but8' id='rmt82' onclick='toggleCheckbox(82);'>8</button>
<button class='button but8' id='rmt83' onclick='toggleCheckbox(83);'>8</button>
<button class='button but8' id='rmt84' onclick='toggleCheckbox(84);'>8</button>
<button class='button but8' id='rmt85' onclick='toggleCheckbox(85);'>8</button>
</td></tr>
</table>
</body>
<style>
  body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px; background-color: rgb(196, 189, 184);}
  table { margin-left: auto; margin-right: auto; }
  td { padding: 8 px; }
  .button {    
    border: none;
    color:  rgb(214, 207, 202);
    padding: 5px 5px;
    text-align: justfy;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 1px 3px;
    width:50px;
    height:30px;
    cursor: pointer;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
  }
  .but {
    background-color: rgb(196, 189, 184);
    color:rgba(7, 7, 6, 0.788);
  }
  .but1 {
    background-color: #2f4468;    
  }
  .but2 {
    background-color: rgb(73, 67, 63);   
  }
  .but3 {
    background-color: rgb(128,134,128);  
  }
  .but4 {
    background-color: rgb(87,105,53);   
  }
  .but5 {
    background-color: rgb(121,37,37);  
  } 
  .but6 {
    background-color: rgb(138, 190, 41);   
  }
  .but7 {
    background-color: rgb(87, 59, 212);  
  } 
  .but8 {
    background-color: rgb(206, 111, 111);  
  }   
  .disp{text-align: center;
    font-size: 28px;
    background-color: chocolate;
    border-color:black;
    border-width: 2px;
    margin-bottom: 15px
    }
</style>
<script>
var ratio=Array(8).fill("100"); 
var kvalue=Array(24).fill(" "); 
var keyv="";
var keyvout="";

function inputclick()
{
  document.getElementById('disp').value=""; 
}

var  txed="";
var  norepeat=0;
var  keyno;
var  keytype;

function toggleCheckbox(x)
{if(x<90)
  {keyno=parseInt(x/10)-1;
  toggleCheck(x);
  norepeat=1;
  keytype=x%10;
  }
else
  {const myTimeout = setTimeout(getdispay,500);      
  }
}

function getdispay()
{if(norepeat===1)
  {toggleCheck("08Disp"); //show display
    norepeat=0;
  }  
}

function toggleCheck(x) {
if(keyv.length>20)
  keyv="";
kvalue[x]; 
var xhr = new XMLHttpRequest();
var j;
var t=0;
let hx=x/10;
let lx=x%10;  
keyvout=keyv;
keyv=""; 
xhr.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {      
    /*
    if(this.responseText[3]=='|')           //update key value
      {for(j=3;j<120;j++)
          {if(this.responseText[j]!='%')  //) && this.responseText[j]!='%')          
              keyv=keyv+this.responseText[j];   
          else
              keyv=" ";                  
          if(this.responseText[j+1]=='%')// && this.responseText[j+2]=='%')
              {document.getElementById('rmt'+t).innerText=keyv;
              kvalue[t]=keyv;
              j=j+2;
              keyv="";
              if(t++>=23)
                break;
              }
          }   
      }      
    */  
    if(this.responseText[3]==='d')     //response to display o 4-18 position
      {let disptext="";
      for(let t=4;t<18;t++)
        {if(this.responseText[t]===undefined)
          break;
        if(this.responseText[t]==="u0000")
          continue;   
        disptext=disptext+this.responseText[t];
        }            
      if(keytype===5)
        document.getElementById('disp').value=(disptext*ratio[keyno]/255).toFixed(2);
      else
        document.getElementById('disp').value=disptext;
      }
    else
      document.getElementById('disp').value="";    
    }   
  txed= this.responseText[0]+this.responseText[1]+this.responseText[2]+this.responseText[3];
  if(txed==='mobo')
    toggleCheckbox("18Disp");  //request display
  }
  //if(x===888)   
  //  xhr.open('GET','/action?go=mobd' + x, true);            //require display no repeat
  //else if(x===999)   
  var disptext=document.getElementById('disp').value;
  if(disptext==="")
  disptext="0";
  xhr.open("GET","http://"+URL+"/action?go=mobo" + x + disptext, true);            //update keyboard character 
  xhr.send();    
}
window.onload = async function(){
  URL=localStorage.getItem('localurl');  
  updatekeytext();
  }
//////////////////////////////////////////
function updatekeytext() {
  var xhr = new XMLHttpRequest();
  var keyv="";
  var k=1;
  var t=10;
xhr.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {    
    if(this.responseText[3]=='R')
      {for(j=4;j<305;j++)
          {if(this.responseText[j]=='@')// && this.responseText[j+2]=='%')
              {if(t<90)
                {if(k<6)
                  document.getElementById('rmt'+(k+t)).innerText=keyv;                
                else
                  ratio[(k/6)-1]=keyv;  
                if(k%10===6)  
                  {t=t+10;
                  k=0;
                  }
                k++;                            
                keyv="";                
                }             
              }
          else    
            keyv=keyv+this.responseText[j];   
          }   
      }
    if(this.responseText[3]=='o')
      {
      document.getElementById('disp').value=this.responseText[4]+this.responseText[5]; 
      }  
    } 
  }  
//xhr.open('GET','/action?go=mobi' + x, true);
xhr.open("GET","http://"+URL+"/action?go=keyr", true);
xhr.send();
}
</script>
</html>
)rawliteral";