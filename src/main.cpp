//self-made header: has headers and macros of Firebase and LED Matrix Setup
#include "setupHeaders.h"


//firebase data object, anything read/written from database must be done through this object
FirebaseData firebaseData;
//authentication and configuration objects for data-object
FirebaseAuth auth;
FirebaseConfig config;


//LED matrix object creation
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


//variables to manage LED display
char displayData[130]; //use to store data being currently displayed, used to check if there has been updates on firebase
int intensity = 0; //manage LED intensity on matrix, 0 to 15, 0 is lowest
int scrollSpeed = 150; //manage speed of text scrolling, 0 to inf, but restricted to 100-500, 100 being fastest

void setup() {
	//specify serial monitor timing + open serial monitor
	Serial.begin(115200);
	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

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
	Firebase.reconnectWiFi(true); //try reconnecting to wifi if connection is lost


	//on startup, read from firebase and initialize led matrix with info already there
	Firebase.RTDB.getString(&firebaseData, "/displayData/message");
	strcpy(displayData, firebaseData.stringData().c_str()); //convert firebase string to a C-compatible string
	
	Firebase.RTDB.getInt(&firebaseData, "/displayData/scrollSpeed");
	scrollSpeed = firebaseData.intData();
	
	Firebase.RTDB.getInt(&firebaseData, "/displayData/intensity");
	intensity = firebaseData.intData();

	//initialize display
	Display.begin();
	Display.displayClear(); //clear display of all previous
	Display.setIntensity(intensity);
	Display.displayScroll(displayData, PA_RIGHT, PA_SCROLL_LEFT, scrollSpeed); //set display info for led matrix

	Firebase.RTDB.setBool(&firebaseData, "/displayData/isBeingDisplayed", true); //set status of display message to true
}




unsigned int timeSinceLastCheck = 0; 
const int checkDelay = 3000; //check firebase updates after checkDelay milliseconds

void loop() { //runs on core 1 of ESP32 by default, onlyanimates LED grid

	if(Firebase.ready() && (millis() - timeSinceLastCheck > checkDelay || timeSinceLastCheck == 0))
	{ //confirm that firebase is ready to read from + it is time to check updates
		
		timeSinceLastCheck = millis(); //update timeSinceLastCheck

		if(Firebase.RTDB.getBool(&firebaseData, "/displayData/isBeingDisplayed")) {
			if (!firebaseData.boolData()) { //if information on firebase is currently not being displayed, then:

				Firebase.RTDB.getString(&firebaseData, "/displayData/message");
				strcpy(displayData, firebaseData.stringData().c_str()); //convert firebase info to a C-compatible string
				Firebase.RTDB.getInt(&firebaseData, "/displayData/scrollSpeed");
				scrollSpeed = firebaseData.intData();
				Firebase.RTDB.getInt(&firebaseData, "/displayData/intensity");
				intensity = firebaseData.intData();

				Display.displayClear(); //clear original display
				Serial.print("Data Recevied: ");
				Serial.println(displayData);
				Display.setIntensity(intensity);
				Display.displayScroll(displayData, PA_RIGHT, PA_SCROLL_LEFT, scrollSpeed); //set display info for led grid

				Firebase.RTDB.setBool(&firebaseData, "/displayData/isBeingDisplayed", true); 
				//set "isBeingDisplayed" to true, so this check does not occur until a change is made to firebase RTDB
			}
		}
	}

	if (Display.displayAnimate()) { //animate text scrolling
		Display.displayReset(); //if all text data has been printed, restart scrolling
	}
}
