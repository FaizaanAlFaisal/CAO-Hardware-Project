#include <Arduino.h>

//libraries for led matrix functionality
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

//firebase headers
#include <Firebase_ESP_Client.h>
//additional headers for firebase tokens and realtime database functionality
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


//firebase api key and real-time database link
#define API_KEY "[insert_firebase_api_key_here]"
#define DATABASE_URL "[insert_firebase_realtime_database_url_here]"

//information of authenticated user on firebase
#define USER_EMAIL "[insert_authenticated_email_here]"
#define USER_PASSWORD "[insert_authenticated_password_here]"


//wifi header + wifi signin information for ESP32
#include <WiFi.h>
#define WIFI_SSID "[insert_wifi_connection_name_here]"
#define WIFI_PASS "[insert_wifi_password_here]"


//hardware type depending on type of max7219 chip used
//number/type of connected devices, use either pair depending on device you have
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
// #define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
// #define MAX_DEVICES 1


//pin declarations
#define CS_PIN 15 //chip select pin
#define CLK_PIN 23 //clock pin, 23 
#define DIN_PIN 18 //data-in pin, 18 is vspi pin (fast pin for serial information)


//headers to disable brownout detector in setup()
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
