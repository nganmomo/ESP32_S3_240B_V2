static const char PROGMEM INDEX_HTML_PLC[] = R"rawliteral(
<!DOCTYPE html>
<head>
<title>Network Programmable Logic control</title>
<link rel="icon" type="image/x-icon" href="http://ipcworld.online/views/image/favicon.ico">
</head>
<script src="http://192.168.1.186:8088/hwsim"></script>   
<script src="http://192.168.1.186:8088/svgPick"></script>
<script src="http://192.168.1.186:8088/svgplccell"></script>    
<script src="http://192.168.1.186:8088/icontext"></script>  
<script src="http://192.168.1.186:8088/Property"></script> 
<!--script src="http://192.168.1.186:8088/plcsvgut"></script-->    
<script src="http://192.168.1.186:8088/Linkhardware"></script>
<script src="helppopup"></script>
<!--script src="http://ipcworld.online/helppopup.js"></script-->
<link rel="stylesheet" href="http://192.168.1.186:8088/plcsvgcss">    
<!--link rel="stylesheet" href="http://ipcworld.online/css/plcsvg.css"-->      
<meta name="viewport" content="width=device-width, initial-scale=1">
<body>

<div class="navbar">
  <!--a class="active" href="#home">Home</a>
  <a href="#news">News</a>
  <a href="#contact">Contact</a>
  <a href="#about">About</a--> 
    <button type="button" class="botstatus" id="botstatus" onclick="CheckHWStatus()">STATUS</button>
    <button type="button" class="plcedit" id="plcedit" onclick="showedit(1)" >Edit page</button>   
    <button type="button" class="botcl" onclick="clearfile()" >clear</button>    
    <button type="button" class="botsim" onclick="uploadesp('swur',0)">status probe</button> 
    <button type="button" class="botdl" onclick="DownloadFromHW('start')">Download</button>
    <button type="button" class="botul" id="botul";  onclick="UploadToHW()" >upload</button>      
    <button type="button" class="labelul"  id="labelul"></button>   
    <button type="button" class="about" onclick="Aboutpopup()" id="about">About</button>    
</div>

<div class="sidebar">
    <div class="scaled-content">
      <div class="textw" id="textw"></div>
      <div class="container" id="piccont">
      <div id="SHOWSVG"></div>
      </div>      
      <div class="container" id="hintscont">
      <div id="HINTS"></div>               
      </div>   
    </div>
    <!--a href="#">Home</a>
    <a href="#">About</a>
    <a href="#">Services</a>
    <a href="#">Portfolio</a>
    <a href="#">Contact</a-->
</div>

<!-- Page Content -->
<div class="content">                                     
  <div class="container" id="tablecont">              
  <div id="plcsvgmain"></div> 
  </div><!--end of tablecont-->         
  <div id="popup-overlay" class="overlay"></div>
  <div class="popup" id="popup"></div>                  
</div><!--end of content-->        
<!-- Bottom Bar -->
<div class="bottombar">   
    <!--button type="button" class="bqrcode" onclick="changeframe(1)">QRcode</button-->    
    <button type="button" class="botmqtt" onclick="changeframe(2)">Mqtt</button>
    <button type="button" class="botphone" onclick="changeframe(3)">Phone</button>
    <button type="button" class="botfile" onclick="changeframe(4)">File</button>    
    <button type="button" class="EDIT" id="EDIT";  onclick="showedit(0)" >EDIT</button>  
</div>
<dialog id="dialog">     
</body>
</html>


<script>
var systemtype=0;
var URL;
var Showtable = "";
var divc1 = document.getElementById('piccont'); 
var divc2 = document.getElementById('tablecont'); 
divc1.addEventListener('contextmenu', (e) => {e.preventDefault()});   
divc2.addEventListener("contextmenu", (e) => {e.preventDefault()});   
window.onload = async function(){            
  if(systemtype===0)
    {URL=window.location.host;
    window.localStorage.setItem('localurl',URL);  
    }
  updatacell();  
  if (localStorage.getItem('plcdata') != null)
    {serialcelldata=JSON.parse(localStorage.getItem('plcdata')); 
    //addtext();
    }
  else  
    serialcelldata = Array(1920).fill("");    
  updatetable(FILLALL);  
  //var w = window.innerWidth;  
  var h = window.innerHeight;
  document.getElementById('SHOWSVG').innerHTML=svgdata;
  document.getElementById('HINTS').innerHTML=svghints[0];
  document.getElementById('hintscont').style.height=h-520+'px'; 
  //CheckHWStatus();
  document.getElementById('textw').innerHTML="Pick one to place";
  showedit(1); //close edit bar 
}  

var dailog=document.getElementById("dialog"); 
function openModal(htmltext) { 
    document.getElementById("dialog").innerHTML="<br><button onclick='closeModal()' class='bothelp'>Close</button><br><br>"+htmltext; 
    dailog.showModal();
} 

function closeModal() { 
    dailog.close(); 
}    

function showedit(x)
{if(x===1)
  {//updatacell();  
  if (localStorage.getItem('plcdata') != null)
    {serialcelldata=JSON.parse(localStorage.getItem('plcdata')); 
    //addtext();
    }
  else  
    serialcelldata = Array(1920).fill("");    
  if(localStorage.getItem('refreshpage') === "required")
    {localStorage.setItem('refreshpage',"notneed");
    UploadToHW();
    }
  }
  if(svgwidth===1030 || x===1)
    {svgwidth=962;
    document.getElementById('tablecont').style.width=974+'px';       
    }
  else
    {svgwidth=1030;
    document.getElementById('tablecont').style.width=1042+'px';   
    }
updatetable(FILLALL);  
}
</script>
)rawliteral";
