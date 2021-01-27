#include "webserver.h"

AsyncWebServer server(HTTP_PORT);
configweb config_network;

const char index_html_handle_root[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
            <h1>CONFIG SETUP</h1><br>
			<form action="/mode_run">
				<h3>MODE RUN</h3><br>
				<input type="text" name="mode_run">
				<input type="submit" value="Save"><br>
			</form><br>
            <form action="/time_return">
				<h3>TIME RETURN</h3><br>
				<input type="text" name="time_return">
				<input type="submit" value="Save"><br>
			</form><br>
            <form action="/percent_low">
				<h3>PERCENT LOW</h3><br>
				<input type="text" name="percent_low_in">
                <input type="text" name="percent_low_out">
				<input type="submit" value="Save"><br>
			</form><br>
            <form action="/stop_speed">
				<h3>STOP SPEED</h3><br>
				<input type="text" name="stop_speed">
				<input type="submit" value="Save"><br>
			</form><br>
            <form action="/reset_distant">
				<h3>RESET DISTANT</h3>
				<input type="submit" value="Reset"><br>
			</form><br>
	    </body>
    </html>)rawliteral";

void handleRoot(AsyncWebServerRequest *request) {

	request->send_P(200, "text/html", index_html_handle_root);
}



//mode run
const char index_html_handle_save_mode_run[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP MODE RUN</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleSaveModeRun(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_save_mode_run);
    if (request->hasParam("mode_run")){
		String mode_run = request->getParam("mode_run")->value();
		ECHOLN("mode_run: " + mode_run);
		if(mode_run != ""){
            int mode_run_int = mode_run.toInt();
            EEPROM.write(EEPROM_SET_MODE_RUN_BEGIN, char(mode_run_int));
            EEPROM.commit();
            ECHOLN("Done writing mode run!");
            config_network.is_receive_data = true;
		}
	}

    

    
}

//time return
const char index_html_handle_save_time_return[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP TIME RETURN</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleSaveTimeReturn(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html_handle_save_time_return);
    //time return
    if (request->hasParam("time_return")){
		String time_return = request->getParam("time_return")->value();
		ECHOLN("time_return: " + time_return);
		if(time_return != ""){
            int time_return_int = time_return.toInt();
            EEPROM.write(EEPROM_SET_TIME_RETURN, char(time_return_int));
            EEPROM.commit();
            ECHOLN("Done writing time return!");
            config_network.is_receive_data = true;
		}
	}
}


//percent_low
const char index_html_handle_save_percent_low[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP PERCENT LOW</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleSavePercentLow(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html_handle_save_percent_low);
    //percent_low
    if (request->hasParam("percent_low_in")){
		String percent_low_in = request->getParam("percent_low_in")->value();
        String percent_low_out = request->getParam("percent_low_out")->value();
		ECHOLN("percent_low_in: " + percent_low_in);
        ECHOLN("percent_low_out: " + percent_low_out);
		if(percent_low_in != "" && percent_low_out != ""){
            int percent_low_in_int = percent_low_in.toInt();
            int percent_low_out_int = percent_low_out.toInt();
            EEPROM.write(EEPROM_SET_PERCENT_IN_LOW_SPEED, char(percent_low_in_int));
            EEPROM.write(EEPROM_SET_PERCENT_OUT_LOW_SPEED, char(percent_low_out_int));
            EEPROM.commit();
            ECHOLN("Done writing percent low!");
            config_network.is_receive_data = true;
		}
	}
}


//stop speed
const char index_html_handle_save_stop_speed[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP STOP SPEED</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleSaveStopSpeed(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html_handle_save_stop_speed);
    //time return
    if (request->hasParam("stop_speed")){
		String stop_speed = request->getParam("stop_speed")->value();
		ECHOLN("stop_speed: " + stop_speed);
		if(stop_speed != ""){
            int stop_speed_int = stop_speed.toInt();
            EEPROM.write(EEPROM_MIN_STOP_SPEED, char(stop_speed_int));
            EEPROM.commit();
            ECHOLN("Done writing stop speed!");
            config_network.is_receive_data = true;
		}
	}
}



const char index_html_handle_reset_distan[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE RESET DISTANT</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleResetDistant(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html_handle_reset_distan);
    EEPROM.write(EEPROM_DISTANT, 0);
    EEPROM.commit();
    ECHOLN("Reset Distant!");
    config_network.is_reset_distant = true;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}