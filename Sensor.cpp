#include "Sensor.h"

Sensor::Sensor(Controller* telegram, Controller* web_server, Controller* spiffs_data):
	_telegram(telegram),
	_web_server(web_server),
	_spiffs_data(spiffs_data),
	_dht(DHT_PIN, DHTTYPE),
	_timer_read_BME_room(1000),
	_timer_read_DHT(15000),
	_timer_read_CO_sensor(500),
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
	_rs_gas(0),
        _ratio(0),
        _sensor_value(0),
        _sensor_volt(0),
        _R0(7200.0),
	_ppm(0)
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

void Sensor::button_power_lamp()
{
	if ((digitalRead(BAL_BUTTON_PIN) == HIGH) && (_time_mute_button < millis()))
	{
		delay(300);
		if (digitalRead(BAL_BUTTON_PIN) == LOW)
		{
			_time_mute_button = millis() + 2000;
			_web_server->web_notify_bell_status();
		}
		else
		{
			_time_mute_button = millis() + 1000;
			_web_server->web_notify_bell_status();
		}
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
		_sensor_value = analogRead(SENSOR_MQ7);
		_sensor_volt = _sensor_value / 4096 * 5.0;
		_rs_gas = (5.0 - _sensor_volt) / _sensor_volt;
		_ratio = _rs_gas / _R0; //Replace R0 with the value found using the sketch above
		float x = 1538.46 * _ratio;
		_ppm = pow(x, -1.709) * 10;
		_buf_CO_sensor = String(_ppm);

		_buf_timer_read_CO_sensor = millis() + _timer_read_CO_sensor;
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

void Sensor::init() 
{
	_dht.begin();
	_bme.begin(0x76);
}

void Sensor::update() 
{
	button_power_lamp();

	read_BME_room_humidity();
	read_BME_room_temperature();
	read_BME_pressure();
	read_MQ_room_CO();
	read_DHT_humidity();
	read_DHT_temperature();
}
