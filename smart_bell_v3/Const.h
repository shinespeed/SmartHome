#pragma once

#include "stdif.h"

#define CHAT_ID_CONST "xxxxxxxxxxxxxx"
#define BOTtoken      "xxxxxxxxxxxxxx"
///////////////////////////////////////////////////////////

constexpr char* PASSWORD_TELEGRAM = "xxxxxxxxxxxxx";

///////////////////////////////////////////////////////////

constexpr char* SSID_WIFI     = "xxxxxxxxxxx";
constexpr char* PASSWORD_WIFI = "xxxxxxxxxxx";

///////////////////////////////////////////////////////////

extern bool status_bell;
extern bool status_call;

/////////////////////////PIN_OUT///////////////////////////

constexpr short RELAY_PIN_L = 18;
constexpr short RELAY_PIN_R = 2;
constexpr short BUTTON_PIN  = 16;

////////////////////command_telegram///////////////////////

#define OFF_BELL   "/off_bell"
#define ON_BELL    "/on_bell"
