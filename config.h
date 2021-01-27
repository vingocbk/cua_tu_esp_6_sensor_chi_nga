/*
Design by Ngoc Vi
Ha Noi 01/2021
*/

#ifndef CONFIG_H
#define CONFIG_H


#define hallSensor1 14
#define hallSensor2 27  
#define hallSensor3 26
#define hallSensor4 4
#define hallSensor5 16
#define hallSensor6 17
#define ledTestWifi 32
#define inputFG 21
#define PWM 18
#define DIR 19
#define ANALOG_READ_BUTTON 13 //34
//#define PIN_CONFIG 0          //new board
#define PIN_CONFIG 25   //old board
#define PIN_TOUCH_SENSOR T5
#define PIN_LED_LIGHT_R 22
#define PIN_LED_LIGHT_G 23  //5 with design
#define PIN_LED_LIGHT_B 5   //23 with design
#define LED_CHANNEL_R 1
#define LED_CHANNEL_G 2
#define LED_CHANNEL_B 3
#define MOTOR_CHANNEL 4

#define WL_MAC_ADDR_LENGTH 6
#define SSID_PRE_AP_MODE "AvyInterior-"
#define PASSWORD_AP_MODE "123456789"

// old motor
//#define QUAY_THUAN HIGH
//#define QUAY_NGHICH LOW

  // new motor
 #define QUAY_THUAN LOW
 #define QUAY_NGHICH HIGH

#define MINSPEED 5

#define RESPONSE_LENGTH                     2048     //do dai data nhan ve tu tablet
#define EEPROM_DEVICE_ID                    65
#define EEPROM_SET_MODE_RUN_BEGIN           149
#define EEPROM_DISTANT                      150
#define EEPROM_SET_PERCENT_OUT_LOW_SPEED    151
#define EEPROM_SET_PERCENT_IN_LOW_SPEED     152
#define EEPROM_SET_TIME_RETURN              153
#define EEPROM_WIFI_LED_RED                 154
#define EEPROM_WIFI_LED_GREEN               155 
#define EEPROM_WIFI_LED_BLUE                156
#define EEPROM_WIFI_IS_TOUCH_SENSOR         157
#define EEPROM_DELAY_MQTT_TO_LOW_SPEAKER    158
#define EEPROM_COUNT_TIME_ANALOG            159
#define EEPROM_VALUE_ERROR_ANALOG           160
#define EEPROM_TIME_AUTO_CLOSE              161
#define EEPROM_MIN_STOP_SPEED               162
#define EEPROM_WIFI_MAX_CLEAR               256


#define MOTOR_MODE_SPEED_MAX      255
#define MOTOR_MODE_SPEED_STOP     0
#define MOTOR_MODE_SPEED_1        200
#define MOTOR_MODE_SPEED_2        150
#define MOTOR_MODE_SPEED_3        100
#define MOTOR_MODE_SPEED_4        50


#define MSG_MASTER_ID         0
#define MSG_GET_STATUS        1
#define MSG_CONTROL_OPEN      2
#define MSG_CONTROL_CLOSE     3
#define MSG_CONTROL_STOP      4
#define MSG_CONTROL_LED_VOICE 5
#define MSG_CONTROL_LED_HAND  6
#define MSG_RESET_DISTANT     7
#define MSG_TIME_RETURN       8
#define MSG_MODE_RUN          9
#define MSG_PERCENT_LOW       10
#define MSG_DELAY_ANALOG      11
#define MSG_ERROR_ANALOG      12
#define MSG_AUTO_CLOSE        13
#define MSG_MIN_STOP_SPEED    14

#define MSG_SET_ID       100


#define CONFIG_HOLD_TIME 5000
// #define TIME_CHECK_ANALOG 500       //500ms check 1 lan

#endif