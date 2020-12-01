#pragma once

#include "var.h"
#include "stdaf.h"

class Sensor 
{
public:  
  long timerReadBMERoom; 
  long timerReadDHT;
  long timerReadCOSensor;

  long bufTimerReadBMERoomHumidity;
  long bufTimerReadBMERoomTemperature;  
  long bufTimerReadBMEPressure; 
  long bufTimerReadDHTHumidity;
  long bufTimerReadDHTTemperature;
  long bufTimerReadCOSensor;

  String bufDHTHumidity;
  String bufDHTTemperature;
  String bufCOSensor;

  int numReadErrorHumidity;
  int numReadErrorTemperature;

  int numErrorRead; 
  
  Sensor();

  byte switchPowerBalLamp();
  byte switchPowerHantarex();
  byte switchPowerSensor();
  byte switchMuteHantarex();

  void readBMERoomHumidity();
  void readBMERoomTemperature();
  void readBMEPressure(); 
  void readDHTHumidity();
  void readDHTTemperature();
  void readCOSensor();

  void updateSensor();
};
