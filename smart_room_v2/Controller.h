#pragma once

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "stdif.h"
#include <HTTPClient.h>

struct StatusOutput
{
	byte _fire;
	byte _speaker;
	byte _bell;
	byte _power_hantarex;
	byte _power_sensor;
	byte _roller_blinds;
	byte _power_lamp;
	byte _mute_hantarex;
};

struct IndicationSensor
{
	String BME_room_humidity;
	String BME_room_temperature;
	String BME_pressure;
	String DHT_humidity;
	String DHT_temperature;
	String CO_level;
};

class Controller
{
protected:
	static StatusOutput     _status_output;
	static IndicationSensor _indication_sensor;

public:
	Controller();
	~Controller();

	byte switch_power_lamp();
	byte switch_power_hantarex();
	byte switch_mute_hantarex();
	byte switch_power_sensor();
	byte switch_roller_blinds();
	byte switch_power_speaker();
	byte switch_bell_mode();

	void set_state_roller_blinds(byte state);
	void set_state_power_lamp(byte state);

	virtual void message_telegram(const String& text);
	virtual void web_notify_bell_status();
};

#endif

