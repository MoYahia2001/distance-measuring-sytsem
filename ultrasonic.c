 /******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the AVR GPIO driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h"        /* To use the IO Ports Registers */
#include "util/delay.h"

volatile int distance;
volatile int g_edgeCount;
volatile int g_timeHigh;

/*
 *  Initialize the ICU driver as required.
 *  Setup the ICU call back function.
 *  Setup the direction for the trigger pin as output pin through the GPIO driver.
 */

void Ultrasonic_init(void)
{
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};
	Icu_init(&Icu_Config);
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(Ultra_Trigger_PORT_ID ,Ultra_Trigger_PIN_ID, PIN_OUTPUT);
}
/*
 *  Send the Trigger pulse to the ultrasonic.
 */

void Ultrasonic_Trigger(void)
{
	GPIO_writePin(Ultra_Trigger_PORT_ID , Ultra_Trigger_PIN_ID, LOGIC_HIGH);
	_delay_us(20);
	GPIO_writePin(Ultra_Trigger_PORT_ID , Ultra_Trigger_PIN_ID, LOGIC_LOW);
}

/*
 *   Send the trigger pulse by using Ultrasonic_Trigger function.
 *   Start the measurements by the ICU from this moment.
 */

uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();

    distance = ( g_timeHigh ) / 58.8 ;

	return distance;
}


/*
 *   This is the call back function called by the ICU driver.
 *   This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */

void Ultrasonic_edgeProcessing(void)
{

	g_edgeCount++;
		if(g_edgeCount == 1)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */
			Icu_clearTimerValue();
			/* Detect falling edge */
			Icu_setEdgeDetectionType(FALLING);
		}
		else if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = Icu_getInputCaptureValue();
			Icu_setEdgeDetectionType(RISING); /* To start detecting rising edge again */
			g_edgeCount=0;
		}

}

