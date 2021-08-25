/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 15 de Março de 2018, 13:24
 */

#include <xc.h>
#include <p18f4550.h>

#pragma config WDT=OFF

#define _XTAL_FREQ 20000000


void main(void) {
    
    T0CONbits.TMR0ON = 0;
    
    INTCONbits.TMR0IF = 1;
    INTCONbits.TMR0IF = 0;
    
     T0CONbits.T08BIT = 0;
     T0CONbits.PSA = 0;
     
     T0CONbits.T0PS0 = 1;
     T0CONbits.T0PS1 = 1;
     T0CONbits.T0PS2 = 0;
     T0CONbits.T0CS = 0;
     
     TMR0 = 0;
     T0CONbits.TMR0ON = 1;
     
     while(1){
         if(INTCONbits.TMR0IF == 1)
         {    
         PORTDbits.RD0 = PORTDbits.RD0+1;
         TMR0 = 0;
         INTCONbits.TMR0IF == 0;
         }
     }
    
    
    return;
}
