#include "SmartRoom.h"


SmartRoom::SmartRoom(): _spiffs_data(new SPIFFS_DATA()),
	                    _telegram(new Telegram(_web_server, _sensor, _spiffs_data)),
                        _web_server(new WebServer(_telegram, _sensor, _spiffs_data)),
	                    _sensor(new Sensor(_telegram, _web_server, _spiffs_data))
{}

SmartRoom::~SmartRoom()
{
	if (_spiffs_data != nullptr) delete _spiffs_data;
	if (_telegram    != nullptr) delete _telegram;
	if (_web_server  != nullptr) delete _web_server;
	if (_sensor      != nullptr) delete _sensor;
}

void SmartRoom::init() 
{
	_web_server->init();
	_sensor->init();
}

void SmartRoom::update()
{
	_web_server->update();
	_sensor->update();
}
