

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
char cont = 4;
char amareloLigado = 0;
char verdeLigado = 0;
char vermelhoLigado = 1;

void interrupt low_priority pic_isr(void){
  
    
    
     if(TMR0IF){
         
         
        if(cont != 0) PORTB = display[cont];
        
        
         if((cont < 1 && verdeLigado == 0)&&(amareloLigado == 0) ){
            PORTCbits.RC0 = 0;
            PORTCbits.RC2 = 1;
            cont = 8;
            vermelhoLigado = 0;
            verdeLigado = 1;
         }
         if((cont < 1 && verdeLigado == 1)&&(vermelhoLigado == 0)){
            PORTCbits.RC1 = 1;
            PORTCbits.RC2 = 0;
            cont = 3;
            amareloLigado =1;
            verdeLigado = 0;
         }
         if((cont < 1) &&(amareloLigado == 1)){
             PORTCbits.RC1 = 0;
             PORTCbits.RC0 = 1;
             vermelhoLigado = 1;
             amareloLigado = 0;
            cont = 5;
         }
         
         
         cont--;
         TMR0 =26474;
         INTCONbits.TMR0IF = 0;
     }
    
}




void main(void) {
      ADCON1 = 0x0F;
      TRISB = 0x00;
      TRISC = 0x00;
              
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
      INTCONbits.GIEH = 1;
      INTCONbits.GIEL = 1;
      PORTCbits.RC0 = 1;
     
    
     
     while(1){
        
     }
    
    
    return;
}
