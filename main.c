/*
 ================================================================================================
 Name        : main.c
 Author      : Mohamed Yahia
 Description : Measure Distance by Ultrasonic and display it on LCD
 Date        : 10/13/2021
 ================================================================================================
 */

#include "lcd.h"
#include "gpio.h"
#include "icu.h"
#include "ultrasonic.h"
#include <avr/io.h> /* To use the SREG register */
#include "util/delay.h"
#include "std_types.h"


int main()
{
	/* stores the returned distance */
	uint16 d;

	/* Enable Global Interrupt I-Bit */
	SREG |= (1 << 7);

	/* Initialize Ultrasonic driver */
	Ultrasonic_init();

	/* Initialize the LCD */
	LCD_init();

	/* Print on LCD */
	LCD_displayString("Distance=     cm");

	while (1)
	{
		LCD_moveCursor(0,10);
		d = Ultrasonic_readDistance();

	if (d > 0 && d < 100)
		{
			LCD_intgerToString(d+1);
			LCD_displayCharacter(' ');
		}

	else if (d >= 100 )
		{
			LCD_intgerToString(d + 2);
		}

	else if (d >= 200 && d < 300)
		{
			LCD_intgerToString(d+3);
		}

	else if (d >= 300 && d < 400)
		{
			LCD_intgerToString(d + 3);
		}


	}
}
