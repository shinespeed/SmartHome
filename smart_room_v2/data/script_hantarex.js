var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);

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
    if (event.data == "PowerHantarex=1") {
        document.getElementById('2').checked = true;
        document.getElementById('4').checked = true;
    }
    else if (event.data == "PowerHantarex=0") {
        document.getElementById('2').checked = false;
    }
    else if (event.data == "MuteHantarex=1") {
        document.getElementById('4').checked = true;
    }
    else if (event.data == "MuteHantarex=0") {
        document.getElementById('4').checked = false;
    }
}
function onLoad(event) {
    initWebSocket();
    initSwitch();
}

function initSwitch() {
    document.getElementById('2').addEventListener('click', switchPowerHantarex);
    document.getElementById('4').addEventListener('click', switchMuteHantarex);
}

function switchPowerHantarex() {
    websocket.send('switchHantarexPower');
}

function switchMuteHantarex() {
    websocket.send('switchMuteHantarex');
}

function reply_click(clicked_id) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/" + clicked_id, true);
    xhr.send();
    console.log(clicked_id);
}