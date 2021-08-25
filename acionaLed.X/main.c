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
    char press = 'n';
    
    while(1){
        PORTCbits.RC0 = PORTCbits.RC1;
        __delay_ms(10);
        
//        if(PORTCbits.RC1 > 0){
//          PORTCbits.RC0 = PORTCbits.RC1;
//          press = 's';
//           __delay_ms(1000);
//        }
//        if(PORTCbits.RC1 > 0 && press == 's'){
//            PORTCbits.RC0 = !PORTCbits.RC1;
//             press = 'n';
//              __delay_ms(1000);
//        }
     
    }
    
    return;
}
