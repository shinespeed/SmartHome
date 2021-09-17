#include "Calling.h"

bool status_call = 0;

Telegraph::Telegraph(): _telegraph_state(false),
                        _time_dahs(500),
                        _time_dits(200)
{}

void Telegraph::telegraph_dahs()
{
	_last_time = millis();
	while (millis() <= (_last_time + _time_dahs))
	{
		digitalWrite(RELAY_PIN_R, HIGH);
	}
	digitalWrite(RELAY_PIN_R, LOW);
}

void Telegraph::telegraph_dits()
{
	_last_time = millis();
	while (millis() <= (_last_time + _time_dits))
	{
		digitalWrite(RELAY_PIN_R, HIGH);
	}
	digitalWrite(RELAY_PIN_R, LOW);
}

void Telegraph::telegraph_code_bin(const String& code_bin)
{
	for (int i = 0; i < code_bin.length(); ++i)
	{
		if (_telegraph_state == 0) break;

		if (code_bin[i] == '0')
		{
			telegraph_dits();
		}
		else if (code_bin[i] == '1')
		{
			telegraph_dahs();
		}
		delay(150);
	}
}

void Telegraph::telegraph_bin_ASCII(const char& symbol_ASCII)
{
	switch (symbol_ASCII)
	{
	case 'A': telegraph_code_bin("01");
		break;
	case 'B': telegraph_code_bin("1000");
		break;
	case 'C': telegraph_code_bin("1010");
		break;
	case 'D': telegraph_code_bin("100");
		break;
	case 'E': telegraph_code_bin("0");
		break;
	case 'F': telegraph_code_bin("0010");
		break;
	case 'G': telegraph_code_bin("110");
		break;
	case 'H': telegraph_code_bin("0000");
		break;
	case 'I': telegraph_code_bin("00");
		break;
	case 'J': telegraph_code_bin("0111");
		break;
	case 'K': telegraph_code_bin("101");
		break;
	case 'L': telegraph_code_bin("0100");
		break;
	case 'M': telegraph_code_bin("11");
		break;
	case 'N': telegraph_code_bin("10");
		break;
	case 'O': telegraph_code_bin("111");
		break;
	case 'P': telegraph_code_bin("0110");
		break;
	case 'Q': telegraph_code_bin("1101");
		break;
	case 'R': telegraph_code_bin("010");
		break;
	case 'S': telegraph_code_bin("000");
		break;
	case 'T': telegraph_code_bin("1");
		break;
	case 'U': telegraph_code_bin("001");
		break;
	case 'V': telegraph_code_bin("0001");
		break;
	case 'W': telegraph_code_bin("011");
		break;
	case 'X': telegraph_code_bin("1001");
		break;
	case 'Y': telegraph_code_bin("1011");
		break;
	case 'Z': telegraph_code_bin("1100");
		break;
	case 'a': telegraph_code_bin("01");
		break;
	case 'b': telegraph_code_bin("1000");
		break;
	case 'c': telegraph_code_bin("1010");
		break;
	case 'd': telegraph_code_bin("100");
		break;
	case 'e': telegraph_code_bin("0");
		break;
	case 'f': telegraph_code_bin("0010");
		break;
	case 'g': telegraph_code_bin("110");
		break;
	case 'h': telegraph_code_bin("0000");
		break;
	case 'i': telegraph_code_bin("00");
		break;
	case 'j': telegraph_code_bin("0111");
		break;
	case 'k': telegraph_code_bin("101");
		break;
	case 'l': telegraph_code_bin("0100");
		break;
	case 'm': telegraph_code_bin("11");
		break;
	case 'n': telegraph_code_bin("10");
		break;
	case 'o': telegraph_code_bin("111");
		break;
	case 'p': telegraph_code_bin("0110");
		break;
	case 'q': telegraph_code_bin("1101");
		break;
	case 'r': telegraph_code_bin("010");
		break;
	case 's': telegraph_code_bin("000");
		break;
	case 't': telegraph_code_bin("1");
		break;
	case 'u': telegraph_code_bin("001");
		break;
	case 'v': telegraph_code_bin("0001");
		break;
	case 'w': telegraph_code_bin("011");
		break;
	case 'x': telegraph_code_bin("1001");
		break;
	case 'y': telegraph_code_bin("1011");
		break;
	case 'z': telegraph_code_bin("1100");
		break;
	case '0': telegraph_code_bin("11111");
		break;
	case '1': telegraph_code_bin("01111");
		break;
	case '2': telegraph_code_bin("00111");
		break;
	case '3': telegraph_code_bin("00011");
		break;
	case '4': telegraph_code_bin("00001");
		break;
	case '5': telegraph_code_bin("00000");
		break;
	case '6': telegraph_code_bin("10000");
		break;
	case '7': telegraph_code_bin("11000");
		break;
	case '8': telegraph_code_bin("11100");
		break;
	case '9': telegraph_code_bin("11110");
		break;
	case '.': telegraph_code_bin("0");
		break;
	case '-': telegraph_code_bin("1");
		break;
	}
}

Call::Call() : _timer_call(300)
{}

void Call::load_spiffs() 
{
	if (_spiffs_data->read_value_setting("timer_call") != "null") _timer_call = _spiffs_data->read_value_setting("timer_call").toInt();
}

void Call::get_call()
{
	digitalWrite(RELAY_PIN_L, HIGH);
	status_call = 1;
	_last_time = millis();
	while (millis() <= (_last_time + _timer_call))
	{
		digitalWrite(RELAY_PIN_R, HIGH);
	}
	digitalWrite(RELAY_PIN_R, LOW);
	digitalWrite(RELAY_PIN_L, LOW);
	status_call = 0;
}

String Call::set_time_call(String time_millis) 
{
	int bufferTime = time_millis.toInt();
	if ((100 <= bufferTime) && (3000 >= bufferTime))
	{
		_timer_call = bufferTime;
		_spiffs_data->write_setting_SPIFFS("timer_call", (String)_timer_call);
		return "You setting call applied";
	}
	else
	{
		return "ERROR VALUE";
	}
}
