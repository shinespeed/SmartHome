#include "Controller.h"

bool status_bell = true;

Controller::Controller()
{}

void Controller::switch_bell_state(bool state)
{
    status_bell = state;
    digitalWrite(RELAY_PIN_L, !state);
    Serial.println("Bell status:"+ state);
    _spiffs_data->write_setting_SPIFFS("status_bell", (String)status_bell);
}

void Controller::switch_call_relay(bool state)
{
    status_call = state;
    digitalWrite(RELAY_PIN_R, state);
    //_spiffs_data.write_setting_SPIFFS("status_call", (String)status_bell);  
}

void Controller::load_data_SPIFFS()
{
    _spiffs_data->read_SPIFFS();
    if (_spiffs_data->read_value_setting("status_bell")!= "null")
    {
        bool buf_state = _spiffs_data->read_value_setting("status_bell").toInt(); 
        digitalWrite(RELAY_PIN_L, !buf_state);
        status_bell = buf_state;
    }
}

void Controller::set_spiffs_data(SPIFFS_DATA* spiffs_data)
{
	this->_spiffs_data = spiffs_data;
}

void Controller::write_message_telegram(String chat_id, String text){}

void Controller::test(){};
