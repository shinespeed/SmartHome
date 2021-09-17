#include "SPIFFS_DATA.h"


SPIFFS_DATA::SPIFFS_DATA()
{}

String SPIFFS_DATA::read_value_setting(const String& name_par)
{
    int begin_pos;
    int end_pos;
    if ((int)setting_SPIFFS.indexOf(name_par) != -1)
    {
        begin_pos = setting_SPIFFS.indexOf(name_par) + name_par.length() + 2;
        end_pos   = setting_SPIFFS.indexOf("};", begin_pos);
        return setting_SPIFFS.substring(begin_pos, end_pos);
    }
    else
    {
        return "null";
    }  
}

String SPIFFS_DATA::dir_view() 
{
	File root = SPIFFS.open("/");
	File file = root.openNextFile();
	String files = "";
	while (file)
	{
		files += String(file.name()) + " " + String(file.size()) + "B\n";
		file = root.openNextFile();
	}
	return files;
}

String SPIFFS_DATA::format_spiffs() 
{
	setting_SPIFFS.clear();
	bool res = SPIFFS.format();
	if (!res)
		return "Format unsuccessful";
	else
		return "SPIFFS formatted.";
}

void SPIFFS_DATA::read_SPIFFS()
{
    File file = SPIFFS.open("/setting.txt", FILE_READ);
    setting_SPIFFS = file.readString();
	file.close();

	file = SPIFFS.open("/verificate_list_telegram.txt", FILE_READ);
	verificate_telegram_list = file.readString();
    file.close();  
}

void SPIFFS_DATA::add_verificate_telegram(const String& chat_id)
{
	if (cheak_verificate_telegram(chat_id))
	{
		return;
	}
	else
	{
		verificate_telegram_list += chat_id + "\n";
	}

	File file = SPIFFS.open("/verificate_list_telegram.txt", FILE_WRITE);
	if (file.print(verificate_telegram_list))
	{
		Serial.println("list telegram update");
	}
	file.close();
}

bool SPIFFS_DATA::cheak_verificate_telegram(const String& chat_id)
{
	if ((int)verificate_telegram_list.indexOf(chat_id) != -1)
	{
		return true;
	}
	else return false;
}

void SPIFFS_DATA::write_setting_SPIFFS(const String& name_data, const String& value)
{
    change_string(setting_SPIFFS, name_data, value);
    Serial.println(setting_SPIFFS);
    File file = SPIFFS.open("/setting.txt", FILE_WRITE);
    if (file.print(setting_SPIFFS))
    {
        Serial.println("Message successfully written");
    }
    file.close();  
}

void SPIFFS_DATA::change_string(String& setting, const String& name_data, const String& value)
{
    if ((int)setting.indexOf(name_data) != -1)
    {
        String buf_str;
        short begin_pos;
        short end_pos;

        begin_pos = setting.indexOf(name_data) + name_data.length() + 1;
        end_pos   = setting.indexOf("};", begin_pos) + 1;
        buf_str   = setting.substring(0, begin_pos);
        buf_str  += "{" + value + "}";
        buf_str  += setting.substring(end_pos, setting.length());
        setting   = buf_str;
    }
    else
    {
        setting += name_data + "={" + value + "};\n";
    }  
}
