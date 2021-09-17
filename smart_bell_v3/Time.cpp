#include "Time.h"

Time::Time(): time_client(ntpUDP),
	          begin_mute_timer_str("null"),
	          end_mute_timer_str("null"),
	          time_zone_sec(10800),
              SEC_IN_DAY(86400),
	          mute_timer_start(0),
              mute_timer_state(0)
{}

Time::~Time()
{}

unsigned long Time::time_string_to_sec(const String& time_string, long time_correction)
{
	int index = (int)time_string.indexOf(":");
	unsigned long time_sec;

	time_sec  = time_string.substring(0, index).toInt() * 3600;
	time_sec += time_string.substring(index + 1, time_string.length()).toInt() * 60;

	return time_sec + time_correction;
}

unsigned long Time::time_count_sec(const String& time_string)
{
	unsigned long time_sec_day = time_string_to_sec(time_client.getFormattedTime().substring(0, 5), time_client.getFormattedTime().substring(6, 8).toInt()) + time_zone_sec;
	unsigned long time_string_in_sec = time_string_to_sec(time_string);

	if (time_string_in_sec > time_sec_day)
	{
		return (time_client.getEpochTime() - time_sec_day) + time_string_in_sec;
	}
	else if (time_string_in_sec < time_sec_day)
	{
		return (time_client.getEpochTime() - time_sec_day) + time_string_in_sec + SEC_IN_DAY;
	}
}

void Time::time_mute_count_sec(const String& begin_time_string, const String& end_time_string)
{
	begin_mute_timer_sec = time_count_sec(begin_time_string);

    unsigned long time_sec_day_begin = time_string_to_sec(begin_time_string);
	unsigned long time_sec_day_end   = time_string_to_sec(end_time_string);

	if (time_sec_day_end > time_sec_day_begin)
	{
		end_mute_timer_sec = (begin_mute_timer_sec - time_sec_day_begin) + time_sec_day_end;
	}
	else if (time_sec_day_end < time_sec_day_begin)
	{
		end_mute_timer_sec = (begin_mute_timer_sec - time_sec_day_begin) + time_sec_day_end + SEC_IN_DAY;
	}
	_spiffs_data->write_setting_SPIFFS("begin_mute_timer_sec", (String)begin_mute_timer_sec);
	_spiffs_data->write_setting_SPIFFS("end_mute_timer_sec",   (String)end_mute_timer_sec);
}

bool Time::cheak_valid_date(const String& date)
{
	if (((int)date.indexOf(":") != -1) && (date.length() > 3))
	{
		int index = (int)date.indexOf(":");

		if (date.substring(0, index).toInt() >= 0
			&& (date.substring(0, index)).toInt() < 24
			&& (date.substring(index + 1, date.length())).toInt() >= 0
			&& (date.substring(index + 1, date.length())).toInt() < 60)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else return false;
}

void Time::mute_timer()
{
	if ((time_client.getEpochTime() >= begin_mute_timer_sec) && (!mute_timer_start))
	{
		mute_timer_begin();
		_telegram->write_message_telegram(CHAT_ID_CONST, "Timer mute work... BELL OFF");
	}
	else if ((time_client.getEpochTime() >= end_mute_timer_sec) && (mute_timer_start))
	{
		mute_timer_end();
		_telegram->write_message_telegram(CHAT_ID_CONST, "Timer mute dont work... BELL ON");
	}
}

void Time::mute_timer_begin()
{
	switch_bell_state(false);
	mute_timer_start = true;
	_spiffs_data->write_setting_SPIFFS("mute_timer_start", (String)mute_timer_start);
}

void Time::mute_timer_end()
{
	switch_bell_state(true);
	mute_timer_start = false;
	time_mute_count_sec(begin_mute_timer_str, end_mute_timer_str);
	_spiffs_data->write_setting_SPIFFS("mute_timer_start", (String)mute_timer_start);
}

void Time::on_timer_mute() 
{
    mute_timer_state = true;
	time_mute_count_sec(begin_mute_timer_str, end_mute_timer_str);
	_spiffs_data->write_setting_SPIFFS("mute_timer_state", (String)mute_timer_state);
}

void Time::off_timer_mute()
{
	mute_timer_state = false;
	mute_timer_end();
	_spiffs_data->write_setting_SPIFFS("mute_timer_state", (String)mute_timer_state);
}

void Time::comp_time_mute() 
{
	time_mute_count_sec(begin_mute_timer_str, end_mute_timer_str);
	_spiffs_data->write_setting_SPIFFS("begin_mute_timer_str", (String)begin_mute_timer_str);
	_spiffs_data->write_setting_SPIFFS("end_mute_timer_str", (String)end_mute_timer_str);
}

void Time::load_data_SPIFFS() 
{
	Serial.println("load time DATA");
	if (_spiffs_data->read_value_setting("begin_mute_timer_str") != "null")  begin_mute_timer_str = _spiffs_data->read_value_setting("begin_mute_timer_str");
	if (_spiffs_data->read_value_setting("end_mute_timer_str")   != "null")  end_mute_timer_str   = _spiffs_data->read_value_setting("end_mute_timer_str");
	if (_spiffs_data->read_value_setting("begin_mute_timer_sec") != "null")  begin_mute_timer_sec = _spiffs_data->read_value_setting("begin_mute_timer_sec").toInt();
	if (_spiffs_data->read_value_setting("end_mute_timer_sec")   != "null")  end_mute_timer_sec   = _spiffs_data->read_value_setting("end_mute_timer_sec").toInt();
	if (_spiffs_data->read_value_setting("mute_timer_state")     != "null")  mute_timer_state     = _spiffs_data->read_value_setting("mute_timer_state").toInt();
	if (_spiffs_data->read_value_setting("mute_timer_start")     != "null")  mute_timer_start     = _spiffs_data->read_value_setting("mute_timer_start").toInt();
}

void Time::update_time() 
{
	if (time_client.update() && mute_timer_state)
	{
		mute_timer();
	}
}

void Time::set_point_telegram(Controller* telegram) 
{
	this->_telegram = telegram;
}
