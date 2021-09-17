#pragma once

#include "stdif.h"
#include "Time.h"
#include "Telegram.h"
#include "WebServer.h"
#include "Time.h"

class Bell
{
public:
   Bell();
   
   SPIFFS_DATA _spiffs_data;
   Telegram    _telegram;
   WebServer   _web_server;
   Time        _time;

   void update_bell();
   void button_event();
   void init_link_object();
   void start();
};
