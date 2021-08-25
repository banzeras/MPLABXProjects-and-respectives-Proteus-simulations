/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 15 de Março de 2018, 13:55
 */

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
     PORTCbits.RC0 = 1;
     
//    if(TMR0IF){
//       
//       
//         if(PORTCbits.RC0 == 0)
//         {
//             PORTCbits.RC0 = 1;
//            
//         }
//         else{
//             PORTCbits.RC0 = 0;
//         }
//         
         TMR0 = 26474;
         INTCONbits.TMR0IF = 0;
//    }
    
}




void main(void) {
    
     TRISCbits.TRISC0 = 0;
    
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
