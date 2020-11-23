#pragma once

#include "stdaf.h"

#define DHTTYPE DHT22
#define SEALEVELPRESSURE_HPA (1013.25)

extern byte statusFire;
extern byte statusSpeaker;

extern const byte mute_menu[5];
extern const byte info_menu[5];
extern const byte ok_menu[5];
extern const byte enter_menu[5];
extern const byte up_menu[5];
extern const byte down_menu[5];
extern const byte left_menu[5];
extern const byte right_menu[5];
extern const byte back_menu[5];
extern const byte off_power[6];
extern const byte on_power[6];

extern byte statePowerHantarex;
extern byte statePowerSensor;
extern byte stateRollerBlinds;
extern byte statePowerBalLamp;
extern byte stateMuteHantarex;

extern const char* ssid;
extern const char* password;

extern const int numErrorRead;

extern const int balLampPin;
extern const int balButtonPin;
extern const int sensorPowerPin;
extern const int RXD1;
extern const int TXD1;
extern const int DHTPIN; //сенсор тепла и влажности балкон
extern const int SENSORMQ7; //сен

extern int ppm;

extern long timerMuteButton;

extern String BMERoomHumidity;
extern String BMERoomTemperature;
extern String BMEPressure;
extern String DHTHumidity;
extern String DHTTemperature;
extern String COSensor;

extern DHT dht;
extern Adafruit_BME280 bme;
