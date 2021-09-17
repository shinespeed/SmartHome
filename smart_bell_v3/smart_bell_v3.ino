#include "stdif.h"
#include "Bell.h" 

Bell bell;

void OTAInit()
{
  ArduinoOTA.setHostname("smart_bell_esp32");
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

    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }  
	 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}


void setup() 
{
	Serial.begin(115200);
    wifi_init();
  
    if (!SPIFFS.begin(true)) Serial.println("An Error has occurred while mounting SPIFFS"); 
  
    pinMode(RELAY_PIN_L, OUTPUT); 
    pinMode(RELAY_PIN_R, OUTPUT);
    pinMode(BUTTON_PIN,  INPUT);

    digitalWrite(RELAY_PIN_L, LOW); 
    digitalWrite(RELAY_PIN_R, LOW);

    OTAInit();

    bell.start();
}

void loop()
{
    ArduinoOTA.handle();
    bell.update_bell();
}
