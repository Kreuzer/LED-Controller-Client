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

#include "PWM.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

// falls unterschiedliche Tabllen gebraucht werden
#define PWM_TABLE_RED	pwm_table
#define PWM_TABLE_GREEN	pwm_table
#define PWM_TABLE_BLUE	pwm_table

// errechete logaritmische Werte (damit die Ver�nderung linear wirkt)
// im Flash-Speicher gespeichert
const unsigned char pwm_table [101] PROGMEM = {0,0,5,6,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,11,11,12,12,12,13,13,14,15,
												15,16,16,17,18,19,19,20,21,22,23,24,24,25,27,28,29,30,31,32,34,35,36,
												38,39,41,43,44,46,48,50,52,54,56,59,61,64,66,69,72,74,77,81,84,87,91,
												94,98,102,106,111,115,120,125,130,135,141,146,152,158,165,171,178,186,
												193,201,209,218,226,236,245,255,255};


// gamma correction
const unsigned char led_gamma[256] PROGMEM = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,
											5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,10,10,11,11,11,11,
											12,12,12,13,13,13,13,14,14,14,15,15,15,15,16,16,16,17,17,17,18,18,18,19,19,20,20,20,
											21,21,22,22,22,23,23,24,24,25,25,26,26,26,27,27,28,28,29,30,30,31,31,32,32,33,33,34,
											35,35,36,37,37,38,38,39,40,41,41,42,43,43,44,45,46,46,47,48,49,50,51,51,52,53,54,55,
											56,57,58,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,86,87,
											89,90,92,93,95,96,98,99,101,102,104,106,107,109,111,113,115,116,118,120,122,124,126,
											128,130,132,134,137,139,141,143,145,148,150,153,155,157,160,162,165,168,170,173,176,
											179,181,184,187,190,193,196,199,202,206,209,212,216,219,222,226,229,233,237,241,244,
											248,252,255};


extern void pwm_init(void)
{
	// PWM - Einstellungen:
    // Fast PWM 8 Bit | nicht invertierendes PWM gesetzt
    TCCR1A |=  (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);
	// Fast PWM 8 Bit | prescaler 1/256 (ca. 122Hz PWM Frequenz bei 8MHz CPU-Takt)
	TCCR1B |= (1<<WGM12) | (1<<CS12);
	// timer2: 8 bit PWM (max)
#if (__AVR_ATmega8__) || (__AVR_ATmega16__) || (__AVR_ATmega32__)
	TCCR2 |= (1<<WGM21) | (1<<WGM20) | (1<<COM21) | (1<<CS21) | (1<<CS22); // Fast-PWM | non-invert-PWM | Prescaler
#elif (__AVR_ATmega48__) || (__AVR_ATmega88__) || (__AVR_ATmega168__)
	TCCR2A |= (1<<WGM21) | (1<<WGM20) | (1<<COM2A1);
	TCCR2B |= (1<<CS21) | (1<<CS22);
#endif
	// PWM Ports auf Ausgang
	RED_DDR |= (1<<RED_PIN);
	GREEN_DDR |= (1<<GREEN_PIN);
	BLUE_DDR |= (1<<BLUE_PIN);
}


struct rgb hsv_to_rgb (struct hsv hsv)
{
	unsigned char r=0, g=0, b=0, i=0, f = 0;
	unsigned int p, q, t;
	struct rgb rgb;

	if( hsv.s == 0 )
 	{	r = g = b = hsv.v;
	}
	else
	{	i=hsv.h/43;
		f=hsv.h%43;
		p = (hsv.v * (255 - hsv.s))/256;
		q = (hsv.v * ((10710 - (hsv.s * f))/42))/256;
		t = (hsv.v * ((10710 - (hsv.s * (42 - f)))/42))/256;

		switch( i )
		{	case 0:
				r = hsv.v; g = t; b = p; break;
			case 1:
				r = q; g = hsv.v; b = p; break;
			case 2:
				r = p; g = hsv.v; b = t; break;
			case 3:
				r = p; g = q; b = hsv.v; break;
			case 4:
				r = t; g = p; b = hsv.v; break;
			case 5:
	 			r = hsv.v; g = p; b = q; break;
		}
	}
	rgb.red = r;
	rgb.green = g;
	rgb.blue = b;
	return rgb;
}


extern void pwm_set_rgb_brightness ( struct rgb rgb, unsigned char brightness)
{
	// Helligkeit einrechnen
	rgb.red 	= brightness * rgb.red 	 / 100;
	rgb.green 	= brightness * rgb.green / 100;
	rgb.blue	= brightness * rgb.blue  / 100;

	// Logarthmische werte aus dem Speicher holen
	RED_PWM = pgm_read_byte(&PWM_TABLE_RED[rgb.red]);
	GREEN_PWM = pgm_read_byte(&PWM_TABLE_GREEN[rgb.green]);
	BLUE_PWM = pgm_read_byte(&PWM_TABLE_BLUE[rgb.blue]);


	// Wenn LED-Ausgeschaltet sein soll, Pin auf Eingang, sonst leuchtet sie minimal.
	if (RED_PWM) RED_DDR |= (1<<RED_PIN);
	else RED_DDR &= ~(1<<RED_PIN);

	if (GREEN_PWM) GREEN_DDR |= (1<<GREEN_PIN);
	else GREEN_DDR &= ~(1<<GREEN_PIN);

	if(BLUE_PWM) BLUE_DDR |= (1<<BLUE_PIN);
	else BLUE_DDR &= ~(1<<BLUE_PIN);


}

extern void pwm_set_rgb ( struct rgb rgb){
	RED_PWM = pgm_read_byte(&led_gamma[rgb.red]);
	GREEN_PWM = pgm_read_byte(&led_gamma[rgb.green]);
	BLUE_PWM = pgm_read_byte(&led_gamma[rgb.blue]);

	RED_PWM = rgb.red;
	GREEN_PWM = rgb.green;
	BLUE_PWM = rgb.blue;

	// Wenn LED-Ausgeschaltet sein soll, Pin auf Eingang, sonst leuchtet sie minimal.
	if (RED_PWM) RED_DDR |= (1<<RED_PIN);
	else RED_DDR &= ~(1<<RED_PIN);

	if (GREEN_PWM) GREEN_DDR |= (1<<GREEN_PIN);
	else GREEN_DDR &= ~(1<<GREEN_PIN);

	if(BLUE_PWM) BLUE_DDR |= (1<<BLUE_PIN);
	else BLUE_DDR &= ~(1<<BLUE_PIN);
}


extern void pwm_set_hsv( struct hsv hsv){
	pwm_set_rgb(hsv_to_rgb(hsv));
}

