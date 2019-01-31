/*---------------------------------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------------------------------
 *      Name:    main.c
 *      Purpose: Microprocessors Laboratory
 *---------------------------------------------------------------------------------------------------
 *      This project is made for Microprocessor Technology project purpose
 *
 *      Author: Mateusz Gawron and Tomasz Cygan students at AGH University of Science and Technology
 *---------------------------------------------------------------------------------------------------*/
#include "MKL46Z4.h"
#include "tpm.h"
#include "adc.h"
#include "slcd.h"
#include "leds.h"
#include "extra.h"
#include "uart.h"
#include "spi.h"
#include "modLCD.h"
/*----------------------------------------------------------------------------
      Application main procedure 
 *---------------------------------------------------------------------------*/
int main(void)
{
	tpmInitialize();
	slcdInitialize();
	adcInitialize();
	uartInitialize();
	spiInitialize();
	modLCDInitialize();
	//print_string(s,4,2);

	while(1)
	{	
		

	}
}
