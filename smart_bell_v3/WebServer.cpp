#include "WebServer.h"

WebServer::WebServer(Controller* telegram):  _telegram(telegram),
                                             server(80)
{}

void WebServer::init_server()
{
	server.on("/off", HTTP_GET, [this](AsyncWebServerRequest *request) {
		change_mode_bell = true;
		switch_bell_state(false);
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/on", HTTP_GET, [this](AsyncWebServerRequest *request) {
		change_mode_bell = true;
		switch_bell_state(true);
		request->send_P(200, "text/plain", "ok");
	});
	server.on("/status_bell", HTTP_GET, [this](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", String(status_bell).c_str());
	});

	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

	server.begin();
}

void WebServer::update_message_telegram() 
{
	if (change_mode_bell) 
	{
		if (status_bell == true) 
		{
			_telegram->write_message_telegram(CHAT_ID_CONST, "BELL ON");
		}
		else
		{
			_telegram->write_message_telegram(CHAT_ID_CONST, "BELL OFF");
		}
		change_mode_bell = false;
	}

}
