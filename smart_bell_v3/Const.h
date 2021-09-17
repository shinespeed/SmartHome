#pragma once

#include "stdif.h"

#define CHAT_ID_CONST "-1001403273821"
#define BOTtoken      "966406106:AAHMjUkpMS8ctGyfxIYKU5z3O3Jybwo00iM"
///////////////////////////////////////////////////////////

constexpr char* PASSWORD_TELEGRAM = "7127102telegram?";

///////////////////////////////////////////////////////////

constexpr char* SSID_WIFI     = "TITAN";
constexpr char* PASSWORD_WIFI = "boot7127102";

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
