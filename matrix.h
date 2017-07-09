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

uint8_t mxR[32][8];
uint8_t mxG[32][8];

volatile uint8_t mx_frame_cnt;

void mx_init(void);
void mx_enable();
void mx_disable();
//void mx_draw(uint8_t c);
void mx_draw_byte(uint8_t r1,uint8_t g1,uint8_t r2,uint8_t g2);
void mx_draw_row();

#endif /* MATRIX_H_ */