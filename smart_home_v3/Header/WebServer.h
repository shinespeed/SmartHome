#pragma once

#include "stdif.h"
#include "Controller.h"

class WebServer: public Controller
{
public:
    WebServer();

    AsyncWebServer server;
    Controller* _telegram;

    bool change_mode_bell;
    
    void init_server(); 
    void send_message(const String& text); 
    void set_pointer_telegram(Controller* telegram);
    void update_message_telegram();
};
