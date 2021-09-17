#include "SmartRoom.h"


SmartRoom::SmartRoom(): _spiffs_data(new SPIFFS_DATA()),
	                    _telegram(new Telegram(_web_server, _sensor, _spiffs_data)),
                        _web_server(new WebServer(_telegram, _sensor, _spiffs_data)),
	                    _sensor(new Sensor(_telegram, _web_server, _spiffs_data)),
	                    _sheets_weather(new SheetsWeather())
{}

SmartRoom::~SmartRoom()
{
	if (_spiffs_data != nullptr) delete _spiffs_data;
	if (_telegram    != nullptr) delete _telegram;
	if (_web_server  != nullptr) delete _web_server;
	if (_sensor      != nullptr) delete _sensor;
}

void SmartRoom::button_power_lamp()
{
	if ((digitalRead(BAL_BUTTON_PIN) == HIGH) && (_time_mute_button < millis()))
	{
		if (digitalRead(BAL_BUTTON_PIN) == LOW)
		{
			_time_mute_button = millis() + 2000;
			_spiffs_data->switch_power_lamp();
		}
		else
		{
			_time_mute_button = millis() + 1000;
			_spiffs_data->switch_power_lamp();
		}
	}
}


void SmartRoom::init() 
{
	_web_server->init();
	_sensor->init();
	_telegram->start_message();
}

void SmartRoom::update()
{
	button_power_lamp();
	_telegram->update();
	_web_server->update();
	_sensor->update();
	_sheets_weather->update();
}
