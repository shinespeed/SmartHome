#include "Sensor.h"

Sensor::Sensor(Controller* telegram, Controller* web_server, Controller* spiffs_data):
	_telegram(telegram),
	_web_server(web_server),
	_spiffs_data(spiffs_data),
	_dht(DHT_PIN, DHTTYPE),
	_timer_speaker_high(0),
	_timer_speaker_low(0),
	_timer_read_BME_room(1000),
	_timer_read_DHT(15000),
	_timer_read_CO_sensor(500),
	_timer_message_telegram(0),
	_buf_DHT_humidity("0.0"),
	_buf_DHT_temperature("0.0"),
	_buf_CO_sensor("0.0"),
	_buf_timer_read_BME_room_humidity(0),
	_buf_timer_read_BME_room_temperature(0),
	_buf_timer_read_BME_pressure(0),
	_buf_timer_read_DHT_humidity(0),
	_buf_timer_read_DHT_temperature(0),
	_buf_timer_read_CO_sensor(0),
	_num_read_error_humidity(0),
	_num_read_error_temperature(0),
	_num_error_read(5),
	_status_speaker(false),
    _change_status_speaker(true),
    _R0(7200.0),
	_ppm(0),
	_ping_signal_fire(0),
	_status_signal_fire(false),
	_pause_signal_state(true),
	_resolution_active_signal(false)
{
	_indication_sensor.BME_room_humidity    = "NaN";
	_indication_sensor.BME_room_temperature = "NaN";
	_indication_sensor.BME_pressure         = "NaN";
	_indication_sensor.DHT_humidity         = "NaN";
	_indication_sensor.DHT_temperature      = "NaN";
	_indication_sensor.CO_level             = "NaN";
}

Sensor::~Sensor()
{
}

void Sensor::cheak_level_co()
{
	if (_ppm >= TRIGGER_CO_LEVEL)
	{
		_status_output._fire = true;
	}
	else
	{
		_status_output._fire = false;
	}
}

void Sensor::read_BME_room_humidity()
{
	if (_buf_timer_read_BME_room_humidity < millis())
	{
		String bufData = String(_bme.readHumidity());

		_indication_sensor.BME_room_humidity = bufData.substring(0, bufData.length() - 1);
		Serial.println("Room Humidity: " + _indication_sensor.BME_room_humidity);
		_buf_timer_read_BME_room_humidity = millis() + _timer_read_BME_room;
	}
}

void Sensor::read_BME_room_temperature()
{
	if (_buf_timer_read_BME_room_temperature < millis())
	{
		String bufData = String(_bme.readTemperature());

		_indication_sensor.BME_room_temperature = bufData.substring(0, bufData.length() - 1);
		Serial.println("Room Temperature: " + _indication_sensor.BME_room_temperature);
		_buf_timer_read_BME_room_temperature = millis() + _timer_read_BME_room;
	}
}

void Sensor::read_BME_pressure() 
{
	if (_buf_timer_read_BME_pressure < millis())
	{
		String bufData = String(_bme.readPressure() / 100.0F);

		_indication_sensor.BME_pressure = bufData.substring(0, bufData.length() - 1);
		Serial.println("Pressure: " + _indication_sensor.BME_pressure);
		_buf_timer_read_BME_pressure = millis() + _timer_read_BME_room;
	}
}

void Sensor::read_MQ_room_CO() 
{
	if (_buf_timer_read_CO_sensor < millis())
	{
		float _rs_gas = 0;
		float _ratio = 0;
		float _sensor_value = 0;
		float _sensor_volt = 0;

		_sensor_value = analogRead(SENSOR_MQ7);
		_sensor_volt = _sensor_value / 4096 * 5.0;
		_rs_gas = (5.0 - _sensor_volt) / _sensor_volt;
		_ratio = _rs_gas / _R0; //Replace R0 with the value found using the sketch above

		float x = 1538.46 * _ratio;

		_ppm = pow(x, -1.709) * 10;
		_buf_CO_sensor = String(_ppm);
		_buf_timer_read_CO_sensor = millis() + _timer_read_CO_sensor;

		cheak_level_co();
		Serial.println(_ppm);
	}
	_indication_sensor.CO_level = String(_ppm);
}

void Sensor::read_DHT_humidity() 
{
	if (_buf_timer_read_DHT_humidity < millis())
	{
		float h = _dht.readHumidity();

		_buf_timer_read_DHT_humidity = millis() + _timer_read_DHT;

		if (isnan(h))
		{
			Serial.println("Failed to read from DHT sensor!");
			_num_read_error_humidity++;

			if (_num_read_error_humidity < _num_error_read)
			{
				_indication_sensor.DHT_humidity = _buf_DHT_humidity;
			}
			else
			{
				_indication_sensor.DHT_humidity = "0.0";
			}
		}
		else
		{
			Serial.println("Street Humidity: " + _buf_DHT_humidity + "%");
			_buf_DHT_humidity = String(h).substring(0, String(h).length() - 1);
			_num_read_error_humidity = 0;
			_indication_sensor.DHT_humidity = _buf_DHT_humidity;
		}
	}
}

void Sensor::read_DHT_temperature() 
{
	if (_buf_timer_read_DHT_temperature < millis())
	{
		float t = _dht.readTemperature();

		_buf_timer_read_DHT_temperature = millis() + _timer_read_DHT;

		if (isnan(t))
		{
			Serial.println("Failed to read from DHT sensor!");
			_num_read_error_temperature++;

			if (_num_read_error_temperature < _num_error_read)
			{
				_indication_sensor.DHT_temperature = _buf_DHT_temperature;
			}
			else
			{
				_indication_sensor.DHT_temperature = "0.0";
			}
		}
		else
		{
			Serial.println("Street Temperature: " + _buf_DHT_temperature + "C");
			_buf_DHT_temperature = String(t).substring(0, String(t).length() - 1);
			_num_read_error_temperature = 0;
			_indication_sensor.DHT_temperature = _buf_DHT_temperature;
		}
	}
}
void Sensor::signal_fire() 
{
	if ((_status_output._fire)&&(_pause_signal_state))
	{
		_pause_signal_state = false;
		_ping_signal_fire = millis();
	}
	else if ((_status_output._fire) && ((millis() - _ping_signal_fire) >= PING_SIGNAL) 
		&& (_resolution_active_signal == false))
	{
		_resolution_active_signal = true;
	}
	else if ((!_status_output._fire) && (_status_signal_fire)) 
	{
		_resolution_active_signal = false;
	}

	if (_resolution_active_signal)
	{
		if (_status_output._speaker)
		{
			speaker();
			message_fire();
		}
		else 
		{
			digitalWrite(SPEAKER, LOW);
			message_fire();
		}
		_status_signal_fire = true;
	}
	else if (_status_signal_fire)
	{
		_pause_signal_state = true;
		_status_signal_fire = false;
		digitalWrite(SPEAKER, LOW);
	}
}

void Sensor::message_fire()
{
	if (millis() > _timer_message_telegram)
	{
		_telegram->message_telegram("Attemption!!! HOUSE IS BURNING!!!");
		_timer_message_telegram = millis() + MESSAGE_FIRE_DELAY;
	}
}

void Sensor::speaker()
{
	if (millis() < _timer_speaker_high)
	{
		if ((_status_speaker)&&(_change_status_speaker))
		{
			digitalWrite(SPEAKER, HIGH);
			_change_status_speaker = false;
		}
		else if (_change_status_speaker)
		{
			digitalWrite(SPEAKER, LOW);
			_change_status_speaker = false;
		}
		Serial.println("Speaker: " + String(_status_speaker));
	}
	else
	{
		Serial.println("Speaker: TEST");
		_status_speaker = !_status_speaker;
		_change_status_speaker = true;
		_timer_speaker_high = millis() + SPEAKER_TIME_SONG;
	}
}

void Sensor::init() 
{
	_dht.begin();
	_bme.begin(0x76);
}

void Sensor::update() 
{
	signal_fire();

	read_BME_room_humidity();
	read_BME_room_temperature();
	read_BME_pressure();
	read_MQ_room_CO();
	read_DHT_humidity();
	read_DHT_temperature();
}
