#pragma once

#include "stdif.h"

class SPIFFS_DATA
{
public:
	String setting_SPIFFS;
	String verificate_telegram_list;

    SPIFFS_DATA();
  
    String read_value_setting(const String& name_par);
	String dir_view();
	String format_spiffs();

    void read_SPIFFS();

	void add_verificate_telegram(const String& chat_id);
	bool cheak_verificate_telegram(const String& chat_id);


    void write_setting_SPIFFS(const String& name_data, const String& value);
    void change_string(String& setting, const String& name_data, const String& value);
};
