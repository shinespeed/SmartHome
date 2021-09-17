#include "Controller.h"


Controller::Controller()
{}

Controller::~Controller()
{}

byte Controller::switch_power_lamp()
{
	_status_output._power_lamp = !_status_output._power_lamp;
	digitalWrite(BAL_LAMP_PIN, _status_output._power_lamp);
	return _status_output._power_lamp;
}

byte Controller::switch_power_hantarex()
{
	_status_output._power_hantarex = !_status_output._power_hantarex;
	if (_status_output._power_hantarex)
	{
		Serial1.write(ON_MENU, 6);
		_status_output._mute_hantarex = 1;
	}
	else
	{
		Serial1.write(OFF_MENU, 6);
	}

	return _status_output._power_hantarex;
}

byte Controller::switch_mute_hantarex()
{
	_status_output._mute_hantarex = !_status_output._mute_hantarex;
	return _status_output._mute_hantarex;
}

byte Controller::switch_power_sensor() 
{
	_status_output._power_sensor = !_status_output._power_sensor;
	digitalWrite(SENSOR_POWER_PIN, _status_output._power_sensor);
	return _status_output._power_sensor;
}

byte Controller::switch_roller_blinds()
{
	_status_output._roller_blinds = !_status_output._roller_blinds;
	return _status_output._roller_blinds;
}

byte Controller::switch_power_speaker() 
{
	_status_output._speaker = !_status_output._speaker;
	return _status_output._speaker;
}

byte Controller::switch_bell_mode()
{
	_status_output._bell = !_status_output._bell;
	return _status_output._bell;
}

void Controller::set_state_roller_blinds(byte state) 
{
	HTTPClient http;
	if (state == true) 
	{
		http.begin("http://192.168.3.130/down");
	}
	else 
	{
		http.begin("http://192.168.3.130/up");
	}
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		_status_output._roller_blinds = http.getString().toInt();
		Serial.println(_status_output._roller_blinds);
	}
	else {
		Serial.println("ERROR HTTP");
	}
	http.end();
}

void Controller::set_state_power_lamp(byte state) 
{
	_status_output._power_lamp = state;
	digitalWrite(BAL_LAMP_PIN, state);
}

void Controller::message_telegram(const String& text) {}

void Controller::web_notify_bell_status() {}
