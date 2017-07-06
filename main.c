/*
 * Matrix.c
 *
 * Created: 11.06.2017 15:11:46
 * Author : DiGun
 */ 

#include <avr/io.h>
#if INDICATOR
#include "tm1638.h"
#endif

//#include "74hc595.h"
#include "matrix.h"

/*
#define LA PD2
#define LB PD3
#define LC PD4
#define LD PD5

#define G1 PD6
#define G2 PD7

#define R1 PC3
#define R2 PC4

#define LT 1
#define SK 1
*/
// #define LED PIND0


#define MAX_IC 1
unsigned char L;
unsigned char G;

#if INDICATOR
void btn_check()
{
	if (button_press(1))
	{
		L++;
		L&=0x0f;
//		PORTD&=~0b00111100;
//		PORTD|=(L<<2);
	}
	if(getButtons()&0x01)
	{
		setDisplayDigit(L,0,false);
	}
	else
	{
		clearDisplayDigit(0,false);
	}
	
	
	
	if (button_press(2))
	{
		G++;
		G&=0b00000011;
//		PORTD&=~0b11000000;
//		PORTD|=(G<<6);
	}
	if(getButtons()&0x02)
	{
		setDisplayDigit(G,1,false);
	}
	else
	{
		clearDisplayDigit(1,false);
	}
}
#endif


int main(void)
{
    /* Replace with your application code */
	unsigned char f=0;
	unsigned char i=0;
	L=0;
	G=0;
//	shift_init();
//	shift_enable();
//	unsigned char a[MAX_IC];

#if INDICATOR
	port_setup();
	init();
#endif
	mx_init();	
	DDRD|=1;
//	DDRD|=(1<<LA) |(1<<LB) |(1<<LC) |(1<<LD)|(1<<G1)|(1<<G2);
//	DDRC|=(1<<R1) |(1<<R2);
	mx_disable();
    while (1) 
    {
	
		PORTD^=1;
#if INDICATOR
		btn_check();
#endif
//		_delay_ms(10);
		i=0xff>>((f++)%8);
#if INDICATOR		
		setLEDs(i);
		setDisplayDigit(f&0x0f,7,false);
#endif		
		mx_draw(0);
//		shift_out(i);
		
		
    }
}

