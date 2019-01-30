 /*      
 *------------------------------------------------------------------------------------------------
 *      Name:    uart.h
 *      Purpose: Microprocessors Laboratory
 *------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/
 #ifndef uart_h
 #define uart_h
 
 #include "MKL46Z4.h"
 #include "extra.h"
 void uartInitialize(void);
 void uarttransmit(int data);
 
 #endif