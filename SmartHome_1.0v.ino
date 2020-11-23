#include "stdaf.h"
#include "var.h"
#include "Sensor.h"

byte statePowerBalLamp  = 0;
byte statePowerHantarex = 1;
byte stateRollerBlinds  = 0;
byte statePowerSensor   = 1;
byte stateMuteHantarex  = 1;

const char* ssid     = "TITAN";
const char* password = "boot7127102";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

const int balLampPin     = 14;
const int sensorPowerPin = 23;
const int balButtonPin   = 35;
const int RXD1           = 13; //hantarex remote
const int TXD1           = 12; //hantarex remote
const int DHTPIN         = 27; //сенсор тепла и влажности балкон
const int SENSORMQ7      = 34; //сенсор угарного газа

int connectAttemptsWIFI = 0;
int numConnectServer    = 0;

long timerMuteButton = 0;

Sensor sensor;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients(String name, byte state) 
{
  ws.textAll(name+String(state));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char*)data, "switchBalLamp") == 0)
    {
      timerMuteButton = millis() + 2000;
      notifyClients("PowerBalLamp=", sensor.switchPowerBalLamp());
    } 
    else if(strcmp((char*)data, "switchHantarexPower") == 0)
    {
      notifyClients("PowerHantarex=", sensor.switchPowerHantarex());
    }
    else if(strcmp((char*)data, "switchPowerSensor") == 0)
    {
      notifyClients("PowerSensor=", sensor.switchPowerSensor());
    }
    else if(strcmp((char*)data, "switchMuteHantarex") == 0)
    {
      notifyClients("MuteHantarex=", sensor.switchMuteHantarex());
    }
    else if(strcmp((char*)data, "switchRollerBlinds") == 0)
    {
      notifyClients("RollerBlinds=", switchRollerBlinds());
    }
  } 
}

byte switchRollerBlinds()
{
  stateRollerBlinds = !stateRollerBlinds;
  return stateRollerBlinds;
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_CONNECT){numConnectServer++;}
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() 
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String stateSwitch(byte stateData)
{
  if (stateData)
  {
    return "checked"; 
  }
  else
  {
    return "";
  }
} 

String processor(const String& var)
{
  Serial.println(var);
  if(var == "STATE_BAL_LAMP")
  {
    return stateSwitch(statePowerBalLamp);
  } 
  else if(var == "STATE_HANTAREX_POWER")
  {
    return stateSwitch(statePowerHantarex);
  }
  else if(var == "STATE_ROLLER_BLINDS")
  {
    return stateSwitch(stateRollerBlinds);
  }
  else if(var == "STATE_SENSOR_POWER")
  {
    return stateSwitch(statePowerSensor);
  }
  else if(var == "STATE_HANTAREX_MUTE")
  {
    return stateSwitch(stateMuteHantarex);
  }
    else if(var == "TEMPERATURE_ROOM")
  {
    return BMERoomTemperature;
  }
  else if(var == "HUMIDITY_ROOM")
  {
    return BMERoomHumidity;
  }  
  else if(var == "PRESSURE")
  {
    return BMEPressure;
  }
  else if(var == "TEMPERATURE")
  {
    return DHTTemperature;
  }
  else if(var == "HUMIDITY")
  {
    return DHTHumidity;
  }  
  else if(var == "CO")
  {
    return COSensor;
  }
}

void notFound(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}

void initServer()
{
  initWebSocket();
    
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/remote_hantarex", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/remote_hantarex.html", String(), false, processor);
  });
  server.on("/roller_blinds", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/roller_blinds.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "application/javascript");
  });
  server.on("/script_hantarex.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script_hantarex.js", "application/javascript");
  });
  server.on("/roller_blinds.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/roller_blinds.js", "application/javascript");
  });
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.png", "image/png");
  });
  server.on("/remote-control.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/remote-control.png", "image/png");
  });
  server.on("/temperature_room", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", BMERoomTemperature.c_str());
  });
  server.on("/humidity_room", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", BMERoomHumidity.c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", BMEPressure.c_str());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", DHTTemperature.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", DHTHumidity.c_str());
  });
  server.on("/co", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", COSensor.c_str());
  });

  server.on("/enter_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
  Serial1.write(enter_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/up_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(up_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/down_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(down_menu, 5);
     request->send(200, "text/plain", "ok");
  });
  server.on("/left_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(left_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/right_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(right_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/info_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(info_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/ok_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(ok_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  server.on("/back_menu_hantarex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial1.write(back_menu, 5);
    request->send(200, "text/plain", "ok");
  });
  
  server.onNotFound(notFound);
  
  server.begin();
}

void setup()
{
  Serial.begin(19200);
  Serial1.begin(19200, SERIAL_8N1, RXD1, TXD1);

  pinMode(balLampPin,     OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(balButtonPin,   INPUT);
  
  digitalWrite(balLampPin,     LOW);
  digitalWrite(sensorPowerPin, LOW);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    connectAttemptsWIFI++;
    if (connectAttemptsWIFI > 3)
    {
      ESP.restart();
    }
  }

  Serial.println(WiFi.localIP());

  if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  initServer();

  delay(500);
  digitalWrite(sensorPowerPin, statePowerSensor);
    
  dht.begin();
  bme.begin(0x76);
}

void buttonBalLamp()
{
  if ((digitalRead(balButtonPin) == HIGH)&&(timerMuteButton < millis()))
  {
    delay(300);
    if (digitalRead(balLampPin) == LOW)
    {
      timerMuteButton = millis() + 2000;
      notifyClients("PowerBalLamp=", sensor.switchPowerBalLamp());
    } 
    else
    {
      timerMuteButton = millis() + 1000;
      notifyClients("PowerBalLamp=", sensor.switchPowerBalLamp());
    }
  }
}

void loop() 
{
  ws.cleanupClients();
  sensor.updateSensor();
  buttonBalLamp();
}
