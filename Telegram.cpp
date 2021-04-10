
#include "Telegram.h"


Telegram::Telegram(Controller* web_server, Controller* sensor, Controller* spiffs_data):
	_web_server(web_server),
	_sensor(sensor),
	_spiffs_data(spiffs_data),
	_bot(BOT_TOKEN, _client)
{
}

Telegram::~Telegram()
{}
