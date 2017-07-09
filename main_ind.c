/*
* Matrix.c
*
* Created: 11.06.2017 15:11:46
* Author : DiGun
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

void init_screen1(void)
{
	static uint8_t a=0;
	uint8_t x;
	uint8_t y;
	for (y=0;y<32;y++)
	{
		for (x=0;x<8;x++)
		{
			switch ((x+((y+a)>>3))%4)
			{
				case 0:
				mxR[y][x]=0;
				break;
				case 1:
				mxR[y][x]=0x0F;
				break;
				case 2:
				mxR[y][x]=0xFF;
				break;
				case 3:
				mxR[y][x]=0xF0;
				break;
			}

			switch ((x*2+((y+a)>>2))%3)
			{
				case 0:
				mxG[y][x]=0;
				break;
				case 1:
				mxG[y][x]=0x0F;
				break;
				case 2:
				mxG[y][x]=0xF0;
				break;
			}
		}
	}
	a++;
}

void init_screen2(void)
{
	uint8_t x;
	uint8_t y;
	for (y=0;y<32;y++)
	{
		for (x=0;x<8;x++)
		{
			if((y/8)==x)
				mxR[y][x]=1<<(y%8);
			else	
				mxR[y][x]=0;
				
			if(((31-y)/8)==x)
				mxG[y][x]=1<<((31-y)%8);
			else
				mxG[y][x]=0;
		}
	}
}

int main(void)
{
//	uint8_t shift=0;
	/* Replace with your application code */
	//	unsigned char f=0;
	//	unsigned char i=0;
	L=0;
	G=0;
	mx_frame=0;
	//	shift_init();
	//	shift_enable();
	//	unsigned char a[MAX_IC];

	#if INDICATOR
	port_setup();
	init();
	#endif
	mx_init();
	DDRD|=1;
	init_screen2();
	mx_disable();
	
	// enable global interrupts
	mxG[28][7]=1;
	sei();
	_delay_ms(500);
	mxR[27][7]=1;
	_delay_ms(1000);
	mxR[27][7]=7;
	while (1)
	{

		mxR[29][7]=1;
		mxR[31][7]=mx_frame;
//		PORTD^=1;
		if(mx_frame==15)
		{
//			init_screen();
			mxG[30][7]=mx_frame;
			mx_frame=0;
		}
		
		#if INDICATOR
		btn_check();
		#endif
		//		_delay_ms(10);
		//		i=0xff>>((f++)%8);
		#if INDICATOR
		setLEDs(i);
		setDisplayDigit(f&0x0f,7,false);
		#endif
//		mx_draw_row();
		//		shift_out(i);
		
		
	}
}

