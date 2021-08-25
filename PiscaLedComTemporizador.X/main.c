/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 16 de Março de 2018, 17:47
 */


#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 20000000
#pragma config WDT = OFF

void main(){
    TRISC = 0b00000010;
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 0;
    T0CONbits.T0CS = 0;
    
    TMR0 = 26474;
    
    T0CONbits.TMR0ON = 1;
    
    while(1){
        
        
        if(INTCONbits.TMR0IF == 1){
            if(PORTCbits.RC0 == 0){
                PORTCbits.RC0 = 1;
            }else{
                PORTCbits.RC0 = 0;
            }
            
            TMR0 = 26474;
            INTCONbits.TMR0IF = 0;
        }
    }
    
    return;
}
