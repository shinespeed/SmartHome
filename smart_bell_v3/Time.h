#pragma once

#include "stdif.h"
#include "Controller.h"

class Time: public Controller
{
public:
	Time();
	~Time();

	WiFiUDP ntpUDP;
	NTPClient time_client;

	String formatted_date;
	String begin_mute_timer_str;
	String end_mute_timer_str;

	Controller* _telegram;

	const unsigned long SEC_IN_DAY;
	unsigned long       begin_mute_timer_sec;
	unsigned long       end_mute_timer_sec;
	unsigned long       time_zone_sec;
	int                 day_in_year;
	bool                mute_timer_start;
	bool                mute_timer_state;

	unsigned long time_string_to_sec(const String& time_string, long time_correction = 0);
	unsigned long time_count_sec(const String& time_string);

	bool cheak_valid_date(const String& date);
	void time_mute_count_sec(const String& begin_time_string, const String& end_time_string);
	void mute_timer();
	void mute_timer_begin();
	void mute_timer_end();
	void on_timer_mute();
	void off_timer_mute();
	void comp_time_mute();
	void load_data_SPIFFS();
	void update_time();
	void set_point_telegram(Controller* telegram);
};

