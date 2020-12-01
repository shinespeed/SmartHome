#include "Sensor.h"

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BME280 bme;

String BMERoomHumidity    = "0";
String BMERoomTemperature = "0";
String BMEPressure        = "0";
String DHTHumidity        = "0";
String DHTTemperature     = "0";
String COSensor           = "0";

float RS_gas      = 0;
float ratio       = 0;
float sensorValue = 0;
float sensor_volt = 0;
float R0          = 7200.0;

int ppm         = 0;

const byte mute_menu[5]     = {0xF4, 0x88, 0x00, 0xFD, 0x09};
const byte info_menu[5]     = {0xF4, 0x88, 0x00, 0xFD, 0x51};
const byte ok_menu[5]       = {0xF4, 0x88, 0x00, 0xFD, 0x0F};
const byte enter_menu[5]    = {0x45, 0x88, 0x00, 0xFD, 0x0F};
const byte up_menu[5]       = {0xF4, 0x88, 0x00, 0xFD, 0x14};
const byte down_menu[5]     = {0xF4, 0x88, 0x00, 0xFD, 0x15};
const byte left_menu[5]     = {0xF4, 0x88, 0x00, 0xFD, 0x0B};
const byte right_menu[5]    = {0xF4, 0x88, 0x00, 0xFD, 0x0A};
const byte back_menu[5]     = {0xF4, 0x88, 0x00, 0xFD, 0x10};
const byte off_power[6]     = {0xF5, 0x88, 0x00, 0xFE, 0xFE, 0x00};
const byte on_power[6]      = {0xF5, 0x88, 0x00, 0xFE, 0xFE, 0x01};

Sensor::Sensor():
                timerReadBMERoom(1000), 
                timerReadDHT(15000), 
                timerReadCOSensor(500),
                bufDHTHumidity("0.0"),
                bufDHTTemperature("0.0"),
                bufCOSensor("0.0"),
                bufTimerReadBMERoomHumidity(0),
                bufTimerReadBMERoomTemperature(0),  
                bufTimerReadBMEPressure(0), 
                bufTimerReadDHTHumidity(0),
                bufTimerReadDHTTemperature(0),
                bufTimerReadCOSensor(0),
                numReadErrorHumidity(0),
                numReadErrorTemperature(0),
                numErrorRead(5)
{}

byte Sensor::switchPowerBalLamp()
{
  statePowerBalLamp = !statePowerBalLamp;
  digitalWrite(balLampPin, statePowerBalLamp);
  
  return statePowerBalLamp;
}

byte Sensor::switchPowerHantarex()
{
  statePowerHantarex = !statePowerHantarex;
  if(statePowerHantarex)
  {
    Serial1.write(on_power, 6);
    stateMuteHantarex = 1;
  }
  else
  {
    Serial1.write(off_power, 6);
  }
  
  return statePowerHantarex;
}

byte Sensor::switchPowerSensor()
{
  statePowerSensor = !statePowerSensor;
  digitalWrite(sensorPowerPin, statePowerSensor);

  return statePowerSensor;
}

byte Sensor::switchMuteHantarex()
{
  stateMuteHantarex = !stateMuteHantarex;
  Serial1.write(mute_menu, 5);
  
  return stateMuteHantarex;
}

void Sensor::readBMERoomHumidity()
{
  if (bufTimerReadBMERoomHumidity < millis())
  {
    String bufData = String(bme.readHumidity());
    
    BMERoomHumidity = bufData.substring(0, bufData.length()-1);
    Serial.println("Room Humidity: "+ BMERoomHumidity);
    bufTimerReadBMERoomHumidity = millis() + timerReadBMERoom;
  }
}

void Sensor::readBMERoomTemperature()
{
  if (bufTimerReadBMERoomTemperature < millis())
  {
    String bufData = String(bme.readTemperature());
    
    BMERoomTemperature = bufData.substring(0, bufData.length()-1);
    Serial.println("Room Temperature: "+ BMERoomTemperature);
    bufTimerReadBMERoomTemperature = millis() + timerReadBMERoom;
  } 
}

void Sensor::readBMEPressure()
{
  if (bufTimerReadBMEPressure < millis())
  {
    String bufData = String(bme.readPressure() / 100.0F);
    
    BMEPressure = bufData.substring(0, bufData.length()-1);
    Serial.println("Pressure: "+ BMEPressure);
    bufTimerReadBMEPressure = millis() + timerReadBMERoom;
  } 
}

void Sensor::readDHTHumidity()
{
  if (bufTimerReadDHTHumidity < millis())
  {
    float h = dht.readHumidity();

    bufTimerReadDHTHumidity = millis() + timerReadDHT;
    
    if (isnan(h)) 
    {    
      Serial.println("Failed to read from DHT sensor!");
      numReadErrorHumidity++;
    
      if (numReadErrorHumidity < numErrorRead)
      {
        DHTHumidity = bufDHTHumidity;
      }
      else
      {
        DHTHumidity = "0.0";
      }
    }
    else 
    {
      Serial.println("Street Humidity: "+ bufDHTHumidity +"%");
      bufDHTHumidity = String(h).substring(0, String(h).length()-1);
      numReadErrorHumidity = 0;
      DHTHumidity =  bufDHTHumidity;
    }
  }
}

void Sensor::readDHTTemperature()
{
  if (bufTimerReadDHTTemperature < millis())
  {
    float t = dht.readTemperature();
    
    bufTimerReadDHTTemperature = millis() + timerReadDHT;
  
    if (isnan(t)) 
    {    
      Serial.println("Failed to read from DHT sensor!");
      numReadErrorTemperature++;
    
      if (numReadErrorTemperature < numErrorRead)
      {
        DHTTemperature = bufDHTTemperature;
      }
      else
      {
        DHTTemperature = "0.0";
      }
    }
    else 
    {
      Serial.println("Street Temperature: "+ bufDHTTemperature +"C");
      bufDHTTemperature = String(t).substring(0, String(t).length()-1);
      numReadErrorTemperature = 0;
      DHTTemperature = bufDHTTemperature;
    }
  }
}

void Sensor::readCOSensor()
{
  if(bufTimerReadCOSensor < millis())
  {
    sensorValue = analogRead(SENSORMQ7);
    sensor_volt = sensorValue/4096*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt;
    ratio = RS_gas/R0; //Replace R0 with the value found using the sketch above
    float x = 1538.46 * ratio;
    ppm = pow(x,-1.709)*10;
    bufCOSensor = ppm;
    
    bufTimerReadCOSensor = millis() + timerReadCOSensor; 
    Serial.println(ppm);
  }
  COSensor = ppm;
}

void Sensor::updateSensor()
{
  readBMERoomHumidity();
  readBMERoomTemperature();
  readBMEPressure();
  readDHTHumidity();
  readDHTTemperature();
  readCOSensor();
}
