/*
 * matrix.h
 *
 * Created: 02.07.2017 10:46:19
 *  Author: DiGun
 */ 


#ifndef MATRIX_H_
#define MATRIX_H_
#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>
#include <avr/pgmspace.h>


#define LA PD2
#define LB PD3
#define LC PD4
#define LD PD5

#define LT PD6
#define SK PD7

#define G1 PC1
#define G2 PC2

#define R1 PC3
#define R2 PC4

#define OE PC5

#define MX_COLOR_OFF 0
#define MX_COLOR_RED 1
#define MX_COLOR_GREEN 2
#define MX_COLOR_ORANGE 3

#define MX_UP 0
#define MX_DOWN 1
#define MX_LEFT 2
#define MX_RIGHT 3
#define MX_CLEAR 4


#define MX_CYCLE 4
#define MX_CLR 8


#define MX_X 64
#define MX_Y 32


uint8_t mxR[32][8];
uint8_t mxG[32][8];

volatile uint8_t mx_frame_cnt;
uint8_t mx_color;

struct mx_point 
{
	uint8_t y;
	uint8_t x;
};

struct mx_point mx_cursor;

void mx_init(void);
void mx_enable();
void mx_disable();
//void mx_draw(uint8_t c);
void mx_draw_byte(uint8_t r1,uint8_t g1,uint8_t r2,uint8_t g2);
void mx_draw_row();

void mx_char(char c);
void mx_string_p(const char* s);
void mx_scroll(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t dir);
void mx_scroll_char(char c,uint8_t step,uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t pdir);
void mx_clear(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2,uint8_t color);

void mx_digit(char d);
void mx_hex(uint8_t d);

void mx_clear_all();
void mx_plot(uint8_t x, uint8_t y);
void mx_draw(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2);


#endif /* MATRIX_H_ */