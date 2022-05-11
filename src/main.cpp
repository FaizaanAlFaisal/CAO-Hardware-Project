//This file contains the main driver code for the program, including all firebase and wifi connection stuff

#include <Arduino.h>

//libraries for led matrix functionality
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

//firebase header
#include <Firebase_ESP_Client.h>

//Provide the token generation process info
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions
#include <addons/RTDBHelper.h>

#define API_KEY "[insert_here]"
#define DATABASE_URL "[insert_here]"

#define USER_EMAIL "[insert_here]"
#define USER_PASSWORD "[insert_here]"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


//wifi header for ESP32
#include <WiFi.h>
#define WIFI_SSID "[insert_here]"
#define WIFI_PASS "[insert_here]"

//hardware type depending on type of max7219 chip used
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

//number of connected devices, and specifying chip select pin
#define MAX_DEVICES 4
#define CS_PIN 15

//using MD_Parola library to handle grid display
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
	//initialize display
	Display.begin();
	Display.setIntensity(0);
	Display.displayClear();

	//specify serial monitor timing
	Serial.begin(115200);

	//connect to wifi
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED) //wait till wifi is connected
	{
	  Serial.print(".");
	  delay(300);
	}
	Serial.print("   Connected!");
	Serial.println();

	//initializing config with API key, and Real Time DataBase URL
	config.api_key = API_KEY;
	config.database_url = DATABASE_URL;
	config.token_status_callback = tokenStatusCallback;

	//initializing user authorization
	auth.user.email = USER_EMAIL;
	auth.user.password = USER_PASSWORD;

	//create firebase connection
	Firebase.begin(&config, &auth);
 
	Firebase.reconnectWiFi(true);
}

void loop() {
	//to be changed
	//read info from firebase
	String s = Firebase.RTDB.getString(&fbdo, F("/readTest")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
	delay(2000);
	Serial.print(s); //print read info to serial monitor for testing purposes
	Display.displayScroll(s.c_str(), PA_RIGHT, PA_SCROLL_LEFT, 150); //set display info for led grid
	delay(2000);

	if (Display.displayAnimate()) { //animate text scrolling
		Display.displayReset(); //resets to originally specified condition
	}
	delay(2000);

}
