#pragma once

#include "stdif.h"
#include "Controller.h"

class Telegraph
{
public:
	Telegraph();

	long _last_time;
	int  _buf_time_telegraph;
	int  _timer_telegraph;
	int  _time_dahs;
	int  _time_dits;
	bool _telegraph_state;

	void telegraph_dahs();
	void telegraph_dits();
	void telegraph_code_bin(const String& code_bin);
	void telegraph_bin_ASCII(const char& symbol_ASCII);
};

class Call: public Controller
{
public:
	Call();

	long _last_time;
	long _timer_call;

	void load_spiffs();
	void get_call();
    String set_time_call(String time_millis);
};
