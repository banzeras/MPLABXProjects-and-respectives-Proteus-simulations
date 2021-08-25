/*
 * File:   newmain.c
 * Author: Marco
 *
 * Created on 25 de Abril de 2018, 11:01
 */


#include <xc.h>
#include <pic18f4550.h>
#include <pic18.h>


#define ligaDesliga PORTCbits.RC1
#define LED PORTBbits.RB0
#define _XTAL_FREQ 20000000
#pragma config PBADEN = OFF 
#pragma config WDT = OFF




char flagSegundo = 0;
char cont = 0;
char flagEstado = 1;
char sec = 0;

void interrupt low_priority pic_isr(void) {



    if (TMR0IF) {

        cont++;
        if (cont > 1) {
            flagSegundo = 1;
            cont = 0;
        }


        TMR0 = 26474;
        INTCONbits.TMR0IF = 0;
    }

}

void main(void) {

    TRISB = 0x00;
    TRISC = 0x01;
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 0;
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0CS = 0;
    TMR0 = 26474;
    T0CONbits.TMR0ON = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    char estado;
    char ledPiscando;

    while (1) {
        estado = ligaDesliga;
        
        if(ligaDesliga){
            flagEstado++;
            flagEstado = flagEstado % 2;
            while(ligaDesliga);
        }
        
        if(flagEstado){
            if (flagSegundo) {

                if (LED == 0) {
                    LED = 1;
                } else {
                    LED = 0;
                }
                
                
                flagSegundo = 0;
            }
        }else{
            if(flagSegundo){
                sec++;
                sec = sec %4 ;
                if(sec == 3){
                    if (LED == 0) {
                        LED = 1;
                    } else {
                        LED = 0;
                    }
                }
                flagSegundo = 0;
            }
        }


            
        
    }


    return;
}
