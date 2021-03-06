 /*      
 *----------------------------------------------------------------------------
 *      Name:    modLCD.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: Mateusz Gawron i Tomasz Cygan students at AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
#include "modLCD.h"
#include "spi.h"
#include "extra.h"

/*----------------------------------------------------------------------------
  Function that initializes modLCD
 *----------------------------------------------------------------------------*/
void modLCDInitialize(void)
{
	SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;  //Enable Clock for PORT E
	PORTE->PCR[2] |= PORT_PCR_MUX(1);  //RESET as GPIO
	PORTE->PCR[3] |= PORT_PCR_MUX(1);  //D/C as GPIO
	PTE->PDDR |= RESET_MASK | DC_MASK; //initialize GPIO as OUTPUT
	PTE->PSOR |= RESET_MASK | DC_MASK; // set GPIO to pull-up ss
	
	/* Initialize reset function */
	PTE->PCOR |= RESET_MASK; // set RESET to logic '0'
	delay_mc(10); // Reset last 10 ms
	PTE->PSOR |= RESET_MASK; // set RESET to logic '1'
	
	/*Initialize module*/
	sendComand(FUNCTION_SET_MASK | H_MASK); //  extended mode0x21
	sendComand(BIAS_MASK | 3); // mux 1:48 0x13
	sendComand(VOP_MASK | 0x36); // VOP 0xB6
	sendComand(FUNCTION_SET_MASK);//basicmode 0x20
	sendComand(DISPLAY_CONTROL_MASK | D_MASK); // normalmode
	clear();
	/*Preparations */ 
	print_CoordinateSystem();
	sendComand(X_MASK|2);
	
}

/*----------------------------------------------------------------------------
  Function to send Command to modLCD
 *----------------------------------------------------------------------------*/
void sendComand(uint8_t data)
{
	PTE->PCOR |= DC_MASK;
	spiWrite1(data);
}

/*----------------------------------------------------------------------------
  Function to write in DRAM memory
 *----------------------------------------------------------------------------*/
void sendData(uint8_t data)
{
	PTE->PSOR |= DC_MASK;
	spiWrite1(data);
}

/*----------------------------------------------------------------------------
  Function that clears screen
 *----------------------------------------------------------------------------*/
void clear()
{
	uint16_t i;
	for(i=0;i<X_SIZE*Y_SIZE;i++)
	{
		sendData(0x00);
	}
}

/*----------------------------------------------------------------------------
  Function that print coordinate system on modLCD 
 *----------------------------------------------------------------------------*/
void print_CoordinateSystem()
{
	uint8_t i,j;
	for (i=0;i<Y_SIZE;i++)
	{
		sendComand(Y_MASK|i);
		sendComand(X_MASK);
		sendData(0xFF);
		sendData(0x80);
		
		if(i==5)
		{
			for(j=2;j<X_SIZE;j++)
			{
				if((j+1)%8==0) sendData(0xC0);
				else sendData(0x80);
			}
		}
	}
}

/*----------------------------------------------------------------------------
  Function that print pixel corresponding value on coordinate system
 *----------------------------------------------------------------------------*/
void WhichPixel ( uint16_t value,float range,uint8_t* x)
{
	uint8_t bank=0;
  uint8_t pixel=0;
	uint8_t i;
	/*Sequence to choose right bank*/
	for(i=0; i<6; i++)
	{
		
		if(value < (range/6)*(i+1))
			{
				bank=i;
				break;
			}
	}
	/*Sequence to choose right pixel*/	
	for(i=0; i<8; i++)
	{
		if((value-bank*range/6 )< (range/48)*(i+1))
			{
				pixel = 7-i;
				bank = 5-bank;
				break;
			}
	}
	/*Sequence to print proper pixel on modLCD*/	
	sendComand(Y_MASK|bank);
	if(bank==5)
	{	
		if((x[0]+1)%8==0) sendData(1u<<pixel|0xC0);
		else sendData(1u<<pixel|0x80);
		
	}
	else
	sendData(1u<<pixel);	
	x[0]++; // increment column iterator
}
/*----------------------------------------------------------------------------
  Function that change position to X,Y on modLCD, where X is a column and Y is a bank
*----------------------------------------------------------------------------*/
void move_XY(uint8_t X,uint8_t Y)
{
	sendComand(X_MASK|X);
	sendComand(Y_MASK|Y);
	
}
/*----------------------------------------------------------------------------
  Table to decode char to lcd 5x8 font 
*----------------------------------------------------------------------------*/
const char font5x8[][5] = {
	 {0x00, 0x00, 0x00, 0x00, 0x00}	/* 0x20	  */
	,{0x00, 0x00, 0x5f, 0x00, 0x00}	/* 0x21	! */
	,{0x00, 0x07, 0x00, 0x07, 0x00}	/* 0x22	" */
	,{0x14, 0x7f, 0x14, 0x7f, 0x14}	/* 0x23	# */
	,{0x24, 0x2a, 0x7f, 0x2a, 0x12}	/* 0x24	$ */
	,{0x23, 0x13, 0x08, 0x64, 0x62}	/* 0x25	% */
	,{0x36, 0x49, 0x55, 0x22, 0x50}	/* 0x26	& */
	,{0x00, 0x05, 0x03, 0x00, 0x00}	/* 0x27	' */
	,{0x00, 0x1c, 0x22, 0x41, 0x00}	/* 0x28	( */
	,{0x00, 0x41, 0x22, 0x1c, 0x00}	/* 0x29	) */
	,{0x14, 0x08, 0x3e, 0x08, 0x14}	/* 0x2a	* */
	,{0x08, 0x08, 0x3e, 0x08, 0x08}	/* 0x2b	+ */
	,{0x00, 0x50, 0x30, 0x00, 0x00}	/* 0x2c	, */
	,{0x08, 0x08, 0x08, 0x08, 0x08}	/* 0x2d	- */
	,{0x00, 0x60, 0x60, 0x00, 0x00}	/* 0x2e	. */
	,{0x20, 0x10, 0x08, 0x04, 0x02}	/* 0x2f	/ */
	,{0x3e, 0x51, 0x49, 0x45, 0x3e}	/* 0x30	0 */
	,{0x00, 0x42, 0x7f, 0x40, 0x00}	/* 0x31	1 */
	,{0x42, 0x61, 0x51, 0x49, 0x46}	/* 0x32	2 */
	,{0x21, 0x41, 0x45, 0x4b, 0x31}	/* 0x33	3 */
	,{0x18, 0x14, 0x12, 0x7f, 0x10}	/* 0x34	4 */
	,{0x27, 0x45, 0x45, 0x45, 0x39}	/* 0x35	5 */
	,{0x3c, 0x4a, 0x49, 0x49, 0x30}	/* 0x36	6 */
	,{0x01, 0x71, 0x09, 0x05, 0x03}	/* 0x37	7 */
	,{0x36, 0x49, 0x49, 0x49, 0x36}	/* 0x38	8 */
	,{0x06, 0x49, 0x49, 0x29, 0x1e}	/* 0x39	9 */
	,{0x00, 0x36, 0x36, 0x00, 0x00}	/* 0x3a	: */
	,{0x00, 0x56, 0x36, 0x00, 0x00}	/* 0x3b	; */
	,{0x08, 0x14, 0x22, 0x41, 0x00}	/* 0x3c	< */
	,{0x14, 0x14, 0x14, 0x14, 0x14}	/* 0x3d	= */
	,{0x00, 0x41, 0x22, 0x14, 0x08}	/* 0x3e	> */
	,{0x02, 0x01, 0x51, 0x09, 0x06}	/* 0x3f	? */
	,{0x32, 0x49, 0x79, 0x41, 0x3e}	/* 0x40	@ */
	,{0x7e, 0x11, 0x11, 0x11, 0x7e}	/* 0x41	A */
	,{0x7f, 0x49, 0x49, 0x49, 0x36}	/* 0x42	B */
	,{0x3e, 0x41, 0x41, 0x41, 0x22}	/* 0x43	C */
	,{0x7f, 0x41, 0x41, 0x22, 0x1c}	/* 0x44	D */
	,{0x7f, 0x49, 0x49, 0x49, 0x41}	/* 0x45	E */
	,{0x7f, 0x09, 0x09, 0x09, 0x01}	/* 0x46	F */
	,{0x3e, 0x41, 0x49, 0x49, 0x7a}	/* 0x47	G */
	,{0x7f, 0x08, 0x08, 0x08, 0x7f}	/* 0x48	H */
	,{0x00, 0x41, 0x7f, 0x41, 0x00}	/* 0x49	I */
	,{0x20, 0x40, 0x41, 0x3f, 0x01}	/* 0x4a	J */
	,{0x7f, 0x08, 0x14, 0x22, 0x41}	/* 0x4b	K */
	,{0x7f, 0x40, 0x40, 0x40, 0x40}	/* 0x4c	L */
	,{0x7f, 0x02, 0x0c, 0x02, 0x7f}	/* 0x4d	M */
	,{0x7f, 0x04, 0x08, 0x10, 0x7f}	/* 0x4e	N */
	,{0x3e, 0x41, 0x41, 0x41, 0x3e}	/* 0x4f	O */
	,{0x7f, 0x09, 0x09, 0x09, 0x06}	/* 0x50	P */
	,{0x3e, 0x41, 0x51, 0x21, 0x5e}	/* 0x51	Q */
	,{0x7f, 0x09, 0x19, 0x29, 0x46}	/* 0x52	R */
	,{0x46, 0x49, 0x49, 0x49, 0x31}	/* 0x53	S */
	,{0x01, 0x01, 0x7f, 0x01, 0x01}	/* 0x54	T */
	,{0x3f, 0x40, 0x40, 0x40, 0x3f}	/* 0x55	U */
	,{0x1f, 0x20, 0x40, 0x20, 0x1f}	/* 0x56	V */
	,{0x3f, 0x40, 0x38, 0x40, 0x3f}	/* 0x57	W */
	,{0x63, 0x14, 0x08, 0x14, 0x63}	/* 0x58	X */
	,{0x07, 0x08, 0x70, 0x08, 0x07}	/* 0x59	Y */
	,{0x61, 0x51, 0x49, 0x45, 0x43}	/* 0x5a	Z */
	,{0x00, 0x7f, 0x41, 0x41, 0x00}	/* 0x5b	[ */
	,{0x02, 0x04, 0x08, 0x10, 0x20}	/* 0x5c	yuan symbol */
	,{0x00, 0x41, 0x41, 0x7f, 0x00}	/* 0x5d	] */
	,{0x04, 0x02, 0x01, 0x02, 0x04}	/* 0x5e	^ */
	,{0x40, 0x40, 0x40, 0x40, 0x40}	/* 0x5f	_ */
	,{0x00, 0x01, 0x02, 0x04, 0x00}	/* 0x60	` */
	,{0x20, 0x54, 0x54, 0x54, 0x78}	/* 0x61	a */
	,{0x7f, 0x48, 0x44, 0x44, 0x38}	/* 0x62	b */
	,{0x38, 0x44, 0x44, 0x44, 0x20}	/* 0x63	c */
	,{0x38, 0x44, 0x44, 0x48, 0x7f}	/* 0x64	d */
	,{0x38, 0x54, 0x54, 0x54, 0x18}	/* 0x65	e */
	,{0x08, 0x7e, 0x09, 0x01, 0x02}	/* 0x66	f */
	,{0x0c, 0x52, 0x52, 0x52, 0x3e}	/* 0x67	g */
	,{0x7f, 0x08, 0x04, 0x04, 0x78}	/* 0x68	h */
	,{0x00, 0x44, 0x7d, 0x40, 0x00}	/* 0x69	i */
	,{0x20, 0x40, 0x44, 0x3d, 0x00}	/* 0x6a	j  */
	,{0x7f, 0x10, 0x28, 0x44, 0x00}	/* 0x6b	k */
	,{0x00, 0x41, 0x7f, 0x40, 0x00}	/* 0x6c	l */
	,{0x7c, 0x04, 0x18, 0x04, 0x78}	/* 0x6d	m */
	,{0x7c, 0x08, 0x04, 0x04, 0x78}	/* 0x6e	n */
	,{0x38, 0x44, 0x44, 0x44, 0x38}	/* 0x6f	o */
	,{0x7c, 0x14, 0x14, 0x14, 0x08}	/* 0x70	p */
	,{0x08, 0x14, 0x14, 0x18, 0x7c}	/* 0x71	q */
	,{0x7c, 0x08, 0x04, 0x04, 0x08}	/* 0x72	r */
	,{0x48, 0x54, 0x54, 0x54, 0x20}	/* 0x73	s */
	,{0x04, 0x3f, 0x44, 0x40, 0x20}	/* 0x74	t */
	,{0x3c, 0x40, 0x40, 0x20, 0x7c}	/* 0x75	u */
	,{0x1c, 0x20, 0x40, 0x20, 0x1c}	/* 0x76	v */
	,{0x3c, 0x40, 0x30, 0x40, 0x3c}	/* 0x77	w */
	,{0x44, 0x28, 0x10, 0x28, 0x44}	/* 0x78	x */
	,{0x0c, 0x50, 0x50, 0x50, 0x3c}	/* 0x79	y */
	,{0x44, 0x64, 0x54, 0x4c, 0x44}	/* 0x7a	z */
	,{0x00, 0x08, 0x36, 0x41, 0x00}	/* 0x7b	{ */
	,{0x00, 0x00, 0x7f, 0x00, 0x00}	/* 0x7c	| */
	,{0x00, 0x41, 0x36, 0x08, 0x00}	/* 0x7d	} */
	,{0x10, 0x08, 0x08, 0x10, 0x08}	/* 0x7e	left arrow */
	,{0x78, 0x46, 0x41, 0x46, 0x78}	/* 0x7f	right arrow */
	};
/*----------------------------------------------------------------------------
  Function that print character on X,Y coordinates
  ---
	---
	---
	---	
	---
*----------------------------------------------------------------------------*/
void print_character(char x)
{
	uint8_t i;

	for(i=0;i<5;i++)
	{
		sendData(font5x8[x-0x20][i]);
	}
	sendData(0x00); // space between characters
}
/*----------------------------------------------------------------------------
  Function that print string,starting in X,Y position
*----------------------------------------------------------------------------*/
void print_string(char*str,uint8_t X,uint8_t Y)
{
	move_XY(X,Y);
	while(*str)
	{
		print_character(*str++);
	}
}
