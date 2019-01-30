/*---------------------------------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------------------------------
 *      Name:    main.c
 *      Purpose: Microprocessors Laboratory
 *---------------------------------------------------------------------------------------------------
 *      
 *       Author: Mateusz Gawron and Tomasz Cygan students at AGH University of Science and Technology
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
//#include "pmodRF2.h"
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
	//pmodInitialize();
	//char* s="Kocham SMPII!";
	//print_string(s,4,2);

	while(1)
	{	
		

	}
}
