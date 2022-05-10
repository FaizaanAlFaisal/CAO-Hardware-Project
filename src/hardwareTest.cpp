//

#include <Arduino.h>

//libraries for led matrix functionality
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

//hardware type depending on type of max7219 chip used
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

#define MAX_DEVICES 1
#define CS_PIN 15

//using MD_Parola library to handle grid display
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
	Display.begin();
	Display.setIntensity(0);
	Display.displayClear();
	Display.displayScroll("TEST", PA_RIGHT, PA_SCROLL_LEFT, 150);
	//display text "TEST" going from right to left, demo included in /demos
}

void loop() {
	if (Display.displayAnimate()) { //animate text scrolling
		Display.displayReset(); //resets to originally specified condition
	}
}
