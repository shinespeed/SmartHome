#pragma once

#include "stdif.h"

#define CHAT_ID_CONST "XXXXXXXXXXXX"
#define BOTtoken      "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

extern const char SSID_WIFI[];
extern const char PASSWORD_WIFI[];

///////////////////////////////////////////////////////////

extern bool status_bell;
extern bool status_call;

/////////////////////////PIN_OUT///////////////////////////

extern const short RELAY_PIN_L;
extern const short RELAY_PIN_R;
extern const short BUTTON_PIN;

////////////////////command_telegram///////////////////////

#define OFF_BELL   "/off_bell"
#define ON_BELL    "/on_bell"
