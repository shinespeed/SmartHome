#include "SPIFFS_DATA.h"

StatusOutput Controller::_status_output = {0};
IndicationSensor Controller::_indication_sensor = {};

SPIFFS_DATA::SPIFFS_DATA()
{
	_status_output._power_hantarex = 1;
	_status_output._power_sensor   = 1;
    _status_output._fire           = 0;
	_status_output._speaker        = 1;
	_status_output._bell           = 1;
	_status_output._roller_blinds  = 0;
	_status_output._power_lamp     = 0;
	_status_output._mute_hantarex  = 0;
}

SPIFFS_DATA::~SPIFFS_DATA()
{
}
