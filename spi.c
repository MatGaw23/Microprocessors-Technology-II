 /*      
 *----------------------------------------------------------------------------
 *      Name:    spi.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
 #include "spi.h"
 /*----------------------------------------------------------------------------
  Function that initialize SPI0 module
 *----------------------------------------------------------------------------*/
 void spiInitialize(void)
 {
	 SIM->SCGC5|=SIM_SCGC5_PORTE_MASK; // Enable Clock for PORT E
	 PORTE->PCR[16] |= PORT_PCR_MUX(1); //ss 
	 PORTE->PCR[17] |= PORT_PCR_MUX(2);	// clk
	 PORTE->PCR[18] |= PORT_PCR_MUX(2); //mosi
	 PORTE->PCR[19] |= PORT_PCR_MUX(2); //miso
	  
	 PTE->PDDR |= (1<<16); //initialize GPIO as OUTPUT
	 PTE->PSOR |= (1<<16); // set GPIO to pull-up ss
	 
	 SIM->SCGC4|=SIM_SCGC4_SPI0_MASK; // enable Clock for SPI0 module
	 SPI0->C1 = 0;
	 SPI0->C1 |= SPI_C1_SPE_MASK| SPI_C1_MSTR_MASK  ;
								// ENABLE SPI | SET AS MASTER 
	 SPI0->C1 &= ~(SPI_C1_CPOL_MASK|SPI_C1_SPIE_MASK|SPI_C1_CPHA_MASK);// CLOCK SETTING (MODE 0 0 )| INTERRUPT DISABLE- USING POLLING
	 SPI0->C2 &= ~(SPI_C2_SPMIE_MASK|SPI_C2_SPIMODE_MASK ); // 8 BIT | USING POLLING
	 
	 SPI0->BR = 34; // SET BOUNDRATE AT 1MHZ
		
	 
 }
  /*----------------------------------------------------------------------------
  Function that send a N byte frame 8 bit data each
 *----------------------------------------------------------------------------*/
 void spiWrite1(uint8_t dataWrite)
 {
	 
	 uint8_t dummy=0; // variable to read junky data
	 PTE->PCOR |= (1<<16); // SS set to low ;

		 while((SPI0->S & SPI_S_SPTEF_MASK)!=SPI_S_SPTEF_MASK); // wait to spi buffer is empty
			
		 SPI0->DL = dataWrite; // send data

		 while((SPI0->S & SPI_S_SPRF_MASK)!=SPI_S_SPRF_MASK); 
			
		 dummy=SPI0->DL; 	
	 PTE->PSOR |= (1<<16); //SS set to high;
 }
 /*----------------------------------------------------------------------------
  Function that send a N byte frame 8 bit data each
 *----------------------------------------------------------------------------*/
 void spiWrite(uint8_t dataWrite[],uint8_t N)
 {
	 uint8_t i; // initialize iterator
	 
	 uint8_t dummy=0; // variable to read junky data
	 PTE->PCOR |= (1<<16); // SS set to low ;
	 for(i=0;i<N;i++)
	 {
		 while((SPI0->S & SPI_S_SPTEF_MASK)!=SPI_S_SPTEF_MASK); // wait to spi buffer is empty
			
		 SPI0->DL = dataWrite[i]; // send data

		 while((SPI0->S & SPI_S_SPRF_MASK)!=SPI_S_SPRF_MASK); 
			
		 dummy=SPI0->DL; 	
	 }
	 PTE->PSOR |= (1<<16); //SS set to high;
 }
  /*----------------------------------------------------------------------------
  Function that read 8 bit frame 
 *----------------------------------------------------------------------------*/
uint8_t spiRead(uint8_t adress)
 {
	//  uint8_t i; // initialize iterator
	  uint8_t dummy=0; // variable to read junky data
	  PTE->PCOR |= (1<<16); // SS set to low ;

		while((SPI0->S & SPI_S_SPTEF_MASK)!=SPI_S_SPTEF_MASK); // wait to spi buffer is empty
			
		SPI0->DL = adress; // send data

		while((SPI0->S & SPI_S_SPRF_MASK)!=SPI_S_SPRF_MASK); 
			
		dummy=SPI0->DL; 	
		while((SPI0->S & SPI_S_SPTEF_MASK)!=SPI_S_SPTEF_MASK); // wait to spi buffer is empty
			
		SPI0->DL = 0x00; // send data

	  while((SPI0->S & SPI_S_SPRF_MASK)!=SPI_S_SPRF_MASK); 
		PTE->PSOR |= (1<<16); //SS set to high;
	 	return SPI0->DL;
	 
	  

 }	 
