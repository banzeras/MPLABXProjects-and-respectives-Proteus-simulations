/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 15 de Março de 2018, 13:46
 */


#include <xc.h>
#include <p18f4550.h>

#pragma config WDT=OFF

#define _XTAL_FREQ 20000000

void interrupt low_priority pic_isr(void){
    //timer interrupt
    char x;
    if(TMR0IF){
        x++;
        TMR0 = 0;
         INTCONbits.TMR0IF = 0;
         if(x==1)
         {
             PORTDbits.RD0 = PORTDbits.RD0 +1;
             x = 0;
         }
    }
    
}




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
     
     INTCONbits.GIEH = 1;
     INTCONbits.GIEL = 1;
     
     while(1){
        
     }
    
    
    return;
}
