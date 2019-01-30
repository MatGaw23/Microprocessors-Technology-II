/*--------------------------------------------------------------------------------------------------
 *      
 *--------------------------------------------------------------------------------------------------
 *      Name:    tpm.h
 *      Purpose: Microprocessors Laboratory
 *--------------------------------------------------------------------------------------------------     
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *-------------------------------------------------------------------------------------------------*/
#ifndef tpm_h
#define tpm_h

#include "MKL46Z4.h"   
#include "adc.h"

void tpmInitialize(void);
void TPM0_IRQHandler (void);


#endif

