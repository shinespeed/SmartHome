#include "Bell.h"


Bell::Bell(): _web_server(&_telegram),
              _telegram(&_web_server)
{}

void Bell::update_bell()
{
     button_event();
    _telegram.update();
   	_time.update_time();
	_web_server.update_message_telegram();
}

void Bell::button_event()
{
    if ((digitalRead(BUTTON_PIN) == HIGH)&&(status_call == 0))
    {
        _telegram.button_message();
    } 
}

void Bell::init_link_object() 
{
	_time.set_spiffs_data(&_spiffs_data);
	_time.set_point_telegram(&_telegram);

	_web_server.set_spiffs_data(&_spiffs_data);

	_telegram.set_spiffs_data(&_spiffs_data);
	_telegram.set_pointer_time(&_time);

	_telegram._call.set_spiffs_data(&_spiffs_data);
}

void Bell::start()
{
	init_link_object();
	
	_telegram.load_data_SPIFFS();
	_time.load_data_SPIFFS();
    _telegram.start_message();
	_telegram._call.load_spiffs();
	_web_server.init_server();
}
