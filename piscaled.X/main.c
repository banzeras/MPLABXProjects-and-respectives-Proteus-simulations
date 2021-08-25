/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 8 de Março de 2018, 13:21
 */


#include <xc.h>
#include <p18f4550.h>

#pragma CONFIG WDT=OFF
#define _XTAL_FREQ 20000000

void acionaLed(){
   
     PORTBbits.RB0 = 1;
     __delay_ms(1000);
     PORTBbits.RB0 = 0;
     __delay_ms(1000);
}


void main(void) {
    TRISBbits.TRISB0 = 0;
    
    while(1){
        acionaLed();
    }
    
    return;
}
