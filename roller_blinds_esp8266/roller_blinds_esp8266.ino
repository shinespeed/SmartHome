#include <StaticThreadController.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <Thread.h> 

AsyncWebServer server(80);

Thread motorThread = Thread();

String sliderValue = "0";

int startStep = 0;
int endStep   = 7800;

int allStepCorr    = 0;
int stepCorrection = 240;

int eepromAdress = 4000;

int Step         = 16; //GPIO0---D0 Step of stepper motor driver
int Dir          = 4; //GPIO2---D2 Direction of stepper motor driver
int PowerMotor   = 14;

int nowStepMotor = 0;
int nowStep         = 0;
int futureStep      = 0;
int startDriveStep  = 0;
int correctionStepMode  = 0;

const int CORRECTION_STEP_VALUE = 100;
const char* ssid     = "TITAN";
const char* password = "boot7127102";
const char* PARAM_INPUT = "value";

void EEPROM_int_write(int addr, int num) 
{
	byte raw[2];
	(int&)raw = num;
	for(byte i = 0; i < 2; i++) EEPROM.write(addr+i, raw[i]);
	EEPROM.commit();
}

int EEPROM_int_read(int addr) 
{   
	byte raw[2];
	for(byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr+i);
	int &num = (int&)raw;
	
	return num;
}

void eepromWrite()
{ 
	EEPROM_int_write(eepromAdress, nowStep);
}

void eepromRead()
{
	nowStep = EEPROM_int_read(eepromAdress);
}

void setup() 
{
	Serial.begin(19200);
	EEPROM.begin(4096);
	if ((EEPROM_int_read(eepromAdress)<=endStep)&&(EEPROM_int_read(eepromAdress)>=startStep))
	{
		nowStep    = EEPROM_int_read(eepromAdress);
		futureStep = EEPROM_int_read(eepromAdress); 
	}
	pinMode(Step,       OUTPUT);
	pinMode(Dir,        OUTPUT);
	pinMode(PowerMotor, OUTPUT); 
	
	digitalWrite(Step,       LOW); 
	digitalWrite(Dir,        LOW);  
	digitalWrite(PowerMotor, LOW);  
 
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	Serial.println(WiFi.localIP());

	delay(5000);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	ArduinoOTA.setHostname("roller_blinds_esp8266");
	ArduinoOTA.onStart([]() 
    {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH) 
        {
		   type = "sketch";
        } 
        else 
        { 
		    type = "filesystem";
        }
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
       Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
    }
    });
    ArduinoOTA.begin();
    initServer();
    motorThread.onRun(changeStepMotor);
    motorThread.setInterval(1);
}

void initServer()
{
	server.on("/up_correction", HTTP_GET, [](AsyncWebServerRequest *request) {
		correctionStepMode =- CORRECTION_STEP_VALUE;
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/down_correction", HTTP_GET, [](AsyncWebServerRequest *request) {
		correctionStepMode =+ CORRECTION_STEP_VALUE;
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
		futureStep = startStep;
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
		futureStep = endStep;
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/status_step", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", statusStep().c_str());
	});
	server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
		String inputMessage;
		if (request->hasParam(PARAM_INPUT)) 
		{
			inputMessage = request->getParam(PARAM_INPUT)->value();
			sliderValue  = inputMessage;
			futureStep   = sliderValue.toInt()*((endStep-startStep)/100);
		}
		else 
		{
			inputMessage = "No message sent";
		}
		Serial.println(inputMessage);
		request->send(200, "text/plain", "OK");
	});
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin(); 
}

String statusStep()
{
	int data = (futureStep*100)/(endStep-startStep);
	return String(data);
}

void changeStepMotor()
{
	if (futureStep > nowStep)
	{
		int bufCounter = 0;
		digitalWrite(Dir, LOW);
		startDriveStep = nowStep;
		for (int i = 0; i < futureStep - startDriveStep; ++i)
		{
			digitalWrite(Step, HIGH);
			delay(1);
			nowStep++;
			allStepCorr++;  
			digitalWrite(Step, LOW);
			delay(1);   
		}
		eepromWrite();
	} 
	else if (futureStep < nowStep)
	{
		int bufCounter = 0;
		int correction = 0;
		if (allStepCorr >= stepCorrection)
		{
			correction = allStepCorr / stepCorrection;
			allStepCorr = allStepCorr - correction * stepCorrection;
		}
		else
		{
			correction = 0;
		}
		digitalWrite(Dir, HIGH);
		startDriveStep = nowStep;
		for (int i = 0; i < startDriveStep - futureStep; ++i)
		{
			digitalWrite(Step, HIGH);
			delay(1);
			nowStep--;  
			allStepCorr++;  
			digitalWrite(Step, LOW);
			delay(1);   
		}
		for (int i = 0; i < correction; ++i)
		{
			digitalWrite(Step, HIGH);
			delay(1);
			digitalWrite(Step, LOW);
			delay(1);   
		}
		eepromWrite();
	} 
}

void powerMotorSwitch()
{
	if ((nowStep != futureStep)&&(digitalRead(PowerMotor) == LOW)||(correctionStepMode!=0))
	{
		digitalWrite(PowerMotor, HIGH); 
		delay(1000);
	} 
	else if ((nowStep == futureStep)&&(digitalRead(PowerMotor) == HIGH)||
	  (correctionStepMode == 0)&&(digitalRead(PowerMotor) == HIGH))
	{
		digitalWrite(PowerMotor, LOW); 
	}
}

void correctionStepModeFunc() 
{
	if (correctionStepMode != 0)
	{
		if (correctionStepMode < 0)
		{
			digitalWrite(Dir, HIGH);
			for (int i = 0; i < CORRECTION_STEP_VALUE; ++i)
			{
				correctionStepMode++;
				digitalWrite(Step, HIGH);
				delay(1);
				digitalWrite(Step, LOW);
				delay(1);
			}
		}
		else if (correctionStepMode > 0)
		{
			digitalWrite(Dir, LOW);
			for (int i = 0; i < CORRECTION_STEP_VALUE; ++i)
			{
				correctionStepMode--;
				digitalWrite(Step, HIGH);
				delay(1);
				digitalWrite(Step, LOW);
				delay(1);
			}
		}
	}
}

void loop() 
{
	ArduinoOTA.handle();
	powerMotorSwitch();
	correctionStepModeFunc();
	changeStepMotor();
	Serial.println(nowStep);
}
