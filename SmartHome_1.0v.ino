#include "stdif.h"
#include "SmartRoom.h"


SmartRoom smart_room;

void ota_init()
{
	ArduinoOTA.setHostname("smart_room_esp32");
	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else
			type = "filesystem";
		Serial.println("Start updating " + type);
	})
		.onEnd([]() {
		Serial.println("\nEnd");
	})
		.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	})
		.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	ArduinoOTA.begin();
}

void wifi_init()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID_WIFI, PASSWORD_WIFI);

	int connectAttemptsWIFI = 0;

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println("Connecting to WiFi..");
		connectAttemptsWIFI++;
		if (connectAttemptsWIFI > 6)
		{
			ESP.restart();
		}
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void setup()
{
	Serial.begin(19200);
	Serial1.begin(19200, SERIAL_8N1, RX_RS232, TX_RS232);

	pinMode(BAL_LAMP_PIN,     OUTPUT);
	pinMode(SENSOR_POWER_PIN, OUTPUT);
	pinMode(BAL_BUTTON_PIN,   INPUT);

	digitalWrite(BAL_LAMP_PIN,     LOW);
	digitalWrite(SENSOR_POWER_PIN, HIGH);

	wifi_init();
	ota_init();

	if (!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	smart_room.init();
}

void loop()
{
	ArduinoOTA.handle();
	smart_room.update();
}
