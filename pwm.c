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

#include "PWM.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

// falls unterschiedliche Tabllen gebraucht werden
#define PWM_TABLE_RED	pwm_table
#define PWM_TABLE_GREEN	pwm_table
#define PWM_TABLE_BLUE	pwm_table


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



extern void pwm_set ( unsigned int red, unsigned int green, unsigned int blue, unsigned char brightness)
{

	// errechete logaritmische Werte (damit die Ver�nderung linear wirkt)
	// im Flash-Speicher gespeichert
	static unsigned char pwm_table [101] PROGMEM = {0,0,5,6,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,11,11,12,12,12,13,13,14,15,
													15,16,16,17,18,19,19,20,21,22,23,24,24,25,27,28,29,30,31,32,34,35,36,
													38,39,41,43,44,46,48,50,52,54,56,59,61,64,66,69,72,74,77,81,84,87,91,
													94,98,102,106,111,115,120,125,130,135,141,146,152,158,165,171,178,186,
													193,201,209,218,226,236,245,255,255};

	// brightness einrechnen
	red 	= brightness * red 	 / 100;
	green 	= brightness * green / 100;
	blue	= brightness * blue  / 100;


	RED_PWM = pgm_read_byte(&PWM_TABLE_RED[red]);
	GREEN_PWM = pgm_read_byte(&PWM_TABLE_GREEN[green]);
	BLUE_PWM = pgm_read_byte(&PWM_TABLE_BLUE[blue]);

	if (RED_PWM) RED_DDR |= (1<<RED_PIN);
	else RED_DDR &= ~(1<<RED_PIN);

	if (GREEN_PWM) GREEN_DDR |= (1<<GREEN_PIN);
	else GREEN_DDR &= ~(1<<GREEN_PIN);

	if(BLUE_PWM) BLUE_DDR |= (1<<BLUE_PIN);
	else BLUE_DDR &= ~(1<<BLUE_PIN);


}
