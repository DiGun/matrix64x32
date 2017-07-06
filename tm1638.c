/*
 * CFile1.c
 *
 * Created: 17.05.2017 18:42:17
 *  Author: DiGun
 */ 
#include "tm1638.h"

PROGMEM const uint8_t NUMBER_FONT[] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // B
	0b00111001, // C
	0b01011110, // D
	0b01111001, // E
	0b01110001  // F
};

PROGMEM const uint8_t MINUS = 0b01000000;

// definition for error
PROGMEM const uint8_t ERROR_DATA[] = {
	0b01111001, // E
	0b01010000, // r
	0b01010000, // r
	0b01011100, // o
	0b01010000, // r
	0,
	0,
	0
};


void send(uint8_t data)
{
	for (int i = 0; i < 8; i++)
	{
		TM_CLK_LOW();
		if(data & 1)
		TM_DO_HIGH();
		else
		TM_DO_LOW();
		data >>= 1;
		TM_CLK_HIGH();
	}
}

void sendCommand(uint8_t cmd)
{
	TM_STB_LOW();
	send(cmd);
	TM_STB_HIGH();
}

void sendData(uint8_t address, uint8_t data)
{
	sendCommand(0x44);
	TM_STB_LOW();
	send(0xC0 | address);
	send(data);
	TM_STB_HIGH();
}

void sendArray(uint8_t* data , uint8_t len)
{
	for (int i = 0; i < len ; i++) 
	{
		sendData(i << 1, data[i]);
	}
}

void sendError()
{
	for (int i = 0; i < 8 ; i++)
	{
		sendData(i << 1, pgm_read_byte(&ERROR_DATA[i]));
	}
}

void sendChar(uint8_t pos, uint8_t data, bool dot)
{
	sendData(pos << 1, data | (dot ? 0b10000000 : 0));
}


void setDisplayDigit(uint8_t digit, uint8_t pos, bool dot)
{
	sendChar(pos, pgm_read_byte(&NUMBER_FONT[digit & 0xF]), dot);
}

void clearDisplayDigit(uint8_t pos, bool dot)
{
	sendChar(pos, 0, dot);
}

void setDisplayToDecNumberAt(unsigned long number, uint8_t dots, uint8_t startingPos,uint8_t len, bool leadingZeros)
{
	if (number > 99999999L)
	{
		//		setDisplayToError();
		sendError();
		//;
	}
	else
	{
		for (int i = startingPos; (i < 8)&&(i<startingPos+len) ; i++) {
			if (number != 0) {
				setDisplayDigit(number % 10, 8 - i - 1, (dots & (1 << i)) != 0);
				number /= 10;
				} else {
				if (leadingZeros) {
					setDisplayDigit(0, 8 - i - 1, (dots & (1 << i)) != 0);
					} else {
					clearDisplayDigit(8 - i - 1, (dots & (1 << i)) != 0);
				}
			}
		}
	}
}


uint8_t receive()
{
	uint8_t temp = 0;
	// Pull-up on
	TM_DIO_IN();
	TM_DO_HIGH();
	for (int i = 0; i < 8; i++)
	{
		temp >>= 1;
		TM_CLK_LOW();
		_delay_us(DELAY_US);
		if ((TM_PIN_DI &(1<<TM_DI))!=0)
		{
			temp |= 0x80;
		}
		TM_CLK_HIGH();
	}

	// Pull-up off
	TM_DIO_OUT();
	TM_DO_LOW();
	return temp;
}


uint8_t getButtons(void)
{
	uint8_t keys = 0;

	TM_STB_LOW();
	send(0x42);
	for (int i = 0; i < 4; i++) {
		keys |= receive() << i;
	}
	TM_STB_HIGH();

	return keys;
}


void clear(char fill,bool led)
{
	sendCommand(0x40);
	TM_STB_LOW();
	send(0xC0);
	for (char i = 0; i < 8; i++)
	{
		//		 write(0xC0+i+i); // address
		send(fill);
		send(led);
	}
	TM_STB_HIGH();
	//	 TM_DIO_IN();
}

void init()
{
	TM_DIO_OUT();
	TM_STB_HIGH();
	TM_CLK_HIGH();
	_delay_us(DELAY_US);
	sendCommand(0x40);
	sendCommand(0x80 | 8 | 6);
	TM_STB_LOW();
	send(0xC0);
	for (int i = 0; i < 16; i++) {
		send(0x00);
	}
	TM_STB_HIGH();
	//	    clear(0xff);
}

void port_setup()
{
	TM_DDR |= TM_BIT_CLK | TM_BIT_STB;
}

void setLED(uint8_t color, uint8_t pos)
{
	sendData((pos << 1) + 1, color);
}

void setLEDs(uint16_t leds)
{
	for (int i = 0; i < 8; i++) {
		uint8_t color = TM_COLOR_NONE;

		if ((leds & (1 << i)) != 0) {
			color |= TM_COLOR_RED;
		}

		if ((leds & (1 << (i + 8))) != 0) {
			color |= TM_COLOR_GREEN;
		}
		setLED(color, i);
	}
}

bool button_press(char btn)
{
	uint8_t bit=1<<(btn-1);
	if(getButtons()&bit)
	{
		if ((btn_last&bit)==0)
		{
			btn_last|=bit;
			return true;
		}
	}
	else
	{
		btn_last&= ~bit;
	}
	return false;
}