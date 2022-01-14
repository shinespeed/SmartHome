#pragma once

using namespace std;

#include "stdif.h"
#include "KeyboardTelegram.h"
#include "Calling.h"
#include "Controller.h"
#include "Time.h"

class Telegram: public Controller
{
public:
    Telegram(Controller* _web);

    Controller*          _web;
    Time*                _time;
    WiFiClientSecure     _client;
    UniversalTelegramBot _bot;
    KeyboardTelegram     _keyboard;
    Telegraph            _telegraph;
    Call                 _call;
  
    const String _CHAT_ID;
    String       _send_text;
    long         _bot_last_time;   
    short        _bot_mtbs;
    
    void update();
    void write_message_telegram(String chat_id, String text);
    void command_telegram(const String& text, const String& chat_id, const String& from_name);
    void allowed_commands(const String& text, const String& chat_id, const String& from_name);
    void handle_new_messages(int num_new_messages);
    void update_write_data_ESP(String chat_id, int i, int& numNewMessages);
    void call_bell(const String& from_name, const String& chat_id);
    void start_message();
    void button_message();
    void view_keyboard(const String& command, const String& chat_id, const String& chat_text = "Keyboard", const String& change_command = "");
    void telegraph_send_text(const String& text, const String& chat_id, const String& from_name);
    void set_pointer_time(Time* time);
    
    bool mute_bell(const String& chat_id, String& mute_str);
    bool cheak_password(const String& password);
    bool verificate_id(const String& chat_id);
    
    void add_verificate_id(const String& chat_id);

    String ip_address_to_string(const IPAddress& ip_address);
    String status_message(const String& chat_id);
    String read_message();
    
    void test();
};
