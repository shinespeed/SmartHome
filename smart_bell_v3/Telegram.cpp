#include "Telegram.h"


Telegram::Telegram(Controller* web):  _web(web),
                                      _bot(BOTtoken, _client),
                                      _CHAT_ID(CHAT_ID_CONST),
                                      _bot_last_time(0),   
                                      _bot_mtbs(20)
{}

void Telegram::update()
{
    if (millis() > _bot_last_time + _bot_mtbs)
    {
        long buf_time = millis();
        int num_new_messages = _bot.getUpdates(_bot.last_message_received + 1);

        while (num_new_messages)
        {
            Serial.println("got response");
            handle_new_messages(num_new_messages);
            num_new_messages = _bot.getUpdates(_bot.last_message_received + 1);
        }
        _bot_last_time = millis();
        Serial.print("Time telegram: ");
        Serial.println(millis() - buf_time);
    }
}

void Telegram::write_message_telegram(String chat_id, String text)
{
	_bot.sendMessage(chat_id, text, "");
}

void Telegram::command_telegram(const String& text, const String& chat_id, const String& from_name)
{
	if (verificate_id(chat_id))
	{
		allowed_commands(text, chat_id, from_name);
	} 
	else if (text == "/start") 
	{
		_bot.sendMessage(chat_id, "Hello " + from_name + "! Welcome Smart Bellv3, Access blocked, enter password: ", "");
		if (cheak_password(read_message())) 
		{
			add_verificate_id(chat_id);
			view_keyboard("/start", chat_id, "Access is allowed!");
		}
		else 
		{
			_bot.sendMessage(chat_id, "Invalid password, please try again", "");
		}
	}
}

void Telegram::allowed_commands(const String& text, const String& chat_id, const String& from_name)
{
	if (text == "/start")
	{
		view_keyboard(text, chat_id, "Hello " + from_name + "! Welcome Smart Bellv3, Nice to see you again!");
	}
	if (text == "/on_bell")
	{
		view_keyboard(text, chat_id, "BELL ON", OFF_BELL);
		switch_bell_state(true);
	}
	if (text == "/off_bell")
	{
		view_keyboard(text, chat_id, "BELL OFF", ON_BELL);
		switch_bell_state(false);
	}
	if (text == "/setting")
	{
		view_keyboard(text, chat_id, "Setting");
	}
	if (text == "/mute_timer")
	{
		String time_str_menu;
		time_str_menu = "status mute: " + String(_time->mute_timer_state) + "\n"
			+ "begin: " + _time->begin_mute_timer_str + "\n"
			+ "end: " + _time->end_mute_timer_str;
		view_keyboard(text, chat_id, time_str_menu);
	}
	if (text == "/status")
	{
		view_keyboard(text, chat_id, status_message(_CHAT_ID));
	}
	///////////////////////////////////////////////////////
	if (text == "/smart_room")
	{
		view_keyboard(text, chat_id, "Smart Room");
	}
	///////////////////////////////////////////////////////
	if (text == "/calling")
	{
		view_keyboard(text, chat_id, "Calling");
	}
	if (text == "/call")
	{
		call_bell(from_name, chat_id);
	}
	if (text == "/telegraph")
	{
		view_keyboard(text, chat_id, "Telegraph start, limit 25 symbol latin, for end session /stop_telegraph");
		String temp = read_message();
		if (temp == "/back")
		{
			view_keyboard(temp, chat_id);
		}
		else
		{
			telegraph_send_text(temp, chat_id, from_name);
		}
	}
	///////////////////////////////////////////////////////
	if (text == "/dir")
	{
		_bot.sendMessage(chat_id, _spiffs_data->dir_view(), "");
	}
	if (text == "/format")
	{
		_bot.sendMessage(chat_id, _spiffs_data->format_spiffs(), "");
	}
	if (text == "/write_spiffs")
	{
		view_keyboard(text, chat_id, "Send file doc in format .txt");
	}
	if (text == "/update_firmware")
	{
		view_keyboard(text, chat_id, "Send file doc in format .bin");
	}
	if (text == "/update_spiffs")
	{
		view_keyboard(text, chat_id, "Send file doc in format .bin");
	}
	if (text == "/setting_call")
	{
		view_keyboard(text, chat_id, "Set time to bell from 100ms to 3000ms");
		_bot.sendMessage(chat_id, _call.set_time_call(read_message()), "");
	}
	//////////////////////Mute time//////////////////////
	if (text == "/on_timer_mute")
	{
		_time->on_timer_mute();
		view_keyboard(text, chat_id, "Dir view", "/off_timer_mute");
	}
	if (text == "/off_timer_mute")
	{
		_time->off_timer_mute();
		view_keyboard(text, chat_id, "Dir view", "/on_timer_mute");
	}
	if (text == "/begin_time_mute")
	{
		_bot.sendMessage(chat_id, "Enter begin time mute in format XX:XX", "");
		if (mute_bell(chat_id, _time->begin_mute_timer_str))
		{
			_time->comp_time_mute();
			_bot.sendMessage(chat_id, "You setting begin mute time applied", "");
		}
	}
	if (text == "/end_time_mute")
	{
		_bot.sendMessage(chat_id, "Enter end time mute in format XX:XX", "");
		if (mute_bell(chat_id, _time->end_mute_timer_str))
		{
			_time->comp_time_mute();
			_bot.sendMessage(chat_id, "You setting end mute time applied", "");
		}
	}
	///////////////////////////////////////////////////////
	if (text == "/back")
	{
		view_keyboard(text, chat_id);
	}
}

void Telegram::handle_new_messages(int num_new_messages)
{  
    for (int i = 0; i < num_new_messages; ++i)
    {
        String from_name = _bot.messages[i].from_name;

		if (from_name == "") from_name = "Guest";

		if (_bot.messages[i].hasDocument == true)
		{
			update_write_data_ESP(_bot.messages[i].chat_id, i, num_new_messages);
		}
		else 
		{
			command_telegram(_bot.messages[i].text, _bot.messages[i].chat_id, from_name);
		}
    } 
}

void Telegram::update_write_data_ESP(String chat_id, int i, int& numNewMessages) 
{
	httpUpdate.rebootOnUpdate(false);
	t_httpUpdate_return ret = (t_httpUpdate_return)3;
	if (_bot.messages[i].file_caption == "/write_spiffs")
	{
		size_t spiffsFreeSize = SPIFFS.totalBytes() - SPIFFS.usedBytes();
		if (_bot.messages[i].file_size < spiffsFreeSize)
		{
			_bot.sendMessage(_bot.messages[i].chat_id, "File downloading.", "");
			HTTPClient http;
			if (http.begin(_client, _bot.messages[i].file_path))
			{
				int code = http.GET();
				if (code == HTTP_CODE_OK)
				{
					int total = http.getSize();
					int len = total;
					uint8_t buff[128] = { 0 };
					WiFiClient* tcp = http.getStreamPtr();
					if (SPIFFS.exists("/" + _bot.messages[i].file_name))
						SPIFFS.remove(("/" + _bot.messages[i].file_name));
					File fl = SPIFFS.open("/" + _bot.messages[i].file_name, FILE_WRITE);
					if (!fl)
					{
						_bot.sendMessage(_bot.messages[i].chat_id, "File open error.", "");
					}
					else
					{
						while (http.connected() && (len > 0 || len == -1))
						{
							size_t size_available = tcp->available();
							Serial.print("%");
							Serial.println(100 - ((len * 100) / total));
							if (size_available)
							{
								int c = tcp->readBytes(buff, ((size_available > sizeof(buff)) ? sizeof(buff) : size_available));
								fl.write(buff, c);
								if (len > 0)
								{
									len -= c;
								}
							}
							delay(1);
						}
						fl.close();
						if (len == 0)
							_bot.sendMessage(_bot.messages[i].chat_id, "Success.", "");
						else
							_bot.sendMessage(_bot.messages[i].chat_id, "Error.", "");
					}
				}
				http.end();
			}
		}
		else
		{
			_bot.sendMessage(_bot.messages[i].chat_id, "SPIFFS size to low (" + String(spiffsFreeSize) + ") needed: " + String(_bot.messages[i].file_size), "");
		}
	}
	else
	{
		if (_bot.messages[i].file_caption == "/update_firmware")
		{
			_bot.sendMessage(_bot.messages[i].chat_id, "Firmware writing...", "");
			ret = httpUpdate.update(_client, _bot.messages[i].file_path);
		}
		if (_bot.messages[i].file_caption == "/update_spiffs")
		{
			_bot.sendMessage(_bot.messages[i].chat_id, "SPIFFS writing...", "");
			ret = httpUpdate.updateSpiffs(_client, _bot.messages[i].file_path);
		}
		switch (ret)
		{
		case HTTP_UPDATE_FAILED:
			_bot.sendMessage(_bot.messages[i].chat_id, "HTTP_UPDATE_FAILED Error (" + String(httpUpdate.getLastError()) + "): " + httpUpdate.getLastErrorString(), "");
			break;

		case HTTP_UPDATE_NO_UPDATES:
			_bot.sendMessage(_bot.messages[i].chat_id, "HTTP_UPDATE_NO_UPDATES", "");
			break;

		case HTTP_UPDATE_OK:
			_bot.sendMessage(_bot.messages[i].chat_id, "UPDATE OK.\nRestarting...", "");
			numNewMessages = _bot.getUpdates(_bot.last_message_received + 1);
			ESP.restart();
			break;
		default:
			break;
		}
	}
}

void Telegram::call_bell(const String& from_name, const String& chat_id)
{
    _bot.sendMessage(chat_id, from_name + " is calling in bell...", "");
    if(_CHAT_ID != chat_id)
    {
        _bot.sendMessage(_CHAT_ID, from_name + " is calling in bell...", "");
    }
    _call.get_call();
}

void Telegram::start_message()
{
    _bot.sendMessage(_CHAT_ID, "Smart Bellv3 Started", "");
}

void Telegram::button_message()
{
    _bot.sendMessage(_CHAT_ID, "ZVONOK!!! dzzzzzz", "");
} 

void Telegram::view_keyboard(const String& command, const String& chat_id, const String& chat_text, const String& change_command)
{
    _bot.sendMessageWithReplyKeyboard(chat_id, chat_text, "", _keyboard.view_json_keyboard(command, change_command), true);
}

void Telegram::telegraph_send_text(const String& text, const String& chat_id, const String& from_name)
{
	_telegraph._buf_time_telegraph = millis() + _telegraph._buf_time_telegraph;
	_telegraph._telegraph_state = 1;
	if (text.length() <= 25)
	{
		digitalWrite(RELAY_PIN_L, HIGH);
		for (int i = 0; i < text.length(); ++i)
		{
			if (_telegraph._telegraph_state == 0) break;
			_telegraph.telegraph_bin_ASCII(text[i]);
		}
		digitalWrite(RELAY_PIN_L, !status_bell);
	}
	else
	{
		_bot.sendMessage(chat_id, from_name + " this is too long text! (>25)", "");
	}
	_telegraph._telegraph_state = 0;
}

void Telegram::set_pointer_time(Time* time) 
{
	this->_time = time;
}

bool Telegram::mute_bell(const String& chat_id, String& mute_str)
{
	String date;
	date =  read_message();

	if (_time->cheak_valid_date(date))
	{
		String buf;
		int index = (int)date.indexOf(":");
		if (date.substring(0, index).length() == 1)
		{
			buf = "0" + date.substring(0, index) + ":";
		}
		else
		{
			buf = date.substring(0, index) + ":";
		}

		if (date.substring(index + 1, date.length()).length() == 1)
		{
			buf = buf + "0" + date.substring(index + 1, date.length());
		}
		else
		{
			buf = buf + date.substring(index + 1, date.length());
		}
		mute_str = buf;
		return true;
	}
	else
	{
		_bot.sendMessage(chat_id, "ERROR TYPE", "");
		return false;
	}
}

bool Telegram::cheak_password(const String& password)
{
	return password == PASSWORD_TELEGRAM ? true : false;
}

bool Telegram::verificate_id(const String& chat_id)
{
	return _spiffs_data->cheak_verificate_telegram(chat_id);
}

void Telegram::add_verificate_id(const String& chat_id)
{
	_spiffs_data->add_verificate_telegram(chat_id);
}

String Telegram::ip_address_to_string(const IPAddress& ip_address)
{
	return String(ip_address[0]) + String(".") + \
	       String(ip_address[1]) + String(".") + \
		   String(ip_address[2]) + String(".") + \
	       String(ip_address[3]);
}

String Telegram::status_message(const String& chat_id)
{
	String status_stirng;
	status_stirng = status_stirng + "SSID: " + SSID_WIFI + "\n";
	status_stirng = status_stirng + "Local IP: " + ip_address_to_string(WiFi.localIP()) + "\n";
	if (status_bell)
	{
		status_stirng = status_stirng + "Bell ON\n";
	}
	else
	{
		status_stirng = status_stirng + "Bell OFF\n";
	}
	status_stirng = status_stirng + "bell status: " + status_bell + "\n";
	status_stirng = status_stirng + "begin sec: "+ _time->begin_mute_timer_sec + "\n";
	status_stirng = status_stirng + "end sec: "+ _time->end_mute_timer_sec + "\n";
	status_stirng = status_stirng + "begin str: "+ _time->begin_mute_timer_str + "\n";
	status_stirng = status_stirng + "end str: "+ _time->end_mute_timer_str + "\n";
	status_stirng = status_stirng + "mute_timer_start: " + _spiffs_data->read_value_setting("mute_timer_start")+"\n";
	status_stirng = status_stirng + "mute_timer_state: " + _time->_spiffs_data->read_value_setting("mute_timer_state")+"\n";
	status_stirng = status_stirng + "now time: " + _time->time_client.getEpochTime() + "\n";
	status_stirng = status_stirng + "time str: " + _time->time_client.getFormattedTime() + "\n";
	status_stirng = status_stirng + "time call millis: " + _call._timer_call;
	return status_stirng;
}

String Telegram::read_message() 
{
	int index = _bot.getUpdates(_bot.last_message_received + 1);
    
	while (index == 0)
	{
		index = _bot.getUpdates(_bot.last_message_received + 1);
	}
	return _bot.messages[0].text;
}

void Telegram::test()
{
  Serial.println("df");
}
