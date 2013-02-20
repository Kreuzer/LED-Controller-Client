/*
 * main.c
 *
 *  Created on: 14.02.2013
 *      Author: Frederick
 */

#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

int main(void){

	pwm_init();

	/* Endlosschleife
	 * die Unterfunktionen dürfen keine Schleifen enthalten!
	 *
	 */

		unsigned char red = 0;
		unsigned char green = 0;
		unsigned char blue = 0;
		unsigned char step = 1;
		unsigned char brightness = 100;

	while(1){



		_delay_ms(10);


		switch (step)
				{
					case 1:
						if (++green < 100) pwm_set(red, green, blue, brightness);
						else step++;
						break;
					case 2:
						if (--red > 0) pwm_set(red, green, blue, brightness);
						else step++;
						break;
					case 3:
						if (++blue < 100) pwm_set(red, green, blue, brightness);
						else step++;
						break;
					case 4:
						if (--green > 0) pwm_set(red, green, blue, brightness);
						else step++;
						break;
					case 5:
						if (++red < 100) pwm_set(red, green, blue, brightness);
						else step++;
						break;
					case 6:
						if (--blue > 0) pwm_set(red, green, blue, brightness);
						else step = 1;
						break;
				} // switch (step)
	}
}
