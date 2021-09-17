#include "WebServer.h"

using namespace std;

WebServer::WebServer(Controller* telegram, Controller* sensor, Controller* spiffs_data):
	_server(80),
	_ws("/ws"),
	_spiffs_data(spiffs_data),
	_telegram(telegram),
	_sensor(sensor)
{}

WebServer::~WebServer(){}

void WebServer::notify_clients(String name, byte state) 
{
	_ws.textAll(name + String(state));
}

void WebServer::handle_web_socket_message(void *arg, uint8_t *data, size_t len) 
{
	AwsFrameInfo *info = (AwsFrameInfo*)arg;
	if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
	{
		data[len] = 0;
		if (strcmp((char*)data, "switchBalLamp") == 0)
		{
			//_time_mute_button = millis() + 2000;
			notify_clients("PowerBalLamp=", switch_power_lamp());
		}
		else if (strcmp((char*)data, "switchHantarexPower") == 0)
		{
			notify_clients("PowerHantarex=", switch_power_hantarex());
		}
		else if (strcmp((char*)data, "switchPowerSensor") == 0)
		{
			notify_clients("PowerSensor=", switch_power_sensor());
		}
		else if (strcmp((char*)data, "switchMuteHantarex") == 0)
		{
			notify_clients("MuteHantarex=", switch_mute_hantarex());
		}
		else if (strcmp((char*)data, "switchRollerBlinds") == 0)
		{
			notify_clients("RollerBlinds=", switch_roller_blinds());
		}
		else if (strcmp((char*)data, "RollerBlinds=1") == 0)
		{
			_status_output._roller_blinds = 1;
			notify_clients("stateRollerBlinds=", 1);
		}
		else if (strcmp((char*)data, "RollerBlinds=0") == 0)
		{
			_status_output._roller_blinds = 0;
			notify_clients("stateRollerBlinds=", 0);
		}
		else if (strcmp((char*)data, "updateStateBlinds") == 0)
		{
			notify_clients("updateStateBlinds=", 1);
		}
		else if (strcmp((char*)data, "switchBellMode") == 0)
		{
			notify_clients("BellMode=", switch_bell_mode());
		}
		else if (strcmp((char*)data, "switchPowerSpeaker") == 0)
		{
			notify_clients("PowerSpeaker=", switch_power_speaker());
		}
	}
}

void WebServer::on_event(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
	void *arg, uint8_t *data, size_t len) 
{
	//if (type == WS_EVT_CONNECT) { numConnectServer++; }
	switch (type) {
	case WS_EVT_CONNECT:
		Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
		break;
	case WS_EVT_DISCONNECT:
		Serial.printf("WebSocket client #%u disconnected\n", client->id());
		break;
	case WS_EVT_DATA:
		handle_web_socket_message(arg, data, len);
		break;
	case WS_EVT_PONG:
	case WS_EVT_ERROR:
		break;
	}
}

void WebServer::not_found(AsyncWebServerRequest *request)
{
	request->send(404, "text/plain", "Not found");
}

String WebServer::state_switch(byte state_data) 
{
	if (state_data)
	{
		return "checked";
	}
	else
	{
		return "";
	}
}

String WebServer::processor(const String& var) 
{
	Serial.println(var);
	if (var == "STATE_BAL_LAMP")
	{
		return state_switch(_status_output._power_lamp);
	}
	else if (var == "STATE_HANTAREX_POWER")
	{
		return state_switch(_status_output._power_hantarex);
	}
	else if (var == "STATE_BELL_MODE")
	{
		Serial.print(_status_output._bell);
		init_bell_status_http();
		return state_switch(_status_output._bell);
	}
	else if (var == "STATE_ROLLER_BLINDS")
	{
		return state_switch(_status_output._roller_blinds);
	}
	else if (var == "STATE_SENSOR_POWER")
	{
		return state_switch(_status_output._power_sensor);
	}
	else if (var == "STATE_POWER_SPEAKER")
	{
		return state_switch(_status_output._speaker);
	}
	else if (var == "STATE_HANTAREX_MUTE")
	{
		return state_switch(_status_output._mute_hantarex);
	}
	else if (var == "TEMPERATURE_ROOM")
	{
		return _indication_sensor.BME_room_temperature;
	}
	else if (var == "HUMIDITY_ROOM")
	{
		return _indication_sensor.BME_room_humidity;
	}
	else if (var == "PRESSURE")
	{
		return _indication_sensor.BME_pressure;
	}
	else if (var == "TEMPERATURE")
	{
		return _indication_sensor.DHT_temperature;
	}
	else if (var == "HUMIDITY")
	{
		return _indication_sensor.DHT_humidity;
	}
	else if (var == "CO")
	{
		return _indication_sensor.CO_level;
	}
}

void WebServer::index(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/index.html", String(), false, bind(&WebServer::processor, this, placeholders::_1));
}

void WebServer::remote_hantarex(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/remote_hantarex.html", String(), false, bind(&WebServer::processor, this, placeholders::_1));
}

void WebServer::roller_blinds(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/roller_blinds.html", String(), false, bind(&WebServer::processor, this, placeholders::_1));
}

void WebServer::style_css(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/style.css", "text/css");
}

void WebServer::script_js(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/script.js", "application/javascript");
}

void WebServer::script_hantarex_js(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/script_hantarex.js", "application/javascript");
}

void WebServer::roller_blinds_js(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/roller_blinds.js", "application/javascript");
}

void WebServer::favicon_png(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/favicon.png", "image/png");
}

void WebServer::temperature_room(AsyncWebServerRequest *request)
{
	request->send_P(200, "text/plain", _indication_sensor.BME_room_temperature.c_str());
}

void WebServer::humidity_room(AsyncWebServerRequest *request) 
{
	request->send_P(200, "text/plain", _indication_sensor.BME_room_humidity.c_str());
}

void WebServer::pressure(AsyncWebServerRequest *request) 
{
	request->send_P(200, "text/plain", _indication_sensor.BME_pressure.c_str());
}

void WebServer::temperature(AsyncWebServerRequest *request) 
{
	request->send_P(200, "text/plain", _indication_sensor.DHT_temperature.c_str());
}

void WebServer::humidity(AsyncWebServerRequest *request)
{
	request->send_P(200, "text/plain", _indication_sensor.DHT_humidity.c_str());
}

void WebServer::co(AsyncWebServerRequest *request) 
{
	request->send_P(200, "text/plain", _indication_sensor.CO_level.c_str());
}

void WebServer::enter_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(ENTER_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::up_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(UP_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::down_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(DOWN_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::left_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(LEFT_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::right_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(RIGHT_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::info_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(INFO_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::ok_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(OK_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::back_menu_hantarex(AsyncWebServerRequest *request) 
{
	Serial1.write(BACK_MENU, 5);
	request->send(200, "text/plain", "ok");
}

void WebServer::init_bell_status_http() 
{
	HTTPClient http;
	http.begin("http://192.168.3.10/status_bell");
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		_status_output._bell = http.getString().toInt();
		Serial.println(_status_output._bell);
	}
	else {
		Serial.println("ERROR HTTP");
	}
	http.end();
}

void WebServer::init_web_socket()
{
	_ws.onEvent(std::bind(&WebServer::on_event, this, std::placeholders::_1, std::placeholders::_2, 
		std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	_server.addHandler(&_ws);
}

void WebServer::init()
{
	init_web_socket();

	_server.on("/", HTTP_GET, bind(&WebServer::index, this, placeholders::_1));
	_server.on("/remote_hantarex", HTTP_GET, bind(&WebServer::remote_hantarex, this, placeholders::_1));
	_server.on("/roller_blinds", HTTP_GET, bind(&WebServer::roller_blinds, this, placeholders::_1));
	_server.on("/style.css", HTTP_GET, bind(&WebServer::style_css, this, placeholders::_1));
	_server.on("/script.js", HTTP_GET, bind(&WebServer::script_js, this, placeholders::_1));
	_server.on("/script_hantarex.js", HTTP_GET, bind(&WebServer::script_hantarex_js, this, placeholders::_1));
	_server.on("/roller_blinds.js", HTTP_GET, bind(&WebServer::roller_blinds_js, this, placeholders::_1));
	_server.on("/favicon.png", HTTP_GET, bind(&WebServer::favicon_png, this, placeholders::_1));

	_server.on("/temperature_room", HTTP_GET, bind(&WebServer::temperature_room, this, placeholders::_1));
	_server.on("/humidity_room", HTTP_GET, bind(&WebServer::humidity_room, this, placeholders::_1));
	_server.on("/pressure", HTTP_GET, bind(&WebServer::pressure, this, placeholders::_1));
	_server.on("/temperature", HTTP_GET, bind(&WebServer::temperature, this, placeholders::_1));
	_server.on("/humidity", HTTP_GET, bind(&WebServer::humidity, this, placeholders::_1));
	_server.on("/co", HTTP_GET, bind(&WebServer::co, this, placeholders::_1));

	_server.on("/enter_menu_hantarex", HTTP_GET, bind(&WebServer::enter_menu_hantarex, this, placeholders::_1));
	_server.on("/up_menu_hantarex", HTTP_GET, bind(&WebServer::up_menu_hantarex, this, placeholders::_1));
	_server.on("/down_menu_hantarex", HTTP_GET, bind(&WebServer::down_menu_hantarex, this, placeholders::_1));
	_server.on("/left_menu_hantarex", HTTP_GET, bind(&WebServer::left_menu_hantarex, this, placeholders::_1));
	_server.on("/right_menu_hantarex", HTTP_GET, bind(&WebServer::right_menu_hantarex, this, placeholders::_1));
	_server.on("/info_menu_hantarex", HTTP_GET, bind(&WebServer::info_menu_hantarex, this, placeholders::_1));
	_server.on("/ok_menu_hantarex", HTTP_GET, bind(&WebServer::ok_menu_hantarex, this, placeholders::_1));
	_server.on("/back_menu_hantarex", HTTP_GET, bind(&WebServer::back_menu_hantarex, this, placeholders::_1));

	_server.onNotFound(std::bind(&WebServer::not_found, this, std::placeholders::_1));
	_server.begin();
}

void WebServer::update()
{
	_ws.cleanupClients();
}

void WebServer::web_notify_bell_status()
{
	notify_clients("PowerBalLamp=", _status_output._power_lamp);
}
