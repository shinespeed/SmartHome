#pragma once

#include "Controller.h"
#include "stdif.h"
#include "Telegram.h"

class WebServer: public Controller
{
public:
	WebServer(Controller* telegram);

	AsyncWebServer server;
    Controller* _telegram;

	bool change_mode_bell;

	void init_server();
	void send_message(const String& text);
	void update_message_telegram();
};
