var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var lasdValue;
var delayAnimate = false;

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
        getValueStep();
    }
    else if (event.data == "RollerBlinds=0") {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "http://192.168.3.130/up", true);
        xhttp.send();
        document.getElementById('5').checked = false;
        getValueStep();
    }
    else if (event.data == "stateRollerBlinds=1") {
        document.getElementById('5').checked = true;
    }
    else if (event.data == "stateRollerBlinds=0") {
        document.getElementById('5').checked = false;
    }
    else if (event.data == "updateStateBlinds=1") {
        updateValueStepS();
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
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "http://192.168.3.130/slider?value=" + sliderValue, true);
    xhr.send();
}

function updateValueStepS() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("pwmSlider").value = this.responseText;
            $('.bubble').text(this.responseText);
            lasdValue = this.responseText;
            $('.runner').animate({ height: lasdValue + '%' }, 0);
        }
    };
    xhttp.open("GET", "http://192.168.3.130/status_step", true);
    xhttp.send();
}

function getValueStep() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("pwmSlider").value = this.responseText;
            $('.bubble').text(this.responseText); 
            lasdValue = this.responseText;
            $('.runner').animate({ height: lasdValue + '%' }, 0);
        }
    };
    xhttp.open("GET", "http://192.168.3.130/status_step", true);
    xhttp.send();
}

function setValueStep() {

}

$('input[type="range"]').change(function () {

    lasdValue = $('.runner').height() * 0.4;
    console.log(Math.abs($('.runner').height() * 0.4));

    var $current = parseFloat($('.bubble').text());
    var $value = parseFloat($(this).val());
    var $min = parseFloat($(this).attr('min'));
    var $max = parseFloat($(this).attr('max'));
    var $diff = Math.abs($current - $value);


    calc = 100 * ($value - $min) / ($max - $min);
    if ($value > lasdValue) {
        time = 19000 * Math.abs(($value - lasdValue - $min) / ($max - $min));
    } else {
        time = 21000 * Math.abs(($value - lasdValue - $min) / ($max - $min));
    }

    if (delayAnimate == true) {
        $('.runner').stop(true);
    } else {
        $('.runner').stop(true).delay(1000);
    }

    delayAnimate = true;
    $('.runner').animate({ height: calc + '%' }, time).promise().done(function () {
        delayAnimate = false;
    });

    if ($value > 0) {
        websocket.send('RollerBlinds=1');
    } else {
        websocket.send('RollerBlinds=0');
    }

    websocket.send('updateStateBlinds');

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

