static const char PROGMEM INDEX_HTML_mobile[] = R"rawliteral(
<!DOCTYPE html>
<script src='https://cdnjs.cloudflare.com/ajax/libs/qrcodejs/1.0.0/qrcode.min.js'></script>
<body><div id=mobilekey></div>
  <p>LAN Phone URL</p> 
  <h2 id='UTLI'>URL</h2>
  <div id='qrcode'></div><br><br>
  <p>MQTT Phone URL</p>
  <h2 id='MQUTLI'>URL</h2>
  <div id='MQqrcode'></div>  
</body>
<script>
let mxsetdev=6;
let mysetdev=8;
var rowtext = Array.from(Array(mxsetdev), () => new Array(mysetdev)); 
var mcell = new Array(48);
 
function Uploadremotekey()
{var txdata="keys";
var xhtp = new XMLHttpRequest(); 
xhtp.onreadystatechange = function() {
  txre="";  
  if (this.readyState == 4 && this.status == 200) {      
      txre=this.responseText[0]+this.responseText[1]+this.responseText[2];                                 
      if(txre==="keys")        
        console.log('key upload ok');
      }         
  }  
for(var t=0;t<48;t++)
  {var ktext;
  ktext=document.getElementById('rmtt'+t).value;  
  txdata=txdata+ktext+"@";
  }
let length= txdata.length; 
if(length<305)
  {xhtp.open("GET","http://"+URL+"/action?go=" + txdata, true);   
  xhtp.send();  
  } 
}
//////////////////////////////////////////
function toggleCheckbox() {
  var xhr = new XMLHttpRequest();
  var keyv="";
  var k;
  var t=0;
xhr.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {    
    if(this.responseText[3]=='R')
      {for(j=4;j<305;j++)
          {if(this.responseText[j]=='@')// && this.responseText[j+2]=='%')
              {if(t<48)
                {document.getElementById('rmtt'+t).value=keyv;
                j++;
                t++;
                keyv="";                
                }             
              }
            keyv=keyv+this.responseText[j];   
          }   
      }
    if(this.responseText[3]=='o')
      {
      document.getElementById('disp').value=this.responseText[4]+this.responseText[5]; 
      }  
    } 
  }  
xhr.open("GET","http://"+URL+"/action?go=keyr", true);
xhr.send();
}


///////////////////////////////////////////////////////
function loaddefaultA()
{//loadkeytable2();
for(var t=0;t<mysetdev;t++)
  {for(var j=0;j<mxsetdev;j++)
    {if(j<mxsetdev-1)
        document.getElementById('rmtt'+(j+t*mxsetdev)).value=t+1;   
      else  
        document.getElementById('rmtt'+(j+t*mxsetdev)).value=100;              
    }
  }        
}

function loaddefaultB()
{//loadkeytable2();
for(var t=0;t<mysetdev;t++)
  {for(var j=0;j<mxsetdev;j++)
    {
      if(j<mxsetdev-1)
        document.getElementById('rmtt'+(j+t*mxsetdev)).value=(t+1)*10000;   
      else  
        document.getElementById('rmtt'+(j+t*mxsetdev)).value=100;   
    }
  }    
}


function loadkeytable1(){             
  for(let y=0;y<mysetdev;y++)  
    {var row = document.getElementById('myDRow'+y.toString());   
    for(var t=0;t<mxsetdev;t++)
      row.deleteCell(0);      
    }  
  loadkeytable();  
  }  

function loadkeytable(){ 
  loadkeytable2();  
  //toggleCheckbox(999);      
}

function loadkeytable2(){    
  for(let t=0;t<mysetdev;t++)   
    {   rowtext[0][t] = "<td><label>A"+t+"</label><input class='rmtt' id='rmtt"+(0+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[0+t*mxsetdev]+"' style='width:50px;height:22px;'></td>"
        rowtext[1][t] = "<td><label>B"+t+"</label><input class='rmtt' id='rmtt"+(1+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[1+t*mxsetdev]+"' style='width:50px'height:25px;'></td>"  
        rowtext[2][t] = "<td><label>C"+t+"</label><input class='rmtt' id='rmtt"+(2+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[2+t*mxsetdev]+"' style='width:50px'height:25px;'></td>"             
        rowtext[3][t] = "<td><label>D"+t+"</label><input class='rmtt' id='rmtt"+(3+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[3+t*mxsetdev]+"' style='width:50px;height:22px;'></td>"
        rowtext[4][t] = "<td><label>E"+t+"</label><input class='rmtt' id='rmtt"+(4+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[4+t*mxsetdev]+"' style='width:50px'height:25px;'></td>"         
        rowtext[5][t] = "<td><label>F"+t+"</label><input class='rmtt' id='rmtt"+(5+t*mxsetdev)+"' maxlength='5' type='text' value='"+mcell[5+t*mxsetdev]+"' style='width:50px'height:25px;'></td>"   
    }     
  for(let j=0;j<mysetdev;j++)  
      {row = document.getElementById('myDRow'+j.toString());         
      x0 = row.insertCell(0);  
      x1 = row.insertCell(1); 
      x2 = row.insertCell(2);   
      x3 = row.insertCell(3);  
      x4 = row.insertCell(4);   
      x5 = row.insertCell(5);   
      x0.innerHTML = rowtext[0][j];  //
      x1.innerHTML = rowtext[1][j];  //   
      x2.innerHTML = rowtext[2][j];  //    
      x3.innerHTML = rowtext[3][j];  //
      x4.innerHTML = rowtext[4][j];  //     
      x5.innerHTML = rowtext[5][j];  //     
      }          
      toggleCheckbox(999);      
    }


var mobilekey=
"<h3>Input key character show on keyboard</h3>\
<table class='devtable'><tbody>\
<tr id='myDRow0'></tr><tr id='myDRow1'></tr><tr id='myDRow2'></tr><tr id='myDRow3'>\
</tr><tr id='myDRow4'></tr><tr id='myDRow5'></tr><tr id='myDRow6'></tr><tr id='myDRow7'></tr>\
</tbody></table><br>\
<button type='button' class='mkey' onclick='Uploadremotekey()' style='margin-left:50px'>Upload remote keyboard</button><br>\
<button type='button' class='mkey' onclick='loadkeytable1()' style='margin-left:50px'>refresh or cancel input</button>\
<button type='button' class='mkey' onclick='loaddefaultA()' style='margin-left:50px'>Load default A</button>\
<button type='button' class='mkey' onclick='loaddefaultB()' style='margin-left:50px'>Load default B</button>\
<h3>keypad specification</h4>\
<h4>All 24 keys can assign 1 to 4 character</h4>\
<h4>Avoid dupicate program will not check this</h4>\
<h3>Special key</h4>\
<h4>The last 3 rows are special keys, total 9 keys</h4>\
<h4>Other can free assign function in Ladder program</h4>"

window.onload = async function(){       
  URL=localStorage.getItem('localurl');
  console.log('11ter',mcell);  
  document.getElementById('mobilekey').innerHTML=mobilekey;   
  loadkeytable(); 
  }
var qrcode = new QRCode('qrcode',window.location.host);
document.getElementById('UTLI').innerText=URL=window.localStorage.getItem('localurl');
var MQqrcode = new QRCode('MQqrcode','http://ipcworld.online/phone/phonepad.html');
document.getElementById('MQUTLI').innerText='http://108.199.198.19:3300/phonepad';
</script>
<style>
.rmtt
{height: 25px;
background-color: beige;
text-align: center;
}
</style>
</html>  
)rawliteral";
