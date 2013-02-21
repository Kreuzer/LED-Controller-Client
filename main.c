/*
 * main.c
 *
 *  Created on: 14.02.2013
 *  Author: Frederick
 *
 *  Beschreibung auf https://github.com/Kreuzer/LED-Controller-Client/wiki
 */

#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"


struct hsv hsv;


// Mode 1: fading durch alle Farben:
void mode1(unsigned char speed, unsigned char brightness){

	static unsigned char counter = 0;

	if(counter==speed){
		hsv.h++;
		hsv.s = 255;
		hsv.v = brightness;
		counter = 0;
	}
	else
		counter++;
}

// Mode 3: 7-Farben (direkt) schnell wechseln
void mode3(unsigned char speed, unsigned char brightness){

	static unsigned char counter = 0;

	if(counter==speed){
		if(hsv.h < 200){
			hsv.h = hsv.h + 43;
		}
		else{
			hsv.h = 0;
		}

		hsv.s = 255;
		hsv.v = brightness;
		counter = 0;
	}
	else
		counter++;
}

// Mode 4: 3-Farben (direkt) schnell wechseln
void mode4(unsigned char speed, unsigned char brightness){

	static unsigned char counter = 0;

	if(counter==speed){
		if(hsv.h < 100){
			hsv.h = hsv.h + 85;
		}
		else{
			hsv.h = 0;
		}

		hsv.s = 255;
		hsv.v = brightness;
		counter = 0;
	}
	else
		counter++;
}

// Strobo
void mode7(unsigned char speed){

	static unsigned char counter;

	if(counter==speed){
		counter = 0;
		hsv.v=255;
		hsv.s=255;
	}
	else{
		counter++;
		hsv.v=0;
	}
}

int main(void){

	pwm_init();

	/* Endlosschleife
	 * die Unterfunktionen dürfen keine Schleifen enthalten!
	 *
	 */

	hsv.h=0;	// Farbe
	hsv.s=255;	// Sätigung
	hsv.v=255;	// Helligkeit

	while(1){

		_delay_ms(10);	//TODO: Durch Timer ersetzen.

		mode1(10,128);
//		mode3(100,255);
//		mode4(100,100);
//		mode7(10);
		pwm_set_hsv(hsv);
	}
}
