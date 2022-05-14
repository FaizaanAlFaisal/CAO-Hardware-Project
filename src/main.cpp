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


//firebase api key and real-time database link
#define API_KEY "[insert_firebase_api_key_here]"
#define DATABASE_URL "[insert_firebase_realtime_database_url_here]"

//information of authenticated user on firebase
#define USER_EMAIL "[insert_authenticated_email_here]"
#define USER_PASSWORD "[insert_authenticated_password_here]"

//firebase data object, anything read/written from database must be done through this object
FirebaseData firebaseData;

//authentication and configuration objects for data-object
FirebaseAuth auth;
FirebaseConfig config;

//wifi header + wifi signin information for ESP32
#include <WiFi.h>
#define WIFI_SSID "[insert_wifi_connection_name_here]"
#define WIFI_PASS "[insert_wifi_password_here]"

//hardware type depending on type of max7219 chip used

//number/type of connected devices
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
// #define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
// #define MAX_DEVICES 1

//pin declarations
#define CS_PIN 15 //chip select pin
#define CLK_PIN 23 //clock pin, 23 
#define DIN_PIN 18 //data-in pin, 18 is vspi pin (fast pin for serial information)

//using MD_Parola library to handle grid display
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
	//initialize display
	Display.begin();
	Display.setIntensity(0);
	Display.displayClear();

	//specify serial monitor timing + open serial monitor
	Serial.begin(115200);

	//connect to wifi
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED) //wait till wifi is connected
	{
	  Serial.print(".");
	  delay(500);
	}
	Serial.println("   Connected!"); //print confirmation message

	//initializing config with API key, and Real Time DataBase URL
	config.api_key = API_KEY;
	config.database_url = DATABASE_URL;
	config.token_status_callback = tokenStatusCallback;

	//initializing user authorization
	auth.user.email = USER_EMAIL;
	auth.user.password = USER_PASSWORD;

	//create firebase connection
	Firebase.begin(&config, &auth);
	Firebase.reconnectWiFi(true); //try reconnecting wifi if connection is lost
}


char displayData[128]; //use to store data being currently displayed, used to check if there has been updates on firebase
int checkDelay = 5000; //check firebase updates after checkDelay milliseconds
unsigned long long timeSinceLastCheck = 0; 
void loop() {

	if(Firebase.ready() && (millis() - timeSinceLastCheck > checkDelay || timeSinceLastCheck == 0))
	{ //confirm that firebase is ready to read from + it is time to check updates
		timeSinceLastCheck = millis(); //update timeSinceLastCheck

		if(Firebase.RTDB.getString(&firebaseData, "/readTest")) { //when triggered, check for updates in firebase data
			int resultCmp = strcmp(displayData, firebaseData.stringData().c_str()); //compare previous data to new data
			if (resultCmp != 0) //if new data is different (0 if equal), then update the led grid
			{
				strcpy(displayData, firebaseData.stringData().c_str()); //convert firebase info to a C-compatible string
				Display.displayClear(); //clear original display
				Serial.print("Data Recevied: ");
				Serial.println(displayData);
				Display.displayScroll(displayData, PA_RIGHT, PA_SCROLL_LEFT, 100); //set display info for led grid
			}
		}
	}

	if (Display.displayAnimate()) { //animate text scrolling
		Display.displayReset(); //if all text data has been printed, restart scrolling
	}
}
