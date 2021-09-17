var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);

colorSetIcon('co_icon_id',        parseInt(document.getElementById("co").innerHTML));
colorSetIcon('temp_room_icon_id', parseInt(document.getElementById("temperature_room").innerHTML));
colorSetIcon('hum_room_icon_id',  parseInt(document.getElementById("humidity_room").innerHTML));
colorSetIcon('pressure_icon_id',  parseInt(document.getElementById("pressure").innerHTML));
colorSetIcon('temp_icon_id',      parseInt(document.getElementById("temperature").innerHTML));
colorSetIcon('hum_icon_id',       parseInt(document.getElementById("humidity").innerHTML));

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage; 
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 500);
}

function onMessage(event) {
    if (event.data == "PowerBalLamp=1") {
        document.getElementById('1').checked = true;
    }
    else if (event.data == "PowerBalLamp=0"){
        document.getElementById('1').checked = false;
    }
    else if (event.data == "PowerHantarex=1") {
        document.getElementById('2').checked = true;
    }
    else if (event.data == "PowerHantarex=0") {
        document.getElementById('2').checked = false;
    }
    else if (event.data == "PowerSpeaker=1") {
        document.getElementById('4').checked = true;
    }
    else if (event.data == "PowerSpeaker=0") {
        document.getElementById('4').checked = false;
    }
    else if (event.data == "PowerSensor=1") {
        document.getElementById('3').checked = true;
    }
    else if (event.data == "PowerSensor=0") {
        document.getElementById('3').checked = false;
    }
    else if (event.data == "RollerBlinds=1") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.225/down", true);
        xhttp.send();
        document.getElementById('5').checked = true;
    }
    else if (event.data == "RollerBlinds=0") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.225/up", true);
        xhttp.send();
        document.getElementById('5').checked = false;
    }
    else if (event.data == "BellMode=1") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.10/on", true);
        xhttp.send();
        document.getElementById('7').checked = true;
    }
    else if (event.data == "BellMode=0") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.10/off", true);
        xhttp.send();
        document.getElementById('7').checked = false;
    }
}
function onLoad(event) {
    initWebSocket();
    initSwitch();
}

function initSwitch() {
    document.getElementById('1').addEventListener('click', switchBalLamp);
    document.getElementById('2').addEventListener('click', switchPowerHantarex);
    document.getElementById('3').addEventListener('click', switchPowerSensor);
    document.getElementById('4').addEventListener('click', switchPowerSpeaker);
    document.getElementById('5').addEventListener('click', switchRollerBlinds);
    document.getElementById('7').addEventListener('click', switchBellMode);
}

function switchPowerHantarex() {
    websocket.send('switchHantarexPower');
}

function switchRollerBlinds() {
    websocket.send('switchRollerBlinds');
}

function switchPowerSpeaker() {
    websocket.send('switchPowerSpeaker');
}


function switchBalLamp() {
    websocket.send('switchBalLamp');
}

function switchPowerSensor() {
    websocket.send('switchPowerSensor');
}

function switchBellMode() {
    websocket.send('switchBellMode');
}

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temperature").innerHTML = this.responseText;
            colorSetIcon('temp_icon_id', this.responseText); 
        }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();

}, 10000);

setInterval(function () {

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity").innerHTML = this.responseText;
            colorSetIcon('hum_icon_id', this.responseText); 
        }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
}, 10000);

setInterval(function () {

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("pressure").innerHTML = this.responseText;
            colorSetIcon('pressure_icon_id', this.responseText);
        }
    };
    xhttp.open("GET", "/pressure", true);
    xhttp.send();
}, 1000);

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temperature_room").innerHTML = this.responseText;
            colorSetIcon('temp_room_icon_id', this.responseText); 
        }
    };
    xhttp.open("GET", "/temperature_room", true);
    xhttp.send();
}, 1000);

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity_room").innerHTML = this.responseText;
            colorSetIcon('hum_room_icon_id', this.responseText); 
        }
    };
    xhttp.open("GET", "/humidity_room", true);
    xhttp.send();

}, 1000);

setInterval(function () {
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("co").innerHTML = this.responseText;
            colorSetIcon('co_icon_id', this.responseText); 
        }
    };
    xhttp.open("GET", "/co", true);
    xhttp.send();

}, 2000);

function colorSetIcon(id_icon, value) {
    if (id_icon == 'co_icon_id') {
        if (value <= 50) {
            document.getElementById(id_icon).setAttribute("style", "color:#02ab3a;");
        }
        else if ((value > 50) && (value <= 100)) {
            document.getElementById(id_icon).setAttribute("style", "color:#dede04;");
        }
        else if ((value > 100) && (value <= 200)) {
            document.getElementById(id_icon).setAttribute("style", "color:#e8a600;");
        }
        else if ((value > 200) && (value <= 300)) {
            document.getElementById(id_icon).setAttribute("style", "color:#e84900;");
        }
        else if (value > 300) {
            document.getElementById(id_icon).setAttribute("style", "color:#ff0505;");
        }
    }
    else if (id_icon == 'temp_room_icon_id') {
        if (value <= 0) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-empty";
            document.getElementById(id_icon).setAttribute("style", "color:#9d00d6;");
        }
        else if ((value > 0) && (value <= 10)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-quarter";
            document.getElementById(id_icon).setAttribute("style", "color:#004ed6;");
        }
        else if ((value > 10) && (value <= 20)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-quarter";
            document.getElementById(id_icon).setAttribute("style", "color:#00add6;");
        }
        else if ((value > 20) && (value <= 25)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-half";
            document.getElementById(id_icon).setAttribute("style", "color:#02ab3a;");
        }
        else if ((value > 25) && (value <= 30)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-three-quarters";
            document.getElementById(id_icon).setAttribute("style", "color:#cfdb1d;");
        }
        else if ((value > 30) && (value <= 35)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-three-quarters";
            document.getElementById(id_icon).setAttribute("style", "color:#db8f1d;");
        }
        else if (value > 35) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-full";
            document.getElementById(id_icon).setAttribute("style", "color:#db3d1d;");
        }
    }
    else if (id_icon == 'hum_room_icon_id') {
        if (value < 30) {
            document.getElementById(id_icon).setAttribute("style", "color:#585e75;");
        }
        else if ((value >= 30) && (value <= 60)) {
            document.getElementById(id_icon).setAttribute("style", "color:#00add6;");
        }
        else if (value > 60) {
            document.getElementById(id_icon).setAttribute("style", "color:#004ed6;");
        }
    }
    else if (id_icon == 'temp_icon_id') {
        if (value <= 0) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-empty";
            document.getElementById(id_icon).setAttribute("style", "color:#9d00d6;");
        }
        else if ((value > 0) && (value <= 10)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-quarter";
            document.getElementById(id_icon).setAttribute("style", "color:#004ed6;");
        }
        else if ((value > 10) && (value <= 20)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-quarter";
            document.getElementById(id_icon).setAttribute("style", "color:#00add6;");
        }
        else if ((value > 20) && (value <= 25)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-half";
            document.getElementById(id_icon).setAttribute("style", "color:#02ab3a;");
        }
        else if ((value > 25) && (value <= 30)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-three-quarters";
            document.getElementById(id_icon).setAttribute("style", "color:#cfdb1d;");
        }
        else if ((value > 30) && (value <= 35)) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-three-quarters";
            document.getElementById(id_icon).setAttribute("style", "color:#db8f1d;");
        }
        else if (value > 35) {
            document.getElementById(id_icon).className = "";
            document.getElementById(id_icon).className = document.getElementById(id_icon).className + "fas fa-thermometer-full";
            document.getElementById(id_icon).setAttribute("style", "color:#db3d1d;");
        }
    }
    else if (id_icon == 'hum_icon_id') {
        if (value < 30) {
            document.getElementById(id_icon).setAttribute("style", "color:#585e75;");
        }
        else if ((value >= 30) && (value <= 90)) {
            document.getElementById(id_icon).setAttribute("style", "color:#00add6;");
        }
        else if (value > 90) {
            document.getElementById(id_icon).setAttribute("style", "color:#004ed6;");
        }
    }
    else if (id_icon == 'pressure_icon_id') {
        if (value < 950) {
            document.getElementById(id_icon).setAttribute("style", "color:#004ed6;");
        }
        else if ((value >= 950) && (value <= 1200)) {
            document.getElementById(id_icon).setAttribute("style", "color:#02ab3a;");
        }
        else if (value > 1200) {
            document.getElementById(id_icon).setAttribute("style", "color:#db8f1d;");
        }
    }
}