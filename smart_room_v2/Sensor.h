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

	float _R0;

	long _timer_speaker_high;
	long _timer_speaker_low;
	long _timer_read_BME_room;
	long _timer_read_DHT;
	long _timer_read_CO_sensor;

	long _buf_timer_read_BME_room_humidity;
	long _buf_timer_read_BME_room_temperature;
	long _buf_timer_read_BME_pressure;
	long _buf_timer_read_DHT_humidity;
	long _buf_timer_read_DHT_temperature;
	long _buf_timer_read_CO_sensor;
	long _ping_signal_fire;

	long _timer_message_telegram;

	int _num_read_error_humidity;
	int _num_read_error_temperature;
	int _num_error_read;
	int _ppm;


	Controller* _telegram;
	Controller* _web_server;
	Controller* _spiffs_data;

	byte _status_speaker;
	byte _change_status_speaker;
	byte _status_signal_fire;
	byte _pause_signal_state;
	byte _resolution_active_signal;

public:
	Sensor(Controller* telegram, Controller* web_server, Controller* spiffs_data);
	~Sensor();

	void cheak_level_co();

	void read_BME_room_humidity();
	void read_BME_room_temperature();
	void read_BME_pressure();
	void read_MQ_room_CO();
	void read_DHT_humidity();
	void read_DHT_temperature();

	void signal_fire();
	void message_fire();
	void speaker();

	void init();
	void update();
};

#endif

