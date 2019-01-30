/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    leds.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "leds.h"												//Declarations

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void ledsInitialize(void){
}
/*----------------------------------------------------------------------------
  Function that blinks (x) times green led with (y) delay in mc
 *----------------------------------------------------------------------------*/
void ledgreenBlink(uint32_t x, uint32_t y){
	uint32_t count;
	
	for(count = 0; count < x; count++){	
		PTD->PCOR = led_mask[ledGreen];
		delay_mc(y);		
		PTD->PSOR = led_mask[ledGreen];
		delay_mc(y);
	}
}
/*----------------------------------------------------------------------------
  Function that blinks (x) times red led with (y) delay in mc
 *----------------------------------------------------------------------------*/
void ledredBlink(uint32_t x, uint32_t y){
	uint32_t count;
	
	for(count = 0; count < x; count++){	
		PTE->PCOR = led_mask[ledRed];
		delay_mc(y);		
		PTE->PSOR = led_mask[ledRed];
		delay_mc(y);	
	}
}
/*----------------------------------------------------------------------------
  Function that turns all LEDs off
 *----------------------------------------------------------------------------*/
void ledsOff(void) {
		PTD->PSOR = led_mask[ledGreen];          /* switch Red LED off  */
	  PTE->PSOR = led_mask[ledRed];       /* switch Green LED off  */
}
/*----------------------------------------------------------------------------
  Function that turns all LEDs on
 *----------------------------------------------------------------------------*/
void ledsOn(void) {
		PTD->PCOR = led_mask[ledGreen];      	/* switch Red LED on  */
	  PTE->PCOR = led_mask[ledRed];     	/* switch Green LED on */
}
