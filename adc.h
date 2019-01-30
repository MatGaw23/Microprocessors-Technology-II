 /*      
 *------------------------------------------------------------------------------------------------
 *      Name:    adc.h
 *      Purpose: Microprocessors Laboratory
 *------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/
 #ifndef adc_h
 #define adc_h
 
 #include "MKL46Z4.h"
 #include "uart.h"
 #include "slcd.h"
 #include "modLCD.h"

 void adcInitialize(void);

 void ADC0_IRQHandler(void);
 #endif