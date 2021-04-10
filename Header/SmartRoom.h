#pragma once

#ifndef _SMART_ROOM_H
#define _SMART_ROOM_H

#include "stdif.h"
#include "Telegram.h"
#include "SPIFFS_DATA.h"
#include "WebServer.h"
#include "Sensor.h"

class SmartRoom
{
private:
	long time_mute_button;

	Telegram*    _telegram;
	SPIFFS_DATA* _spiffs_data;
	WebServer*   _web_server;
	Sensor*      _sensor;

public:
	SmartRoom();
	~SmartRoom();

	void init();
	void update();
};

#endif

