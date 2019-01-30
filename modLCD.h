 /*      
 *-------------------------------------------------------------------------------------------------
 *      Name:    modLCD.h
 *      Purpose: Microprocessors Laboratory
 *-------------------------------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *------------------------------------------------------------------------------------------------*/
 #ifndef modLCD_h
 #define modLCD_h
 
 #include "MKL46Z4.h"
 /*DEFINITION OF GPIO MASKS */
 #define RESET_MASK (1u<<2) 
 #define DC_MASK (1u<<3)
 /*DEFINITION OF COLUMNS AND BANKS SIZE*/
 #define X_SIZE 84
 #define Y_SIZE 6
 /*DEFINITION OF REGISTER MASKS*/
 #define Y_MASK (1u<<6)
 #define X_MASK (1u<<7)
 #define H_MASK 1u
 #define V_MASK (1u<<1)
 #define FUNCTION_SET_MASK (1u<<5)
 #define DISPLAY_CONTROL_MASK (1u<<3)
 #define D_MASK (1u<<2)
 #define E_MASK 1u
 #define VOP_MASK (1u<<7)
 #define BIAS_MASK (1u<<4)
 

	void modLCDInitialize(void);
	void sendComand(uint8_t data);
	void sendData(uint8_t data);
	void clear();
	void print_CoordinateSystem();
	void WhichPixel(uint16_t value,float range,uint8_t* x);
	void move_XY(uint8_t X,uint8_t Y);
	void print_character(char x);
	void print_string(char*str,uint8_t X,uint8_t Y);


	#endif



