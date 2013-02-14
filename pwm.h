/******************************************************************************
 Title:    Pulse Wide Modulation (PWM) for RGB LEDs
 Author:   Frederick Brand
 Date:     8/2009
 Software: WinAVR 20090313
 Hardware: ATmega8 mit 8 Mhz (intern)

 Description:
 Generate  Pulse Width Modulation (PWM) for RGB-Control.

 RED to OCR1A
 GREEN to OCR1B
 BLUE to OC(R)2


*******************************************************************************/

#ifndef PWM_H
#define PWM_H

#include <avr/pgmspace.h>			// Für den zugriff auf den Flashspeicher



#if __AVR_ATmega8__
	// Ports festlegen:
	#define RED_DDR	 	DDRB        // OC1A DDR
	#define GREEN_DDR 	DDRB        // OC1B DDR
	#define BLUE_DDR	DDRB		// OC2  DDR
	#define RED_PIN 	PB1			// OC1A pin
	#define GREEN_PIN 	PB2         // OC1B pin
	#define BLUE_PIN  	PB3			// OC2  pin
	// OCn-Register festlegen:
	#define RED_PWM 	OCR1A
	#define GREEN_PWM 	OCR1B
	#define BLUE_PWM 	OCR2

#elif (__AVR_ATmega16__) || (__AVR_ATmega32__)
	// Ports festlegen:
	#define RED_DDR		DDRD        // OC1A DDR
	#define GREEN_DDR 	DDRD        // OC1B DDR
	#define BLUE_DDR	DDRD		// OC2  DDR
	#define RED_PIN 	PD5         // OC1A pin
	#define GREEN_PIN 	PD4         // OC1B pin
	#define BLUE_PIN  	PD7			// OC2  pin
	// OCn-Register festlegen:
	#define RED_PWM 	OCR1A
	#define GREEN_PWM 	OCR1B
	#define BLUE_PWM 	OCR2
#endif


// Inizialisiert die Timer1 und 2 auf: - 8bit Fast-PWM nicht invertierend ca. 1kHz

extern void pwm_init(void);



// setzt die Farben auf den angegebenen Prozentwert, zusätzlich wird die Helligkeit mit eingerechnet.
// alle angaben in Prozent

extern void set_pwm ( unsigned int red, unsigned int green, unsigned int blue, unsigned char brigtness);


#endif
