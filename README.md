# SmartHome
This project has three related parts based on esp32 and esp8266 microcontrollers.
1. Smart Room - responsible for controlling the TV via RS-232, reading the weather indoors and outdoors from 5 sensors. Sends them to in a Google spreadsheet, sends a signal with an increased CO to telegrams and emits a sound is also the main web server.
2. Smart Bell is the main user of telegram, sends a message to a group, has many settings and interacts with the web interface.
3. Roller blinds - a separate board for the correct operation of the blinds, receiving commands from a smart room and a smart call.

# Plans

1. Add a graph of sensors to the web interface.
2. Add authentication to the web interface.
3. Add WIFI setting mode.
4. Add correction mode for roller blinds

# Web interface screenshot

<p>
  <img src="https://github.com/shinespeed/SmartHome/blob/master/index.png" width="250" title="hover text">
  <img src="https://github.com/shinespeed/SmartHome/blob/master/remote_tv.png" width="250" title="hover text">
  <img src="https://github.com/shinespeed/SmartHome/blob/master/roller_blinds.png" width="250" title="hover text">
</p>

# Telegram screenshot

<p>
  <img src="https://github.com/shinespeed/SmartHome/blob/master/weather.jpg" width="250" title="hover text">
  <img src="https://github.com/shinespeed/SmartHome/blob/master/call.jpg" width="250" title="hover text">
  <img src="https://github.com/shinespeed/SmartHome/blob/master/mute_bell.jpg" width="250" title="hover text">
</p>


