#pragma once

#ifndef _TELEGRAM_MODULE_H
#define _TELEGRAM_MODULE_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "Controller.h"

class Telegram: public Controller
{
private:
	UniversalTelegramBot _bot;
	WiFiClientSecure     _client;

	Controller* _web_server;
	Controller* _sensor;
	Controller* _spiffs_data;

	String       _send_text;
	long         _bot_last_time;
	short        _bot_mtbs;

public:
	Telegram(Controller* web_server, Controller* sensor, Controller* spiffs_data);
	~Telegram();

	String status_message(const String& chat_id);
	String ip_address_to_string(const IPAddress& ip_address);

	void write_message_telegram(String chat_id, String text);
	void command_telegram(const String& text, const String& chat_id, const String& from_name);
	void handle_new_messages(int num_new_messages);
	void message_telegram(const String& text);
	void start_message();
	void update();
};

#endif
