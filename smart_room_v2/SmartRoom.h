#pragma once

#ifndef _SMART_ROOM_H
#define _SMART_ROOM_H

#include "stdif.h"
#include "Telegram.h"
#include "SPIFFS_DATA.h"
#include "WebServer.h"
#include "Sensor.h"
#include "SheetsWeather.h"

class SmartRoom
{
private:
	long _time_mute_button;

	Telegram*      _telegram;
	SPIFFS_DATA*   _spiffs_data;
	WebServer*     _web_server;
	Sensor*        _sensor;
	SheetsWeather* _sheets_weather;

public:
	SmartRoom();
	~SmartRoom();

	void button_power_lamp();
	void init();
	void update();
};

#endif

