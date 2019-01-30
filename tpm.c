/*-------------------------------------------------------------------------------------------------
 *      
 *-------------------------------------------------------------------------------------------------
 *      Name:    tpm.c
 *      Purpose: Microprocessors Laboratory
 *-------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/
 
 #include "tpm.h"
 
 #define TPM_NBR (IRQn_Type) TPM0_IRQn
 /*----------------------------------------------------------------------------
  Function that initialize TPM0 module 
							PWM 0.2% 
 *----------------------------------------------------------------------------*/
 
void tpmInitialize(void)
 {		
	 
	 SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;// enable Clock for PORT D
	 PORTD->PCR[2] |= PORT_PCR_MUX(4);//enable porta pin2 as TPM0 Channel2 output
	 
	 SIM->SCGC6|=SIM_SCGC6_TPM0_MASK; // enable Clock for TPM0 module
	 SIM->SOPT2|=SIM_SOPT2_TPMSRC(2); // MCGFLLCLK clock or MCGPLLCLK/2
	  
	 TPM0->SC &= ~TPM_SC_CPWMS_MASK; // TPM counter operates in up counting mode.
	 TPM0->SC |= TPM_SC_PS(7)|TPM_SC_TOIE_MASK;// Prescaler 128 = 4MHZ/128 =31.25 kHz
	 
	 TPM0->MOD =0xF424; // TPM period is 1s

	 TPM0->CONTROLS[2].CnSC&=~TPM_CnSC_ELSA_MASK;
	 TPM0->CONTROLS[2].CnSC&=~TPM_CnSC_MSA_MASK;
	 TPM0->CONTROLS[2].CnSC|=TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK; // Edge-aligned PWM , High-true pulses (clear Output on match, set Output on reload)
	 TPM0->CONTROLS[2].CnV= 0x007D; // TPM HIGH for 1ms
	 TPM0->SC |= TPM_SC_CMOD(1);//Counting on clocks rising edge

	 NVIC_ClearPendingIRQ(TPM_NBR);	// Clear NVIC any pending interrupts 
	 NVIC_EnableIRQ(TPM_NBR);				// Enable NVIC Interrupts


 }
 
 /*----------------------------------------------------------------------------
  Function that handle TPM0 Interrupts 
 *----------------------------------------------------------------------------*/
 
 void TPM0_IRQHandler (void)
 {
	 TPM0->SC |= TPM_SC_TOF_MASK;    // Clear TPM Overflow Flag 
	 ADC0->SC1[0] = ADC_SC1_ADCH(8); // Chosen channel, begin measurement
   ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;//enable interrupt from ADC;
}
 
