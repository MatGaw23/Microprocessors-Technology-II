 /*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    uart.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
 #include "uart.h"
 
 /*---------------------------------------------------------------------------
  Function that initialize UART0 module
 *----------------------------------------------------------------------------*/
 void uartInitialize(void)
 {
	 SIM->SCGC4|= SIM_SCGC4_UART0_MASK; // Enable Clock for UART0 module
	 SIM->SCGC5|= SIM_SCGC5_PORTA_MASK; // Enable Clock for PORT A
	 PORTA->PCR[1]|= PORT_PCR_MUX(2);  // SET PORT A1 as GPIO
	 PORTA->PCR[2]|= PORT_PCR_MUX(2);  // SET PORT A2 as GPIO
	 
	 SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2); // Set 8MHZ Clock for UART0
	 
	 UART0->C2 &= ~(UART0_C2_RE_MASK | UART0_C2_TE_MASK); // Disable transmitter and receiver
	 UART0->C4 |= UART0_C4_OSR(31);//Over Sampling Ratio
	 
	 UART0->BDH = 0x00;
	 UART0->BDL = 0x19; // Boundrate set at 10000 
	 
	 UART0->BDH &= ~UART0_BDH_SBNS_MASK; // 1 bit stop
	 UART0->C1 &= ~(UART0_C1_M_MASK|UART0_C1_PE_MASK);//no parrity control | 8 bit data
	 
	 UART0->C2 |= UART0_C2_RE_MASK | UART0_C2_TE_MASK;//Enable transmitter and receiver
 } 
  /*----------------------------------------------------------------------------
  Function that handle Transmition data to PC 
 *----------------------------------------------------------------------------*/
void uarttransmit(int data)
{ 
	uint8_t i=0;
	int first = data/1000;
	int second = (data-first*1000)/100;
	int third = (data-first*1000-second*100)/10;
	int fourth = data-first*1000-second*100-third*10;
	char a='0'+first;
	char b='0'+second;
	char c='0'+third;
	char d='0'+fourth;
	for (i=0;i<7;i++){
	 
	  if(UART0->S1&UART0_S1_TDRE_MASK)
		{
		 switch(i)
		 {
			 case 0:UART0->D = a;break;
			 case 1:UART0->D = b;break;
			 case 2:UART0->D = c;break;
			 case 3:UART0->D = 0x2E;break;
			 case 4:UART0->D = d;break;
			 case 5:UART0->D = 0x0A; break;
			 case 6:UART0->D = 0x0D; break;
			 default: i=0; 
		 }
		}
		delay_mc(10);
	}
}