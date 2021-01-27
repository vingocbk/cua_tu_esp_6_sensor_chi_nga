/*
Design by Ngoc Vi
Ha Noi 01/2021
*/

#ifndef chi_nga_h
#define chi_nga_h

#include "AppDebug.h"
#include "config.h"
#include <Arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include <EEPROM.h>
#include "webserver.h"
#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"





//normal mode
// void getStatus();
void motor_init();
void led_init();
void IRAM_ATTR inputSpeed();
void IRAM_ATTR dirhallSensor1();
void IRAM_ATTR dirhallSensor2();
void IRAM_ATTR dirhallSensor3();
void IRAM_ATTR dirhallSensor4();
void IRAM_ATTR dirhallSensor5();
void IRAM_ATTR dirhallSensor6();
void setpwmMotor();
void tickerupdate();
void caculateSpeed();
void setSpeedControl();
void Open();
void Close();
void Stop();

void setPwmLedLighton();
void setPwmLedLightoff();

void setLedApMode();
void checkButtonConfigClick();
void SetupConfigMode();
void StartWebServer();
String MacID();
void ReadEeprom();

struct motor
{
	/* data */
	int sensor_hall_duplicate;
	int count_pul_FG;
	int count_pul_distant;
	int pul_speed;
	int pre_pul_speed;
	int count_first_run;
	int count_calcu_speed;
	int mode_run;
	int time_return;
	int percent_low_in;
	int percent_low_out;
	int device_id;
	// int time_delay_analog;
	// int value_error_analog;
	// int pre_value_error_analog;
	// uint32_t time_check_analog;
	// int define_error_analog; 
	// int define_time_auto_close;
	// uint32_t time_auto_close;   
	int stop_speed;
	
	float speed_velectory;

	bool forward;
	bool daytay;
	bool status_stop;
	bool normal_mode;
	bool is_save_distant;
	bool first_run;
	bool flag_auto_close;
	bool hall_sensor_1;
	bool hall_sensor_2;
	bool hall_sensor_3;
	bool hall_sensor_4;
	bool hall_sensor_5;
	bool hall_sensor_6;

}control_motor;

struct led
{
	/* data */
	int red_after;
	int green_after;
	int blue_after;
	int red_before;
	int green_before;
	int blue_before;
	int count_change_led;
	bool status_led;
}control_led;

struct config
{
	/* data */
	bool turn_on_ap_mode;
	uint32_t time_click_button_config;
}configAPmode;


// CAN_device_t CAN_cfg;               // CAN Config
// const int rx_queue_size = 10;       // Receive Queue size


Ticker tickerCaculateSpeed(caculateSpeed, 100);   //every 100ms
Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
Ticker tickerSetPwmLedLightOn(setPwmLedLighton, 20, 255);	//every 20ms
Ticker tickerSetPwmLedLightOff(setPwmLedLightoff, 10, 255);
// Ticker tickerSetPwmLedLightChange(setPwmLedLightChange, 10, 255);


#endif
