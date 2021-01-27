#include "cua_tu_esp_6_sensor_chi_nga.h"

extern AsyncWebServer server;
extern struct configweb config_network;

void ReadEeprom(){
    control_motor.device_id = EEPROM.read(EEPROM_DEVICE_ID);
    ECHO("Device ID = ");
    ECHOLN(control_motor.device_id);

    if(EEPROM.read(EEPROM_DISTANT) != 255 && EEPROM.read(EEPROM_DISTANT) != 0){
        control_motor.is_save_distant = true;
        control_motor.count_pul_distant = EEPROM.read(EEPROM_DISTANT);
        ECHO("Distant = ");
        ECHOLN(control_motor.count_pul_distant);
    }else{
        control_motor.is_save_distant = false;
        ECHOLN("is_save_distant fasle!");
    }

    control_motor.mode_run = char(EEPROM.read(EEPROM_SET_MODE_RUN_BEGIN));
    if(control_motor.mode_run != 1 && control_motor.mode_run != 2 && control_motor.mode_run != 3 && control_motor.mode_run != 4 && control_motor.mode_run != 5){
        control_motor.mode_run = 3;
        ECHOLN("read set moderun EEPROM fail, auto set 3!");
    }else{
        ECHO("Mode Run =  ");
        ECHOLN(control_motor.mode_run);
    }

    if(EEPROM.read(EEPROM_SET_TIME_RETURN) != 255 && EEPROM.read(EEPROM_SET_TIME_RETURN) != 0){
        control_motor.time_return = EEPROM.read(EEPROM_SET_TIME_RETURN);
        ECHO("time_return = ");
        ECHO(control_motor.time_return);
        ECHOLN("0 (ms)");
    }else{
        control_motor.time_return = 45;
        ECHOLN("isSetTimeReurn fasle, auto set 450(ms)");
    }


    if(EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED) != 255 && EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED) != 0
        && EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED) != 255 && EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED) != 0){
        control_motor.percent_low_in = EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED);
        ECHO("percentLowSpeedIn = ");
        ECHOLN(control_motor.percent_low_in);

        control_motor.percent_low_out = EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED);
        ECHO("percentLowSpeedOut = ");
        ECHOLN(control_motor.percent_low_out);
    }else{
        control_motor.percent_low_in = 20;
        control_motor.percent_low_out = 20;
        ECHOLN("isSavePercentLowSpeed fasle, auto set 20");
    }

     //stop speed
    control_motor.stop_speed = EEPROM.read(EEPROM_MIN_STOP_SPEED);
    if(control_motor.stop_speed == 0 || control_motor.stop_speed == 255){
        control_motor.stop_speed = MINSPEED;
    }
    ECHO("Min stop speed: ");
    ECHOLN(control_motor.stop_speed);

}

void motor_init(){
    //set varialble
    control_motor.normal_mode = true;
    control_motor.forward = true;
    control_motor.daytay = true;
    control_motor.first_run = true;
    configAPmode.turn_on_ap_mode = false;
    control_motor.pul_speed = 0;
    control_motor.pre_pul_speed = 0;
    control_motor.count_first_run = 0;
    control_motor.count_pul_FG = 0;

    config_network.is_receive_data = false;
    config_network.is_reset_distant = false;

}

void led_init(){
    //set varialble
    control_led.count_change_led = 0;

    // control_led.red_after = char(EEPROM.read(EEPROM_WIFI_LED_RED));
	// control_led.green_after  = char(EEPROM.read(EEPROM_WIFI_LED_GREEN));
    // control_led.blue_after  = char(EEPROM.read(EEPROM_WIFI_LED_BLUE));
    control_led.red_after = 255;
    control_led.green_after = 255;
    control_led.blue_after = 255;
    control_led.red_before  = control_led.red_after;
    control_led.green_before  = control_led.green_after;
    control_led.blue_before  = control_led.blue_after;
	ledcWrite(LED_CHANNEL_R, control_led.red_after);
    ledcWrite(LED_CHANNEL_G, control_led.green_after);
    ledcWrite(LED_CHANNEL_B, control_led.blue_after);
    ECHO("red: ");
    ECHOLN(control_led.red_after);
    ECHO("green: ");
    ECHOLN(control_led.green_after);
    ECHO("blue: ");
    ECHOLN(control_led.blue_after);
}


void Open(){
    ECHOLN("open");

    control_motor.status_stop = false;
    control_motor.forward = true;
    control_motor.daytay = false;
    control_motor.count_calcu_speed = 0;
    control_led.status_led = true;
    digitalWrite(DIR, QUAY_THUAN);
    tickerCaculateSpeed.start();

	//bat den led
	if(control_led.count_change_led == 0){
		tickerSetPwmLedLightOff.stop();
		tickerSetPwmLedLightOn.start();
	}
}

void Close(){
    ECHOLN("close");

    control_motor.status_stop = false;
    control_motor.forward = false;
    control_motor.daytay = false;
    control_motor.count_calcu_speed = 0;
    
    digitalWrite(DIR, QUAY_NGHICH);
    tickerCaculateSpeed.start();
    tickerSetPwmLedLightOn.stop();
}


void Stop(){
    tickerCaculateSpeed.stop();

    if(control_motor.forward){
        digitalWrite(DIR, QUAY_NGHICH);     //cho dong co quay nghich
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_MAX);
        delay(control_motor.time_return*10);
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_STOP);
        control_motor.forward = false;
    }else{
        digitalWrite(DIR, QUAY_THUAN);
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_MAX);
        delay(control_motor.time_return*10);
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_STOP);
        control_motor.forward = true;
    }

    control_motor.status_stop = true;
    control_motor.daytay = true;
    control_motor.count_calcu_speed = 0;

}


void caculateSpeed(){
    //ECHOLN("speed");
    if(control_motor.count_calcu_speed <= 30){
        control_motor.count_calcu_speed++;
    }
    control_motor.speed_velectory = (control_motor.pul_speed - control_motor.pre_pul_speed)/(0.1*6);
    control_motor.pre_pul_speed = control_motor.pul_speed;
    
    // ECHO("van toc: ");
    // ECHOLN(speed);
    if(abs(control_motor.speed_velectory) <= control_motor.stop_speed && control_motor.count_calcu_speed >= 5){   //sau 5 lan chay thi moi tinh den van toc
        ECHOLN("Da dung lai");
        tickerCaculateSpeed.stop();

        if(control_motor.is_save_distant && control_motor.first_run){
            control_motor.first_run = false;
            control_motor.count_pul_FG = 0;
            control_motor.pre_pul_speed = 0;
        }
        

        if(control_motor.first_run && control_motor.count_first_run < 5){
            control_motor.count_first_run++;
        }
        
        if(control_motor.first_run == true && control_motor.count_first_run == 1){
            control_motor.count_pul_FG = 0;
            control_motor.pre_pul_speed = 0;
        }

        else if(control_motor.first_run == true && control_motor.count_first_run == 2){
            control_motor.count_pul_distant = abs(control_motor.count_pul_FG);
            EEPROM.write(EEPROM_DISTANT, control_motor.count_pul_distant);
            EEPROM.commit();
            control_motor.is_save_distant = true;
            if(control_motor.count_pul_FG < 0){
                control_motor.count_pul_FG = 0;
                control_motor.pre_pul_speed = 0;
            }
            control_motor.first_run = false;
        }


        if(!control_motor.first_run && control_motor.count_pul_FG <= 3){
			//tat den
			tickerSetPwmLedLightOff.stop();
			tickerSetPwmLedLightOn.stop();
			control_led.count_change_led = 0;
			ledcWrite(LED_CHANNEL_R, 255);
            ledcWrite(LED_CHANNEL_G, 255);
            ledcWrite(LED_CHANNEL_B, 255);

			control_motor.count_pul_FG = 0;
        }else if(!control_motor.first_run && (control_motor.count_pul_distant -3) <= control_motor.count_pul_FG){
            //bat den
			tickerSetPwmLedLightOff.stop();
			tickerSetPwmLedLightOn.stop();
			control_led.count_change_led = 0;
			ledcWrite(LED_CHANNEL_R, 0);
            ledcWrite(LED_CHANNEL_G, 0);
            ledcWrite(LED_CHANNEL_B, 0);
			
			control_motor.count_pul_FG = control_motor.count_pul_distant;
        }
        
        control_motor.count_calcu_speed = 0;

        Stop();
    }
}



void setLedApMode() {
    digitalWrite(ledTestWifi, !digitalRead(ledTestWifi));
}

void setPwmLedLighton(){
    control_led.count_change_led++;
    float out_led_red, out_led_green, out_led_blue;
    out_led_red = 255 - control_led.count_change_led;
    out_led_red = abs(out_led_red);
    ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red));

    out_led_green = 255 - control_led.count_change_led;
    out_led_green = abs(out_led_green);
    ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green));

    out_led_blue = 255 - control_led.count_change_led;
    out_led_blue = abs(out_led_blue);
    ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue));

    if(control_led.count_change_led == 255){
        ECHOLN("On Led");
		control_led.count_change_led = 0;
	}

}

void setPwmLedLightoff(){
    control_led.count_change_led++;
	float out_led_red, out_led_green, out_led_blue;
    out_led_red = control_led.count_change_led;
    out_led_red = abs(out_led_red);
	ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red));
    
    out_led_green = control_led.count_change_led;
    out_led_green = abs(out_led_green);
	ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green));
    
    out_led_blue = control_led.count_change_led;
    out_led_blue = abs(out_led_blue);
	ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue));
    
    if(control_led.count_change_led == 255){
        ECHOLN("Off Led");
		control_led.count_change_led = 0;
	}

}


void IRAM_ATTR dirhallSensor1(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 1){
        control_motor.sensor_hall_duplicate = 1;
        ECHOLN("1");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_2 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_2 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_6 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_6 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_1 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor2(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 2){
        control_motor.sensor_hall_duplicate = 2;
        ECHOLN("2");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_3 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_3 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_1 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_1 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_2 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor3(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 3){
        control_motor.sensor_hall_duplicate = 3;
        ECHOLN("3");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_4 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_4 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_2 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_2 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_3 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor4(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 4){
        control_motor.sensor_hall_duplicate = 4;
        ECHOLN("4");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_5 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_5 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_3 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_3 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_4 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor5(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 5){
        control_motor.sensor_hall_duplicate = 5;
        ECHOLN("5");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_6 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_6 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_4 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_4 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_5 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor6(){      //nhan du lieu tu cam bien ben ngoai
    if(control_motor.sensor_hall_duplicate != 6){
        control_motor.sensor_hall_duplicate = 6;
        ECHOLN("6");
        if(control_motor.forward){
            control_motor.count_pul_FG++;
        }else{
            control_motor.count_pul_FG--;
        }

        if(control_motor.daytay && control_motor.status_stop){
            // control_motor.sensor_hall_duplicate = 0;
            if(control_motor.hall_sensor_1 == true){
                //cho dong co chay thuan
                control_motor.hall_sensor_1 = false;
                // ECHOLN("open");
                Open();
            }
            else if(control_motor.hall_sensor_5 == true){
                //cho dong co chay nghich
                control_motor.hall_sensor_5 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                control_motor.hall_sensor_6 = true;
            }
        }
    }
    
}


void IRAM_ATTR inputSpeed(){
    if(control_motor.forward){
        control_motor.pul_speed++;
    }else{
        control_motor.pul_speed--;
    }
    // ECHOLN(control_motor.pul_speed);
}



void tickerupdate(){
    tickerCaculateSpeed.update();
    tickerSetApMode.update();
	tickerSetPwmLedLightOn.update();
	tickerSetPwmLedLightOff.update();
    // tickerSetPwmLedLightChange.update();
}



void setSpeedControl(){
    float inside = ((float)control_motor.percent_low_in/100)*control_motor.count_pul_distant;
    float outside = ((100 - (float)control_motor.percent_low_out)/100)*control_motor.count_pul_distant;
    if(!control_motor.first_run && !control_motor.status_stop && control_motor.forward && control_motor.count_pul_FG < outside){
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_MAX);
        // ECHOLN("MOTOR_MODE_SPEED_MAX");
    }
    
    else if(!control_motor.first_run && !control_motor.status_stop && !control_motor.forward && control_motor.count_pul_FG > inside){
        ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_MAX);  
        // ECHOLN("MOTOR_MODE_SPEED_MAX"); 
    }
    
    else if(!control_motor.status_stop && (control_motor.first_run || (control_motor.forward && control_motor.count_pul_FG > outside) || (!control_motor.forward && control_motor.count_pul_FG < inside))){
        switch (control_motor.mode_run)
        {
        case 1:
            ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_1);
            // ECHOLN("MOTOR_MODE_SPEED_1");
            break;
        case 2:
            ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_2);
            // ECHOLN("MOTOR_MODE_SPEED_2");
            break;
        case 3:
            ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_3);
            // ECHOLN("MOTOR_MODE_SPEED_3");
            break;
        case 4:
            ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_4);
            // ECHOLN("MOTOR_MODE_SPEED_4");
            break;
        default:
            break;
        }
    }
    // else{
    //     ledcWrite(MOTOR_CHANNEL, MOTOR_MODE_SPEED_STOP);
    // }
}


String MacID(){
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    String macID;
    WiFi.mode(WIFI_AP);
    WiFi.softAPmacAddress(mac);
    macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    return macID;
}

void SetupConfigMode(){
    ECHOLN("[WifiService][setupAP] Open AP....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    // server.close();
    delay(500);
    WiFi.mode(WIFI_AP);
    String SSID_AP_MODE = SSID_PRE_AP_MODE + MacID();
    WiFi.softAP(SSID_AP_MODE.c_str(), PASSWORD_AP_MODE);
    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);
    ECHOLN(SSID_AP_MODE);

    ECHOLN("[WifiService][setupAP] Softap is running!");
    IPAddress myIP = WiFi.softAPIP();
    ECHO("[WifiService][setupAP] IP address: ");
    ECHOLN(myIP);
}


void StartWebServer(){
	server.on("/", HTTP_GET, handleRoot);
	server.on("/mode_run", HTTP_GET, handleSaveModeRun);
	server.on("/time_return", HTTP_GET, handleSaveTimeReturn);
	server.on("/percent_low", HTTP_GET, handleSavePercentLow);
	server.on("/stop_speed", HTTP_GET, handleSaveStopSpeed);
	server.on("/reset_distant", HTTP_GET, handleResetDistant);
	server.onNotFound(notFound);
	server.begin();
	Serial.println( "HTTP server started" );
}


void checkButtonConfigClick(){
    //hold to config mode
    if(digitalRead(PIN_CONFIG) == HIGH){
        configAPmode.time_click_button_config = millis();
    }
    if(digitalRead(PIN_CONFIG) == LOW && (configAPmode.time_click_button_config + CONFIG_HOLD_TIME) <= millis()){
        configAPmode.time_click_button_config = millis();
        tickerSetApMode.start();
        // control_motor.normal_mode = false;
        configAPmode.turn_on_ap_mode = true;
        SetupConfigMode();
        StartWebServer();
    }
}

void setup() {
  // put your setup code here, to run once:
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    EEPROM.begin(EEPROM_WIFI_MAX_CLEAR);
     
    ledcSetup(LED_CHANNEL_R, 1000, 8); // 1 kHz PWM, 8-bit resolution
    ledcSetup(LED_CHANNEL_G, 1000, 8); // 1 kHz PWM, 8-bit resolution
    ledcSetup(LED_CHANNEL_B, 1000, 8); // 1 kHz PWM, 8-bit resolution
    ledcSetup(MOTOR_CHANNEL, 30000, 8); // 30 kHz PWM, 8-bit resolution

    ledcAttachPin(PIN_LED_LIGHT_R, LED_CHANNEL_R); // analog pin to channel led_R
    ledcAttachPin(PIN_LED_LIGHT_G, LED_CHANNEL_G); // analog pin to channel led_G
    ledcAttachPin(PIN_LED_LIGHT_B, LED_CHANNEL_B); // analog pin to channel led_B
    ledcAttachPin(PWM, MOTOR_CHANNEL);              // analog pin to channel Motor
    
    ECHOLN("");
    ECHOLN("START!!!");
    pinMode(DIR, OUTPUT);
    pinMode(PIN_CONFIG, INPUT_PULLUP);
    pinMode(inputFG, INPUT_PULLUP);
    pinMode(hallSensor1, INPUT_PULLUP);
    pinMode(hallSensor2, INPUT_PULLUP);
    pinMode(hallSensor3, INPUT_PULLUP);
    pinMode(hallSensor4, INPUT_PULLUP);
    pinMode(hallSensor5, INPUT_PULLUP);
    pinMode(hallSensor6, INPUT_PULLUP);
    pinMode(ledTestWifi, OUTPUT);
    digitalWrite(ledTestWifi, HIGH);
    
    ReadEeprom();
    motor_init();
    led_init();


    attachInterrupt(digitalPinToInterrupt(hallSensor1), dirhallSensor1, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor2), dirhallSensor2, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor3), dirhallSensor3, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor4), dirhallSensor4, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor5), dirhallSensor5, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor6), dirhallSensor6, RISING);
    attachInterrupt(digitalPinToInterrupt(inputFG), inputSpeed, FALLING);


    Close();

}

void loop() {
    // put your main code here, to run repeatedly:

    if(!control_motor.forward && control_led.status_led && !control_motor.status_stop  && control_motor.count_pul_FG <= 15){
        control_led.status_led = false;
        tickerSetPwmLedLightOn.stop();
        tickerSetPwmLedLightOff.start();
    }
    checkButtonConfigClick();
    setSpeedControl();
    tickerupdate();

    if(config_network.is_receive_data){
        ReadEeprom();
        config_network.is_receive_data = false;
    }
    if(config_network.is_reset_distant){
        control_motor.is_save_distant = false;
        control_motor.first_run = true;
        control_motor.count_first_run = 0;
        config_network.is_reset_distant = false;
    }

}
