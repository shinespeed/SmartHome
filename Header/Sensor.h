#pragma once

#ifndef _SENSOR_H
#define _SENSOR_H 

#define DHTTYPE DHT22
#define SEALEVELPRESSURE_HPA (1013.25)

#include "Controller.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class Sensor: public Controller
{
private:
	DHT _dht;
	Adafruit_BME280 _bme;

	String _buf_DHT_humidity;
	String _buf_DHT_temperature;
	String _buf_CO_sensor;

	float _rs_gas = 0;
	float _ratio = 0;
	float _sensor_value = 0;
	float _sensor_volt = 0;
	float _R0 = 7200.0;

	long _timer_read_BME_room;
	long _timer_read_DHT;
	long _timer_read_CO_sensor;

	long _buf_timer_read_BME_room_humidity;
	long _buf_timer_read_BME_room_temperature;
	long _buf_timer_read_BME_pressure;
	long _buf_timer_read_DHT_humidity;
	long _buf_timer_read_DHT_temperature;
	long _buf_timer_read_CO_sensor;

	int _num_read_error_humidity;
	int _num_read_error_temperature;
	int _num_error_read;
	int _ppm;

	Controller* _telegram;
	Controller* _web_server;
	Controller* _spiffs_data;

public:
	Sensor(Controller* telegram, Controller* web_server, Controller* spiffs_data);
	~Sensor();

	void button_power_lamp();

	void read_BME_room_humidity();
	void read_BME_room_temperature();
	void read_BME_pressure();
	void read_MQ_room_CO();
	void read_DHT_humidity();
	void read_DHT_temperature();

	void init();
	void update();
};

#endif

