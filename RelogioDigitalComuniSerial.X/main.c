/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 12 de Abril de 2018, 13:20
 */


#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include <stdlib.h>
#include <xlcd.h>
#include <delays.h>


#define _XTAL_FREQ 20000000
#pragma config PBADEN = OFF 
#pragma config WDT = OFF

#define nome_versao  0
#define hora_data  1
#define alarme  2

char flagSegundo = 0;
char cont=0;
char flagDespertadorLigado = 0;
char virouDia = 1;
char botaoHora;
char botaoMin;
char tempoDespertador = 0;
char horarioDespertador[6];
char BUFFER[7];




char tela_atual = 0;
char versao[6] = {'0','1','.','0','0'};


void leitura(){
    
}


void DelayFor18TCY(void){
    Delay10TCYx(120);
}
 
void DelayPORXLCD(void){
    Delay1KTCYx(180);
    return;
}
 
void DelayXLCD(void){
    Delay1KTCYx(60);
    return;

}

typedef struct 
{
char horario[9];
char data[7];
char dias;
char mes_atual;
int ano;
}horas_datas;

char  dias_mes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
horas_datas data_horario;

void interrupt low_priority pic_isr(void){
  
    
    
     if(TMR0IF){
         
         cont++;
         if(cont > 1){
            flagSegundo = 1;
            cont = 0;
         }
         
        
         TMR0 = 26474;
         INTCONbits.TMR0IF = 0;
     }
    
}

char verificaAnoBissexto(int ano){
    if(ano % 4 == 0 ){
        if( ano % 100 != 0){
            return 1;
        }
        else 
        {
            if( ano % 400 != 0){   
                 return 0;
            }
            else{
                return 1;
            }
        } 
    }
    else{
        return 0;
    }
          
    
}

char *converteAnoString(int ano){
    char anoStr[4];
    sprintf(anoStr,"%i",ano);
    return anoStr;
}

void trata_relogio_data(){
    data_horario.horario[7]++;
    if(data_horario.horario[7]>'9'){
        data_horario.horario[7]='0';
        data_horario.horario[6]++;
      if(data_horario.horario[6]>'5'){
          data_horario.horario[6] ='0';
          data_horario.horario[4]++;
          virouDia = 1;
          if(data_horario.horario[4]>'9'){
               data_horario.horario[4] ='0';
              data_horario.horario[3]++;
              if( data_horario.horario[3]>'5'){
                   data_horario.horario[3] ='0';
                    data_horario.horario[1]++;
                    if( data_horario.horario[1]> '9'){
                         data_horario.horario[1]= '0';
                          data_horario.horario[0]++;
                    }
                          if( data_horario.horario[0] == '2' && data_horario.horario[1] > '3'){
                              data_horario.horario[0] = '0';
                              data_horario.horario[1] = '0';
                              data_horario.data[1]++;
                              data_horario.dias++;
                              if(data_horario.data[1]>'9'){
                                   data_horario.data[1]=0;
                                   data_horario.data[0]++;
                              }
                                   if(data_horario.dias > dias_mes[data_horario.mes_atual-1]){
                                        data_horario.data[1]= '1';
                                        data_horario.data[0]='0';
                                        data_horario.mes_atual++;
                                        data_horario.data[4]++;
                                        if( data_horario.data[4] >'9'){
                                             data_horario.data[4] ='0';
                                             data_horario.data[3]++;
                                        }
                                             if(data_horario.mes_atual >11){
                                                 data_horario.data[4] ='1';
                                                 data_horario.data[3]='0';
                                                 data_horario.mes_atual =1;
                                                 data_horario.ano++;
                                                 if(verificaAnoBissexto(data_horario.ano)){
                                                    dias_mes[1] = 29;
                                                 }
                                                 else
                                                 {
                                                    dias_mes[1] = 28;
                                                 }
                                             }   
                                            }
                                        
                                   }
                              
                          }
                    
              }
          }
      }  
        
      
}

void init_XLCD(VOID){
    OpenXLCD(FOUR_BIT&LINES_5X7);
    while (BusyXLCD());
    WriteCmdXLCD(0x06);
    WriteCmdXLCD(0x0C);
}

void acionaDespertador(){
    if(virouDia){
        if((data_horario.horario[0] == horarioDespertador[0]) && (data_horario.horario[1] == horarioDespertador[1]) && (data_horario.horario[3] == horarioDespertador[3]) && (data_horario.horario[4] == horarioDespertador[4])){
            flagDespertadorLigado = 1;
        }
    }
}


void atualiza_Tela(char troca_ou_nao){
    
    if(troca_ou_nao == 1){
        tela_atual++;
        if(tela_atual >2){
            tela_atual = 0;
        }
    }
    switch(tela_atual){
        case nome_versao:
         SetDDRamAddr(0x00);
          putrsXLCD("Despertador     ");
          SetDDRamAddr(0x40);
          putrsXLCD("Versao: ");
          putrsXLCD(versao);
          
        break;
        case hora_data:
           SetDDRamAddr(0x00);
          putrsXLCD("Hora: ");
          putrsXLCD(data_horario.horario);
          SetDDRamAddr(0x40);
          putrsXLCD("Data: ");
          putrsXLCD(data_horario.data);
          putrsXLCD(converteAnoString(data_horario.ano));
          acionaDespertador();
          if(flagDespertadorLigado){
              PORTDbits.RD0 = 1;
               tempoDespertador++;
               if(tempoDespertador > 5){
                    tempoDespertador = 0;
                    flagDespertadorLigado =0;
                    PORTDbits.RD0 = 0;
                    virouDia = 0;
               }
          }
        break;
        case alarme:
          SetDDRamAddr(0x00);
         putrsXLCD("     ALARME     ");
         SetDDRamAddr(0x40);
         putrsXLCD("     ");
         putrsXLCD(horarioDespertador);
         putrsXLCD("      ");
        break;
    }
}

void setaDespertador(){
  botaoHora =  PORTCbits.RC1;
  botaoMin =  PORTCbits.RC2;
        if(botaoHora){
            Delay10KTCYx(180);
            
            horarioDespertador[1]++;
            if(horarioDespertador[1]>'9'){
                horarioDespertador[1] ='0';
                horarioDespertador[0]++;  
            }
            if(horarioDespertador[0] == '2' && horarioDespertador[1] >'3'){
                    horarioDespertador[1] ='0';
                    horarioDespertador[0] ='0';
            }
            
            atualiza_Tela(0); 
            while(botaoHora){
            botaoHora =  PORTCbits.RC1;
            }
        }  
  
  if(botaoMin){
            Delay10KTCYx(180);
            
            horarioDespertador[4]++;
            if(horarioDespertador[4]>'9'){
                horarioDespertador[4]='0';
                horarioDespertador[3]++;
              
            }
            if(horarioDespertador[3] == '5' && horarioDespertador[4] > '9'){
                horarioDespertador[4] ='0';
                horarioDespertador[3] ='0';
            }
            atualiza_Tela(0); 
            while(botaoMin){
           botaoMin =  PORTCbits.RC2;
            }
        }  
    
}



void main(void) {
      ADCON1 = 0x0F;
      TRISB = 0x00;
      TRISD = 0x00;
      TRISC = 0x01;
      char botao;
      
    
      
      data_horario.horario[0] = '2';
      data_horario.horario[1] = '3';
      data_horario.horario[2] = ':';
      data_horario.horario[3] = '5';
      data_horario.horario[4] = '9';
      data_horario.horario[5] = ':';
      data_horario.horario[6] = '5';
      data_horario.horario[7] = '0';
      data_horario.horario[8] = '\0';
       
      data_horario.data[0] = '3';
      data_horario.data[1] = '1';
      data_horario.data[2] = '/';
      data_horario.data[3] = '1';
      data_horario.data[4] = '2';
      data_horario.data[5] = '/';
      data_horario.data[6] = '\0';
      data_horario.ano = 2018;
      data_horario.dias = 31;
      data_horario.mes_atual = 11;
      
      horarioDespertador[0] = '0';
      horarioDespertador[1] = '0';
      horarioDespertador[2] = ':';
      horarioDespertador[3] = '0';
      horarioDespertador[4] = '1';
      horarioDespertador[5] = '\0';
      
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
      
     
     init_XLCD();
     
      
    while(1){
        botao =  PORTCbits.RC0;
        if(botao){
            Delay10KTCYx(180);
            
            atualiza_Tela(1);
            
            while(botao){
             botao =  PORTCbits.RC0;
            }
        }  
      
        if(flagSegundo){
           
            atualiza_Tela(0);  
            trata_relogio_data();
            flagSegundo=0;
      }
        if(tela_atual == 2){
            setaDespertador();
           
        }
    }
    return;
}

