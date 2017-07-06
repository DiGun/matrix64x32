/*
 * tm16381.h
 *
 * Created: 17.05.2017 18:37:02
 *  Author: DiGun
 */ 


#ifndef TM1638_H_
#define TM1638_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#define TM_DDR          DDRB
#define TM_OUT			PORTB
#define TM_BIT_STB		(1<<PORTB0)
#define TM_BIT_CLK		(1<<PORTB1)
#define TM_BIT_DIO		(1<<PORTB2)

#define TM_DIO_IN()     (TM_DDR &= ~TM_BIT_DIO)
#define TM_DIO_OUT()    (TM_DDR |= TM_BIT_DIO)

#define TM_DI			PINB2
#define TM_PIN_DI		PINB

#define TM_CLK_LOW()            (TM_OUT &= ~TM_BIT_CLK)
#define TM_CLK_HIGH()           (TM_OUT |= TM_BIT_CLK)
#define TM_DO_LOW()            (TM_OUT &= ~TM_BIT_DIO)
#define TM_DO_HIGH()           (TM_OUT |= TM_BIT_DIO)
#define TM_STB_LOW()            (TM_OUT &= ~TM_BIT_STB)
#define TM_STB_HIGH()           (TM_OUT |= TM_BIT_STB)

#define TM_LED_LOW()            (TM_OUT &= ~TM_BIT_LED)
#define TM_LED_HIGH()           (TM_OUT |= TM_BIT_LED)

#define TM_COLOR_NONE	0
#define TM_COLOR_RED	1
#define TM_COLOR_GREEN	2

#define DELAY_US                1

void send(uint8_t data);

void sendCommand(uint8_t cmd);

void sendData(uint8_t address, uint8_t data);

void sendChar(uint8_t pos, uint8_t data, bool dot);

void setDisplayDigit(uint8_t digit, uint8_t pos, bool dot);

void clearDisplayDigit(uint8_t pos, bool dot);

void setDisplayToDecNumberAt(unsigned long number, uint8_t dots, uint8_t startingPos,uint8_t len, bool leadingZeros);

uint8_t receive();

uint8_t getButtons(void);
int8_t btn_last;
bool button_press(char btn);


void clear(char fill,bool led);

void init();

void port_setup();

void setLED(uint8_t color, uint8_t pos);

void setLEDs(uint16_t leds);
void sendArray(uint8_t* data , uint8_t len);
void sendError();





#endif /* TM1638_H_ */