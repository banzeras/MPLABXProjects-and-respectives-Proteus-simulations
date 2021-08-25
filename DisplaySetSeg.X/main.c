/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 29 de Março de 2018, 13:32
 */


#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 20000000
#pragma config PBADEN = OFF 

int x=0, y=0;

char const display[10]={
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F,
};

void main(void) {
    ADCON1 = 0x0F;
    TRISB = 0x00;
    
    PORTB = display[0];
    __delay_ms(100);
    PORTB = display[1];
    __delay_ms(100);
    PORTB = display[2];
    __delay_ms(100);
    PORTB = display[3];
    __delay_ms(100);
    PORTB = display[4];
    __delay_ms(100);
    PORTB = display[5];
    __delay_ms(100);
    PORTB = display[6];
    __delay_ms(100);
    PORTB = display[7];
    __delay_ms(100);
    PORTB = display[8];
    __delay_ms(100);
    PORTB = display[9];
     __delay_ms(100);
    return;
}
