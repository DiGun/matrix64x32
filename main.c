/*
* Matrix.c
*
* Created: 11.06.2017 15:11:46
* Author : DiGun
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "matrix.h"


unsigned char L;
unsigned char G;


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

	L=0;
	G=0;

	mx_init();
	DDRD|=1;
//	init_screen2();
	mx_disable();
	
	// enable global interrupts
//	mxG[28][7]=mx_frame;
	sei();
	_delay_ms(500);
	mxR[27][7]=1;
	_delay_ms(1000);
	mxR[27][7]=3;
	_delay_ms(1000);
//	mxR[31][7]=mx_frame;
	mxR[27][7]=7;
	_delay_ms(1000);
	init_screen1();	
	while (1)
	{
		_delay_ms(1);
//		++mxR[31][7];
//		mxG[30][7]=mx_frame_cnt;
//		mxR[31][7]=mx_frame;
//		PORTD^=1;
		if(mx_frame_cnt>=10)
		{
			init_screen1();
			mx_frame_cnt=0;
		}
	}
}

