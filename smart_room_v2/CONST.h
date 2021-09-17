#pragma once

#ifndef _CONST_H
#define _CONST_H

constexpr char* VERSION       = "Small Room v2";
constexpr char* SSID_WIFI     = "TITAN";
constexpr char* PASSWORD_WIFI = "boot7127102";

//setting for telegram bot
constexpr char* CHAT_ID_TELEGRAM = "-1001403273821";
constexpr char* BOT_TOKEN        = "1124654910:AAG3whPcDrG8cCtaA9ESQApYUrRKhVJqJ_8";

//input output interface esp32 
constexpr int BAL_LAMP_PIN     = 14;
constexpr int SENSOR_POWER_PIN = 23; 
constexpr int BAL_BUTTON_PIN   = 35; 
constexpr int RX_RS232         = 13; //hantarex remote
constexpr int TX_RS232         = 12; //hantarex remote
constexpr int DHT_PIN          = 27; //sensor temp and humidity on the street
constexpr int SENSOR_MQ7       = 34; //sensor level CO
constexpr int SPEAKER          = 26; 

//code rs-232 for hantarex monitor
constexpr byte MUTE_MENU[5]  = { 0xF4, 0x88, 0x00, 0xFD, 0x09 };
constexpr byte INFO_MENU[6]  = { 0xAA, 0x17, 0xFE, 0x01, 0x01, 0x17 };
constexpr byte OK_MENU[5]    = { 0xF4, 0x88, 0x00, 0xFD, 0x0F };
constexpr byte ENTER_MENU[5] = { 0x45, 0x88, 0x00, 0xFD, 0x0F };
constexpr byte UP_MENU[5]    = { 0xF4, 0x88, 0x00, 0xFD, 0x14 };
constexpr byte DOWN_MENU[5]  = { 0xF4, 0x88, 0x00, 0xFD, 0x15 };
constexpr byte LEFT_MENU[5]  = { 0xF4, 0x88, 0x00, 0xFD, 0x0B };
constexpr byte RIGHT_MENU[5] = { 0xF4, 0x88, 0x00, 0xFD, 0x0A };
constexpr byte BACK_MENU[5]  = { 0xF4, 0x88, 0x00, 0xFD, 0x10 };
constexpr byte OFF_MENU[6]   = { 0xF5, 0x88, 0x00, 0xFE, 0xFE, 0x00 };
constexpr byte ON_MENU[6]    = { 0xF5, 0x88, 0x00, 0xFE, 0xFE, 0x01 };

//setting signal fire danger
constexpr long SPEAKER_TIME_SONG  = 400;
constexpr int  TRIGGER_CO_LEVEL   = 100;
constexpr int  PING_SIGNAL        = 5000;
constexpr long MESSAGE_FIRE_DELAY = 30000;

#endif
