#include "Telegram.h"


Telegram::Telegram(Controller* web_server, Controller* sensor, Controller* spiffs_data):
	_web_server(web_server),
	_sensor(sensor),
	_spiffs_data(spiffs_data),
	_bot(BOT_TOKEN, _client),
	_bot_last_time(0),
	_bot_mtbs(500)
{}

Telegram::~Telegram()
{}

String Telegram::ip_address_to_string(const IPAddress& ip_address)
{
	return String(ip_address[0]) + String(".") + \
		String(ip_address[1]) + String(".") + \
		String(ip_address[2]) + String(".") + \
		String(ip_address[3]);
}

void Telegram::write_message_telegram(String chat_id, String text) 
{
	_bot.sendMessage(chat_id, text, "");
}

void Telegram::command_telegram(const String& text, const String& chat_id, const String& from_name) 
{
	if (text == "/on_lamp")
	{
		set_state_power_lamp(true);
	}
	if (text == "/off_lamp")
	{
		set_state_power_lamp(false);
	}
	if (text == "/down_roller")
	{
		set_state_roller_blinds(true);
	}
	if (text == "/up_roller")
	{
		set_state_roller_blinds(false);
	}
	if (text == "/start")
	{
		_bot.sendMessage(chat_id, "Hello "+ from_name+"! Welcome to "+ String(VERSION), "");
	}
	if (text == "/weather")
	{
		String str = "STREET: \n";
		str += "\n";
		str += "\U0001F321 Temperature: " + _indication_sensor.DHT_temperature + "C\n";
		str += "\U0001F4A7 Humidity: "+ _indication_sensor.DHT_humidity + " %\n";
		str += "\U0001F32B Pressure: "+ _indication_sensor.BME_pressure + " hPa\n";
		str += "\n";
		str += "SMALL ROOM:\n";
		str += "\n";
		str += "\U0001F321 Temperature: "+ _indication_sensor.BME_room_temperature + " C\n";
		str += "\U0001F4A7 Humidity: "+ _indication_sensor.BME_room_humidity + " %\n";
		str += "\U0001F525 Level CO: " + _indication_sensor.CO_level+" ppr\n";

		_bot.sendMessage(chat_id, str, "");
	}
	if (text == "/status")
	{
		String str = String(VERSION) + " \n";
		str += "Local IP: " + ip_address_to_string(WiFi.localIP()) + " \n";
		str += "Fire status: " + String(_status_output._fire) + " \n";
		str += "Speaker status: " + String(_status_output._speaker) + " \n";
		str += "Bell status: " + String(_status_output._bell) + " \n";
		str += "Power Hantarex: " + String(_status_output._power_hantarex) + " \n";
		str += "Power sensor: " + String(_status_output._power_sensor) + " \n";
		str += "Roller blinds: " + String(_status_output._roller_blinds) + " \n";
		str += "Power lamp: " + String(_status_output._power_lamp) + " \n";
		str += "Mute Hantarex: " + String(_status_output._mute_hantarex) + " \n";

		_bot.sendMessage(chat_id, str, "");
	}
}

void Telegram::handle_new_messages(int num_new_messages) 
{
	for (int i = 0; i < num_new_messages; ++i)
	{
		String from_name = _bot.messages[i].from_name;

		if (from_name == "") from_name = "Guest";
   
		command_telegram(_bot.messages[i].text, _bot.messages[i].chat_id, from_name);
	}
}

void Telegram::message_telegram(const String& text) 
{
	_bot.sendMessage(CHAT_ID_TELEGRAM, text, "");
}

void Telegram::start_message() 
{
	_bot.sendMessage(CHAT_ID_TELEGRAM, String(VERSION)+" Started", "");
}

void Telegram::update() 
{
	if ((millis() > _bot_last_time + _bot_mtbs)&&(!_status_output._fire))
	{
    long time_buf = millis();
		int num_new_messages = _bot.getUpdates(_bot.last_message_received + 1);

		while (num_new_messages)
		{
			Serial.println("got response");
			handle_new_messages(num_new_messages);
			num_new_messages = _bot.getUpdates(_bot.last_message_received + 1);
		}
		_bot_last_time = millis();
    Serial.print("time telegram: ");
    Serial.println(time_buf - _bot_last_time);
	}
}
