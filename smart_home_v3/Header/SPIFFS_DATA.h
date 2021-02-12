#pragma once

#include "stdif.h"

class SPIFFS_DATA
{
public:
    SPIFFS_DATA();

    String setting_SPIFFS;
  
    String read_value_setting(const String& name_par);
    String dir_view();
    String format_spiffs();

    void read_setting_SPIFFS();
    void write_setting_SPIFFS(const String& name_data, const String& value);
    void change_string(String& setting, const String& name_data, const String& value);
};
