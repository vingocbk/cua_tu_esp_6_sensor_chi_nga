#ifndef webserver_h
#define webserver_h

#include "EEPROM.h"
#include "AppDebug.h"
#include "config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#define HTTP_PORT 80

struct configweb
{
	/* data */
	bool is_receive_data;
    bool is_reset_distant;
};

void handleRoot(AsyncWebServerRequest *request);
void handleSaveModeRun(AsyncWebServerRequest *request);
void handleSaveTimeReturn(AsyncWebServerRequest *request);
void handleSavePercentLow(AsyncWebServerRequest *request);
void handleSaveStopSpeed(AsyncWebServerRequest *request);
void handleResetDistant(AsyncWebServerRequest *request);
void notFound(AsyncWebServerRequest *request);

#endif