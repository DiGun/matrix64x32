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

void mx_digit(char d)
{
	if (d>=10)
	{
		mx_char(d+7+'0');
	}
	else
	{
		mx_char(d+'0');
	}
}

void mx_hex(uint8_t d)
{
	mx_digit((d>>4)&0x0F);
	mx_digit((d)&0x0F);
}

void mx_string_p(const char* s)
{
	while (pgm_read_byte(s)!='\0') // Пока первый байт строки не 0 (конец ASCIIZ строки)
	{
		mx_char(pgm_read_byte(s)); // Мы шлем байты из строки
		s++; // Не забывая увеличивать указатель,
	} // Выбирая новую букву из строки.
}


void mx_char(char c)
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
				if (mx_color&MX_COLOR_RED)
				{
					mxR[mx_cursor.y+b][f_byte]|=f_bit;
				}
				else
				{
					mxR[mx_cursor.y+b][f_byte]&=~(f_bit);
				}
				if (mx_color&MX_COLOR_GREEN)
				{
					mxG[mx_cursor.y+b][f_byte]|=f_bit;
				}
				else
				{
					mxG[mx_cursor.y+b][f_byte]&=~(f_bit);
				}
			}
			else
			{
				mxR[mx_cursor.y+b][f_byte]&=~(f_bit);
				mxG[mx_cursor.y+b][f_byte]&=~(f_bit);
			}
		}
		mx_cursor.x++;
	}
	mx_cursor.x++;
}

uint8_t mx_byte_mask(uint8_t b, uint8_t f_byte, uint8_t f_mask,uint8_t r_byte, uint8_t r_mask)
{
	
	if (f_byte>b)
	{
		return 0;
	}
	else
	{
		if (f_byte==b)
		{
			if(f_byte!=r_byte)
			{
				return f_mask;
			}
			else
			{
				return f_mask&r_mask;
			}
		}
		else
		{
			if (f_byte<b)
			{
				if (r_byte>b)
				{
					return 0xFF;
				}
				else
				{
					if (r_byte==b)
					{
						return r_mask;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}
	return 0;
}


void mx_scroll(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t pdir)
{
	int8_t r;
	int8_t b;

	uint8_t f_byte;
	uint8_t f_mask;
	uint8_t f_shift;
	uint8_t r_byte;
	uint8_t r_mask;
	uint8_t r_shift;
	uint8_t saveR;
	uint8_t saveG;

	uint8_t dir=pdir&3;
	
	
	f_byte=x1/8;
	f_shift=(x1%8);
	f_mask=0xFF>>f_shift;
	r_byte=x2/8;
	r_shift=(7-(x2%8));
	r_mask=0xFF<<r_shift;
	if ((dir)==MX_UP||dir==MX_DOWN)
	{
		for (b=0;b<8;b++)
		{
			uint8_t mask=mx_byte_mask(b,  f_byte, f_mask,r_byte, r_mask);
			if(mask==0)	continue;
			if (dir==MX_UP)
			{
				saveR=mxR[y1][b];
				saveG=mxG[y1][b];
				for (r=y1+1;r<=(y2);r++)
				{
					mxR[r-1][b]=(mxR[r-1][b]&(~mask))|(mxR[r][b]&(mask));
					mxG[r-1][b]=(mxG[r-1][b]&(~mask))|(mxG[r][b]&(mask));
				}
				r--;
			}
			else
			{
				saveR=mxR[y2][b];
				saveG=mxG[y2][b];
				for (r=y2-1;r>=(y1);r--)
				{
					mxR[r+1][b]=(mxR[r+1][b]&(~mask))|(mxR[r][b]&(mask));
					mxG[r+1][b]=(mxG[r+1][b]&(~mask))|(mxG[r][b]&(mask));
				}
				r++;
			}
			mxR[r][b]=(mxR[r][b]&(~mask))|(saveR&(mask));
			mxG[r][b]=(mxG[r][b]&(~mask))|(saveG&(mask));
		}
	}
	else
	{
		uint8_t mask[8];
		for (b=0;b<8;b++)
		{
			mask[b]=mx_byte_mask(b,  f_byte, f_mask,r_byte, r_mask);
		}
		if (dir==MX_RIGHT)
		{
			for (r=y1;r<=y2;r++)
			{
				saveG=(mxG[r][r_byte]&(1<<r_shift));
				saveR=(mxR[r][r_byte]&(1<<r_shift));
				    b = f_byte;
				    do
				    {
						uint8_t overR = mxR[r][b] & 0x01;
						uint8_t overG = mxG[r][b] & 0x01;
				        uint8_t ShiftR = mxR[r][b]>>1;
				        uint8_t ShiftG = mxG[r][b]>>1;
//				        uint8_t ShiftR = mxR[r][b] & (~mask[b]);
//				        uint8_t ShiftG = mxG[r][b] & (~mask[b]);
				        mxR[r][b] >>= 1;
						mxG[r][b] >>= 1;
						if (b == f_byte)
				        {
					        if(saveR!=0)
								ShiftR|=0x80>>f_shift;
					        else
								ShiftR &= ~(0x80>>f_shift);
					        if(saveG!=0)
								ShiftG |= 0x80>>f_shift;
					        else
								ShiftG &= ~(0x80>>f_shift);
						}
						else
						{
					       ShiftR |= (saveR) << 7;
					       ShiftG |= (saveG) << 7;
						}
						mxR[r][b] = (ShiftR & mask[b]) | (mxR[r][b] & (~mask[b]));
						mxG[r][b] = (ShiftG & mask[b]) | (mxG[r][b] & (~mask[b]));
						b++;
						if (b > r_byte)
							break;
						saveR = overR;
						saveG = overG;
				}
				while(1);
			}
		}
		
	}

}