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

inline void mx_row(uint8_t r)
{
	uint8_t t=PORTD&0b11000011;
	r=(r&0x0F)<<2;
//0b00111100	
    PORTD=(t|r);
}

void mx_draw_row()
{
	uint8_t i;
    static uint8_t l=0;	
	l++;
	l&=0x0F;
	for(i=0;i<8;i++)
	{
		mx_draw_byte(mxR[l][i],mxG[l][i],mxR[l+16][i],mxG[l+16][i]);
	}
	mx_disable();
	mx_row(l);
	PORTD |= (1<<LT);
	PORTD &= ~(1<<LT);
	mx_enable();
}

inline void mx_draw_byte(uint8_t r1,uint8_t g1,uint8_t r2,uint8_t g2)
{
//    static uint8_t l=0;
	uint8_t i;
    for (i=8; i; --i,r1<<=1,g1<<=1,r2<<=1,g2<<=1)
	  {
	    if (r1 & 0x80)
		    PORTC &= ~((1<<R1));
	    else
		    PORTC |= (1<<R1);
	    if (g1 & 0x80)
		    PORTC &= ~((1<<G1));
	    else
			PORTC |= (1<<G1);
	    if (r2 & 0x80)
			PORTC &= ~((1<<R2));
	    else
			PORTC |= (1<<R2);
	    if (g2 & 0x80)
			PORTC &= ~((1<<G2));
	    else
			PORTC |= (1<<G2);

	    /* clock pulse */
	    PORTD &= ~(1<<SK);
	    PORTD |= (1<<SK);
	  }
}