#pragma once

#ifndef _SHEETS_WEATHER
#define _SHEETS_WEATHER 

#include "Controller.h"

class SheetsWeather: Controller
{
	const char* server = "maker.ifttt.com";
	const char* resource = "xxxxxxxxxxxxxxxx";

	const uint64_t delay_update = 1800000;
	uint64_t update_time = 0;

public:
	SheetsWeather();
	~SheetsWeather();

	void write_data();
	void update();
};

#endif

