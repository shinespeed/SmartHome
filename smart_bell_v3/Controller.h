#pragma once

#include "stdif.h"
#include "SPIFFS_DATA.h"

class Controller
{
public: 
   Controller();
   
   SPIFFS_DATA* _spiffs_data;

   void switch_bell_state(bool state);
   void switch_call_relay(bool state);
   void load_data_SPIFFS();
   void set_spiffs_data(SPIFFS_DATA* spiffs_data); 
   virtual void write_message_telegram(String chat_id, String text);
   virtual void test();
};
