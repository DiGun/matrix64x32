#ifndef _74HC595_H
#define _74HC595_H

#include <avr/io.h>

#ifndef USE_SPI
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || defined(__AVR_ATmega32__)
    #define USE_SPI 1
#else
    #define USE_SPI 0
#endif
#endif
/************************************************************************/
/* mega8           595              matrix                                  */
/*   SS PB2  16   STCP  12  LATCH   G1    LT  Á                            */
/* MOSI PB3  17     DS  14  DATA    LT    R1  Æ2 Î2                         */
/*  SCK PB5  19   SHCP  11  CLOCK   SK    SK  ×                             */
/*   OE PB0  14     OE  13  OUTEN                                       */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/


#define SHIFTREGISTER DDRB
#define SHIFTPORT PORTB

#define LATCH PB2   /* SS (RCK) (not necessarily on SS) */
#define DATA  PB3   /* MOSI (SI) */
#define CLOCK PB5   /* SCK (SCK) */
#define OUTEN PB0   /* OE - output enable (active low) */


void shift_init(void);
void shift_enable(void);
// inline void shift_disable(void);
void shift_out(uint8_t);
void shift_out_array(uint8_t*, uint8_t);

#endif
