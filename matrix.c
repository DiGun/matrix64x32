/*
 * CFile1.c
 *
 * Created: 02.07.2017 10:46:48
 *  Author: DiGun
 */ 
#include "matrix.h"


void mx_init(void)
{	
	DDRD|=(1<<LA) |(1<<LB) |(1<<LC) |(1<<LD)|(1<<LT)|(1<<SK);
	DDRC|=(1<<R1) |(1<<R2) |(1<<G1) |(1<<G2)|(1<<OE);
	PORTD|=(1<<LA) |(1<<LB) |(1<<LC) |(1<<LD);
	PORTC|=(1<<OE);
	
}

inline void mx_enable()
{
	PORTC &= ~(1<<OE);
}

inline void mx_disable()
{
	PORTC |= (1<<OE);
}

void mx_row(uint8_t r)
{
	PORTD&=0b11000011;
	r=r<<2;
    PORTD|=(r&0b00111100);
}

void mx_draw_row(uint8_t r)
{
}

void mx_draw_byte(uint8_t c)
{
    static uint8_t l=0;
	
	uint8_t i;
//	if(c==0)
//	{
//		c=0xFF;
//	}
//	c=c<<1;
    for (i=8; i; --i,c<<=1)
	  {
//	    PORTC &= ~((1<<R1)|(1<<R2));
	    if (c & 0x80)
	    /* shift a one */
//		    PORTC |= (1<<R1)|(1<<R2)|(1<<G1)|(1<<G2);
		    PORTC |= (1<<R2)|(1<<G1);
	    else
	    /* shift a zero */
//		    PORTC &= ~((1<<R1)|(1<<R2)|(1<<G1)|(1<<G2));
		    PORTC &= ~((1<<R2)|(1<<G1));

	    /* clock pulse */
	    PORTD &= ~(1<<SK);
	    PORTD |= (1<<SK);
		
	  }
	mx_disable();
    mx_row(l++);
	PORTD |= (1<<LT);
//		asm("NOP");
//		asm("NOP");
	PORTD &= ~(1<<LT);
	mx_enable();
}