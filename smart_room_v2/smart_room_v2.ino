#include "stdif.h"
#include "SmartRoom.h"

SmartRoom smart_room;

void OTA_init()
{
	ArduinoOTA.setHostname("smart_room_esp32");
	ArduinoOTA.onStart([]() 
	{
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else
			type = "filesystem";
		Serial.println("Start updating " + type);
	}).onEnd([]()
	{
		Serial.println("\nEnd");
	}).onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	}).onError([](ota_error_t error) 
	{
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	ArduinoOTA.begin();
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.begin(SSID_WIFI, PASSWORD_WIFI);
}

void WIFI_init()
{
    WiFi.disconnect(true);

    delay(1000);

    WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.begin(SSID_WIFI, PASSWORD_WIFI);
    
    Serial.println("Wait for WiFi... ");
}

void setup()
{
	Serial.begin(19200);
	Serial1.begin(19200, SERIAL_8N1, RX_RS232, TX_RS232);

	pinMode(BAL_LAMP_PIN,     OUTPUT);
	pinMode(SENSOR_POWER_PIN, OUTPUT);
	pinMode(SPEAKER,          OUTPUT);
	pinMode(BAL_BUTTON_PIN,   INPUT);

	digitalWrite(BAL_LAMP_PIN, LOW);
	digitalWrite(SPEAKER,      LOW);

	WIFI_init();
	OTA_init();

	if (!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	smart_room.init();

    delay(3000);
  
    digitalWrite(SENSOR_POWER_PIN, HIGH);
}


void loop()
{
	ArduinoOTA.handle();
	smart_room.update();
}
