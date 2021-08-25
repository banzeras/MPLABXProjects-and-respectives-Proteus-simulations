/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 8 de Março de 2018, 14:04
 */


#include <xc.h>
#include <p18f4550.h>

#pragma config WDT=OFF

#define _XTAL_FREQ 20000000





void main(void) {
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 1;
    
   
   
    PORTCbits.RC0 = 0;
    
    while(1){
     
      PORTCbits.RC0 = PORTCbits.RC1;
      __delay_ms(10);
    }
    
    return;
}
