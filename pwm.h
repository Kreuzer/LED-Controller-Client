/******************************************************************************
 Title:    Pulse Wide Modulation (PWM) for RGB LEDs
 Author:   Frederick
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

#include <avr/pgmspace.h>			// Für den Zugriff auf den Flashspeicher


struct hsv{
	unsigned char h;
	unsigned char s;
	unsigned char v;
};

struct rgb{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/*
H:          der Farbton als Farbwinkel H auf dem Farbkreis (z. B. 0° = Rot, 120° = Grün, 240° = Blau)
S:          die Sättigung S in Prozent (z. B. 0% = keine Farbe, 50% = ungesättigte Farbe, 100% = gesättigte, reine Farbe)
V:          der Grauwert V als Prozentwert angegeben (z. B. 0% = keine Helligkeit, 100% = volle Helligkeit)

Skalierung der HSV Werte:
H:			0-255, 0=rot, 42=gelb, 85=grün, 128=türkis, 171=blau, 214=violett
S:			0-255, 0=weißtöne, 255=volle Farben
V:			0-255, 0=aus, 255=maximale Helligkeit

*/


/** initialisiert die PWM-Hardware
 *
 * 	Inizialisiert die Timer1 und 2 auf: - 8bit Fast-PWM nicht invertierend ca. 1kHz
 */
extern void pwm_init(void);


/** setzt neue Ausgabe Werte (Helligkeit)
 *
 * @param struct rgb
 * @param brightness - Helligkeit in %
 */
extern void pwm_set_rgb_brightness ( struct rgb, unsigned char brightness);


/** setzt neue Ausgabe Werte
 * 	@param struct rgb
 */
extern void pwm_set_rgb ( struct rgb);


/** setzt neue Ausgabe Werte
 * 	@param struct hsv
 */
extern void pwm_set_hsv ( struct hsv);




// Hardware config
#if (__AVR_ATmega8__)
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

#elif (__AVR_ATmega48__) || (__AVR_ATmega88__) || (__AVR_ATmega168__)
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
	#define BLUE_PWM 	OCR2A

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

#endif
