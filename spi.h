 /*      
 *-------------------------------------------------------------------------------------------------
 *      Name:    spi.h
 *      Purpose: Microprocessors Laboratory
 *-------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/
 #ifndef spi_h
 #define spi_h
 
 #include "MKL46Z4.h"
 
 void spiInitialize(void);
 void spiWrite(uint8_t dataWrite[],uint8_t howmany);
 uint8_t spiRead(uint8_t adress);
 void spiWrite1(uint8_t dataWrite);
 
 #endif