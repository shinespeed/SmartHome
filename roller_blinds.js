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
    if (event.data == "RollerBlinds=1") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.130/down", true);
        xhttp.send();
        document.getElementById('5').checked = true;
    }
    else if (event.data == "RollerBlinds=0") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.130/up", true);
        xhttp.send();
        document.getElementById('5').checked = false;
    }
}

function onLoad(event) {
    initWebSocket();
    initSwitch();
    getValueStep();
}

function initSwitch() {
    document.getElementById('5').addEventListener('click', switchRollerBlinds);
}

function switchRollerBlinds() {
    websocket.send('switchRollerBlinds');
}

function updateSliderPWM(element) {
    var sliderValue = document.getElementById("pwmSlider").value;
    console.log(sliderValue);
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://192.168.3.130/slider?value=" + sliderValue, true);
    xhr.send();
}

var lasdValue;

function getValueStep() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("pwmSlider").value = this.responseText;
            lasdValue = this.responseText;
            $('.runner').animate({ height: lasdValue + '%' }, 0);
        }
    };
    xhttp.open("GET", "http://192.168.3.130/status_step", true);
    xhttp.send();
}

$('input[type="range"]').change(function () {

    document.getElementById('5').checked = true;

    var $current = parseFloat($('.bubble').text());
    var $value = parseFloat($(this).val());
    var $min = parseFloat($(this).attr('min'));
    var $max = parseFloat($(this).attr('max'));
    var $diff = Math.abs($current - $value);

    calc = 100 * ($value - $min) / ($max - $min);
    time = 19500 * Math.abs(($value - lasdValue - $min) / ($max - $min));
    $('.runner').stop().delay(1000)
    $('.runner').animate({ height: calc + '%' }, time);
    lasdValue = $value;
 
    if ($diff > 1) {
        $('.bubble').prop('Counter', $current).animate({
            Counter: $value
        }, {
            duration: 100,
            easing: 'swing',
            step: function (now) {
                $(this).text(Math.ceil(now));
            }
        });
    } else {
        $('.bubble').text($value);
    }

});

$('.operators > .increment').click(function () {
    var $input = $('input[type="range"]');
    var $current = parseFloat($input.val());
    $input.val($current + 1);
    $input.trigger('change');
});

$('.operators > .decrement').click(function () {
    var $input = $('input[type="range"]');
    var $current = parseFloat($input.val());
    $input.val($current - 1);
    $input.trigger('change');
});

