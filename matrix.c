/*
 * CFile1.c
 *
 * Created: 02.07.2017 10:46:48
 *  Author: DiGun
 */ 
#include "matrix.h"
#include "font5x7.h"

//uint8_t mx_frame=0;
void mx_init(void)
{	
	DDRD|=(1<<LA) |(1<<LB) |(1<<LC) |(1<<LD)|(1<<LT)|(1<<SK);
	DDRC|=(1<<R1) |(1<<R2) |(1<<G1) |(1<<G2)|(1<<OE);
	PORTD|=(1<<LA) |(1<<LB) |(1<<LC) |(1<<LD);
	PORTC|=(1<<OE);
	
	// set up timer with prescaler = 256
//	TCCR0 |= (1 << CS01)|(1 << CS00);
	TCCR0 |= (1 << CS01);

//	TCCR0 |= (1 << CS02);
	// enable overflow interrupt
	TIMSK |= (1 << TOIE0);	
	
	
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t count=0;
	cli();
	count++;
	if (count==5)
	{
		mx_draw_row();
		count=0;
	}
		
//	TCNT0=178;
//	TCNT0=200;

	sei();
}

void mx_string_p(const char* s, uint8_t color)
{
	while (pgm_read_byte(s)!='\0') // Пока первый байт строки не 0 (конец ASCIIZ строки)
	{
		mx_char(pgm_read_byte(s),color); // Мы шлем байты из строки
		s++; // Не забывая увеличивать указатель,
	} // Выбирая новую букву из строки.
}


void mx_char(char c, uint8_t color)
{
	uint8_t ch;
	uint8_t f_byte;
	uint8_t f_bit;
	
	uint8_t i;
	uint8_t b;
	
	for (i=0;i<5;i++)
	{
		ch=pgm_read_byte(&Font5x7[c-32][i]);
		f_byte=mx_cursor.x /8;
		f_bit=0x80>>(mx_cursor.x %8);
		for (b=0;b<7;b++)
		{
			if (ch&(1<<b))
			{
				if (color&MX_COLOR_RED)
				{
					mxR[mx_cursor.y+b][f_byte]|=f_bit;
				}
				else
				{
					mxR[mx_cursor.y+b][f_byte]&=~(f_bit);					
				}
				if (color&MX_COLOR_GREEN)
				{
					mxG[mx_cursor.y+b][f_byte]|=f_bit;
				}
				else
				{
					mxG[mx_cursor.y+b][f_byte]&=~(f_bit);					
				}
			} 
		}
		mx_cursor.x++;
	}
	mx_cursor.x++;
}

void mx_scroll(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t dir,uint8_t clr)
{
	uint8_t r;
	uint8_t b;

	uint8_t f_byte;
	uint8_t f_mask;
	uint8_t r_byte;
	uint8_t r_mask;
	uint8_t saveR;
	uint8_t saveG;
	
	
	f_byte=x1/8;
	f_mask=0xFF>>(x1%8);
	r_byte=(x1+x2)/8;
	r_mask=0xFF<<((x1+x2)%8);
	if (dir==MX_UP||dir==MX_DOWN)
	{
		uint8_t mask=0;
		for (b=0;b<8;b++)
		{
			if (f_byte>b)
			{
				mask=0;
			}
			else
			{
				if (f_byte==b)
				{
					if(f_byte!=r_byte)
					{
						mask=f_mask;
					}
					else
					{
						mask=f_mask&r_mask;
					}
				}
				else
				{
					if (f_byte<b)
					{
						if (r_byte>b)
						{
							mask=0xFF;
						} 
						else
						{
							if (r_byte==b)
							{
								mask=r_mask;
							}
							else
							{
								mask=0;
							}
						}
					}
				}
			}
//			mxR[31][b]=mask;
//			mxG[31][b]=~mask;
			if(mask==0)	continue;
			saveR=mxR[y1][b];
			saveG=mxG[y1][b];
			for (r=y1+1;r<(y1+y2);r++)
			{
				mxR[r-1][b]=(mxR[r-1][b]&(~mask))|(mxR[r][b]&(mask));
				mxG[r-1][b]=(mxG[r-1][b]&(~mask))|(mxG[r][b]&(mask));
			}
			r--;
			mxR[r][b]=(mxR[r][b]&(~mask))|(saveR&(mask));
			mxG[r][b]=(mxG[r][b]&(~mask))|(saveG&(mask));
		}
		
	}
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

inline void mx_draw_row()
{
	uint8_t i;
    static uint8_t row=0;	
	for(i=0;i<8;i++)
	{
		mx_draw_byte(mxR[row][i],mxG[row][i],mxR[row+16][i],mxG[row+16][i]);
	}
	mx_disable();
	mx_row(row);
	row++;
	PORTD |= (1<<LT);
	PORTD &= ~(1<<LT);
	mx_enable();
	row&=0x0F;
	if(row==0)
	{
		mx_frame_cnt++;
	}
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

void mx_digit(char d, uint8_t color)
{
	if (d>=10)
	{
		mx_char(d+7+'0',color);
	}
	else
	{
		mx_char(d+'0',color);
	}
}

void mx_hex(uint8_t d, uint8_t color)
{
	mx_digit((d>>4)&0x0F,color);
	mx_digit((d)&0x0F,color);
}