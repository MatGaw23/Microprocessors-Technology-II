 /*      
 *-------------------------------------------------------------------------------------------------
 *      Name:    adc.c
 *      Purpose: Microprocessors Laboratory
 *-------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/

 #include "adc.h"
 uint8_t x_position=2;
 #define ADC_NBR (IRQn_Type) ADC0_IRQn
  /*----------------------------------------------------------------------------
  Function that initialize ADC0 module  
 *----------------------------------------------------------------------------*/
 void adcInitialize(void)
 {
	 SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; 
	 SIM->SCGC5 |=SIM_SCGC5_PORTE_MASK|SIM_SCGC5_PORTB_MASK;
	 PORTE->PCR[22] |= PORT_PCR_MUX(1); 
	 PORTB->PCR[0] |= PORT_PCR_MUX(0); // ptc1 wejscie dla adc
	 ADC0->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(2) | ADC_CFG1_ADLSMP_MASK;// divide bus clock /2 = 12 MHz | divide clock 12/4=3MHz | long sample time
	 ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS(1);//High-Speed conversion sequence selected with 2 additional ADCK cycles to total conversion
																													// 12 extra ADCK cycles;16 ADCK cycles total sample time
	 //SC2 REFSEL 00 default volatage reference pin pair is, thatis, external pins vreh and vrefl
	 ADC0->SC3 |=ADC_SC3_AVGE_MASK |ADC_SC3_AVGS(3) | ADC_SC3_CAL_MASK;// hardware averaging | enable 32 samples averaged | START CALIBRATION
	 
	 calres:
	 while(ADC0->SC3 & ADC_SC3_CAL_MASK); //wait until end of calibration
	 if(ADC0->SC3 & ADC_SC3_CALF_MASK)   	//check calibration status (Successful or not) 
	 {
		 goto calres;
	 }
	 ADC0->PG = ((ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPD + ADC0->CLPS)>>1ul )| 0x8000; 
		
	 ADC0->MG = ((ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMD + ADC0->CLMS)>>1ul )| 0x8000;
	 
	 ADC0->CFG1 = ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(1);
		// divide bus clock /2 = 12 MHz|divide clock 12/1=12MHz |long sample time|single-ended 12-bit conversion 
	 
	 ADC0->SC1[0] |= ADC_SC1_AIEN_MASK ;   // Enable ADC0 interrupts
	 NVIC_ClearPendingIRQ(ADC_NBR);				/* Clear NVIC any pending interrupts from ADC0 */
	 NVIC_EnableIRQ(ADC_NBR);							/* Enable NVIC interrupts from ADC0*/
	 
 }
  /*----------------------------------------------------------------------------
  Function that handle ADC0 Interrupts
 *----------------------------------------------------------------------------*/ 
 void ADC0_IRQHandler(void)
{
	float voltage=0;
	float res =0;
	
	voltage=(ADC0->R[0]+0.5)*3.3/4096; // calculate value from ADC0 to voltage
	slcdDisplay(voltage*1000,10);			 // display voltage on LCD
	slcdSetDot(1);									
	
	res=(2.86/voltage - 1)*100;// calculate a resistance , formula from TGS 8100 datasheet
	uarttransmit(res*10);// send resistance value via UART
	WhichPixel(res,600,&x_position);// display resistance on modLCD1
	 //RESET SCREEN ON LAST PIXEL
	if(x_position==X_SIZE)
	{
		clear();
		print_CoordinateSystem();
		sendComand(X_MASK|2);
		x_position=2;
	}
}

