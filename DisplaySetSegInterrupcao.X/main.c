/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 29 de Março de 2018, 14:13
 */


#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 20000000
#pragma config PBADEN = OFF 
#pragma config WDT = OFF


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

void main() {
    ADCON1 = 0x0F;
    TRISB = 0x00;
    char cont = 0;
    
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS0 = 0;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0CS = 0;
    TMR0 = 26474;
    T0CONbits.TMR0ON = 1;
    
    
     while(1){
      
        if(INTCONbits.TMR0IF == 1){
            
               PORTB = display[cont];
                cont++;
                
                if(cont >9)  cont = 0;
                   
                 
        
            TMR0 = 26474;
            INTCONbits.TMR0IF = 0;
        }
    }
}  