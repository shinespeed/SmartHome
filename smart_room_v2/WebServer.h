#pragma once

#ifndef _WEB_SERVER_H
#define _WEB_SERVER_H

#include "Controller.h"
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <AsyncTCP.h>
#include <functional>


class WebServer: public Controller
{
private:
	HTTPClient     _http;
	AsyncWebServer _server;
	AsyncWebSocket _ws;

	Controller* _telegram;
	Controller* _sensor;
	Controller* _spiffs_data;

public:
	WebServer(Controller* telegram, Controller* sensor, Controller* spiffs_data);
	~WebServer();

	void notify_clients(String name, byte state);
	void handle_web_socket_message(void *arg, uint8_t *data, size_t len);
    void on_event(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
	void not_found(AsyncWebServerRequest *request);

	String state_switch(byte stateData);
	String processor(const String& var);

	void index(AsyncWebServerRequest *request);
	void remote_hantarex(AsyncWebServerRequest *request);
	void roller_blinds(AsyncWebServerRequest *request);
	void style_css(AsyncWebServerRequest *request);
	void script_js(AsyncWebServerRequest *request);
	void script_hantarex_js(AsyncWebServerRequest *request);
	void roller_blinds_js(AsyncWebServerRequest *request);
	void favicon_png(AsyncWebServerRequest *request);

	void temperature_room(AsyncWebServerRequest *request);
	void humidity_room(AsyncWebServerRequest *request);
	void pressure(AsyncWebServerRequest *request);
	void temperature(AsyncWebServerRequest *request);
	void humidity(AsyncWebServerRequest *request);
	void co(AsyncWebServerRequest *request);

	void enter_menu_hantarex(AsyncWebServerRequest *request);
	void up_menu_hantarex(AsyncWebServerRequest *request);
	void down_menu_hantarex(AsyncWebServerRequest *request);
	void left_menu_hantarex(AsyncWebServerRequest *request);
	void right_menu_hantarex(AsyncWebServerRequest *request);
	void info_menu_hantarex(AsyncWebServerRequest *request);
	void ok_menu_hantarex(AsyncWebServerRequest *request);
	void back_menu_hantarex(AsyncWebServerRequest *request);

	void init_bell_status_http();
	void init_web_socket();
	void init();
	void update();

	void web_notify_bell_status();
};

#endif
