#pragma once

#ifndef _TELEGRAM_MODULE_H
#define _TELEGRAM_MODULE_H

#include "Controller.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

class Telegram: public Controller
{
private:
	UniversalTelegramBot _bot;
	WiFiClientSecure     _client;

	Controller* _web_server;
	Controller* _sensor;
	Controller* _spiffs_data;

public:
	Telegram(Controller* web_server, Controller* sensor, Controller* spiffs_data);
	~Telegram();
};

#endif
