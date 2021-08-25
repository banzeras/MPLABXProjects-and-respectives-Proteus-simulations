/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 12 de Abril de 2018, 13:20
 */




//#include "relogio.h"

#include <xc.h> // include processor files - each processor file is guarded.  
#include <plib/usart.h>
#include <p18f4550.h>
#include <xlcd.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POLY 0x8408
#define _XTAL_FREQ 12000000
#define IDLE 0
#define VERIFICACRC 1
#define COMRECEBIDO 2
#define ENVIARESPOSTA 3
#define ENVIAACK 4
#define ENVIANACK 5	
#define ALTERACAO 2
#define LEITURA 1
#define ACK 	0x05
#define NACK 	0x0F
#define FREQ 12000000
#define baud 9600
#define spbrg_value (((FREQ/64)/baud)-1)
#define nome_versao  0
#define hora_data  1
#define alarme  2


#pragma config PBADEN = OFF 
#pragma config WDT = OFF

typedef struct 
{
char horario[9];
char data[7];
char dias;
char mes_atual;
int ano;
}horas_datas;

char vet[8];
horas_datas data_horario;
char pegaHoraData[3];
unsigned char BUFFCOM[7], BUFFRESP[7];
char estado = IDLE; // estado idle
char versao[6] = {'0','1','.','0','0'};
char horarioDespertador[6];

char i=0;
char flagSegundo = 0;
char cont=0;
char flagDespertadorLigado = 0;
char virouDia = 1;
char botaoHora;
char botaoMin;
char tempoDespertador = 0;
char tela_atual = 0;



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

char  dias_mes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

void enviaByteTeste(char BYTE)
{

    	while(!TXIF);                            // Wait until RCIF gets low
    	TXREG = BYTE;
}


void printaDisplay(){
          SetDDRamAddr(0x00);
          //putrsXLCD(BUFFCOM[1]);
}

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
     
        //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1)
    {
        
        
        while(!RCIF);                            // Wait until RCIF gets low
            BUFFCOM[i]= RCREG;                                   // Retrieve data from reception register
                //enviaByteTeste(BUFFCOM[i]);
          i++;  
        
        if(i>6)
        {
             printaDisplay();
            PIR1bits.RCIF = 0; // clear rx flag
            i=0;
            estado = VERIFICACRC;
            
        }
        
            
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
     if(data_horario.horario[7]> 9){
        data_horario.horario[7]= 0;
        data_horario.horario[6]++;
      if(data_horario.horario[6]> 5 ){
          data_horario.horario[6] = 0;
          data_horario.horario[4]++;
          virouDia = 1;
          if(data_horario.horario[4]>9){
               data_horario.horario[4] = 0;
              data_horario.horario[3]++;
              if( data_horario.horario[3]> 5){
                   data_horario.horario[3] = 0;
                    data_horario.horario[1]++;
                    if( data_horario.horario[1]> 9){
                         data_horario.horario[1]= 0;
                          data_horario.horario[0]++;
                    }
                          if( data_horario.horario[0] == 2 && data_horario.horario[1] > 3){
                              data_horario.horario[0] = 0;
                              data_horario.horario[1] = 0;
                              data_horario.data[1]++;
                              data_horario.dias++;
                              if(data_horario.data[1]> 9){
                                   data_horario.data[1]=0;
                                   data_horario.data[0]++;
                              }
                                   if(data_horario.dias > dias_mes[data_horario.mes_atual-1]){
                                        data_horario.data[1]= 1;
                                        data_horario.data[0]= 0;
                                        data_horario.mes_atual++;
                                        data_horario.data[4]++;
                                        if( data_horario.data[4] > 9){
                                             data_horario.data[4] = 0;
                                             data_horario.data[3]++;
                                        }
                                             if(data_horario.mes_atual >11){
                                                 data_horario.data[4] =1;
                                                 data_horario.data[3]= 0;
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
          sprintf(vet,"%i%i:%i%i:%i%i", data_horario.horario[0], data_horario.horario[1], data_horario.horario[3], data_horario.horario[4], data_horario.horario[6],data_horario.horario[7]);
          putrsXLCD(vet);
          SetDDRamAddr(0x40);
          putrsXLCD("Data: ");
          sprintf(vet,"%i%i/%i%i/",data_horario.data[0],data_horario.data[1],data_horario.data[3],data_horario.data[4]);
          putrsXLCD(vet);
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
         sprintf(vet,"%i%i:%i%i", horarioDespertador[0], horarioDespertador[1], horarioDespertador[3], horarioDespertador[4]);
         putrsXLCD(vet);
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
            if(horarioDespertador[1]>9){
                horarioDespertador[1] = 0;
                horarioDespertador[0]++;  
            }
            if(horarioDespertador[0] == 2 && horarioDespertador[1] > 3){
                    horarioDespertador[1] = 0;
                    horarioDespertador[0] = 0;
            }
            
            atualiza_Tela(0); 
            while(botaoHora){
            botaoHora =  PORTCbits.RC1;
            }
        }  
  
  if(botaoMin){
            Delay10KTCYx(180);
            
            horarioDespertador[4]++;
            if(horarioDespertador[4]>9){
                horarioDespertador[4]=0;
                horarioDespertador[3]++;
              
            }
            if(horarioDespertador[3] == 5 && horarioDespertador[4] > 9){
                horarioDespertador[4] = 0;
                horarioDespertador[3] = 0;
            }
            atualiza_Tela(0); 
            while(botaoMin){
           botaoMin =  PORTCbits.RC2;
            }
        }  
    
}




unsigned short crc16(char *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8;
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
}
//envia buffer de resposta pela serial
void tx_data()
{
	int cont;
	
	for(cont=0;cont<7;cont++)
	{
    	while(!TXIF);                            // Wait until RCIF gets low
    	TXREG = BUFFRESP[cont];
	}
	
	estado = IDLE;
}

void enviaByte(char BYTE)
{

    	while(!TXIF);                            // Wait until RCIF gets low
    	TXREG = BYTE;

		if(BYTE==ACK){
			estado =  COMRECEBIDO;
		}
		else{
			estado = IDLE;
		}

}
void calculaCRCBUFFRESP(){
	
	unsigned short crc;
    
    crc =  crc16(BUFFRESP,5);
    
	BUFFRESP[5] = crc >> 8;
	BUFFRESP[6] = crc & 0xFF;
            
	estado = ENVIARESPOSTA;
}

void trataComando(){

	//verifica se e comando de leitura ou alteracao	
	switch(BUFFCOM[0])
	{
		case LEITURA:
			
			BUFFRESP[0] = BUFFCOM[0];
			BUFFRESP[1] = BUFFCOM[1];
			
			// verifica o que deve ser lido
			switch(BUFFCOM[1])
			{
				//leitura de versao e revisao
				case 0:
				
					BUFFRESP[2] = versao[1];
					BUFFRESP[3] = versao[4];
					
					//posicoes vazias
					BUFFRESP[4] = 0xFF;
				break;			
				
				case 1:
				pegaHoraData[0] =   data_horario.horario[0];
  				pegaHoraData[1] =   data_horario.horario[1];
  				BUFFRESP[2] = atoi(pegaHoraData);
  				pegaHoraData[0] =   data_horario.horario[3];
  				pegaHoraData[1] =   data_horario.horario[4];
  				BUFFRESP[3] =  atoi(pegaHoraData);
  				pegaHoraData[0] =   data_horario.horario[6];
  				pegaHoraData[1] =   data_horario.horario[7];
  				BUFFRESP[4] =  atoi(pegaHoraData);
				break;
				
				case 2:
				pegaHoraData[0] =   data_horario.data[0];
  				pegaHoraData[1] =   data_horario.data[1];
  				BUFFRESP[2] = atoi(pegaHoraData);
  				pegaHoraData[0] =   data_horario.data[3];
  				pegaHoraData[1] =   data_horario.data[4];
  				BUFFRESP[3] = atoi(pegaHoraData);
  				BUFFRESP[4] = data_horario.ano - 2000;
				
				break;
				case 3:
				pegaHoraData[0] =   horarioDespertador[0];
  				pegaHoraData[1] =   horarioDespertador[1];
  				BUFFRESP[2] = atoi(pegaHoraData);
  				pegaHoraData[0] =   horarioDespertador[3];
  				pegaHoraData[1] =   horarioDespertador[4];
  				BUFFRESP[3] = atoi(pegaHoraData);
  				BUFFRESP[4] = 0xFF;
				break;
				
				default:
				break;
				
			}
				
		break;
		
		
		case ALTERACAO:
			
			//utilizar funcao memcpy, obrigatorio!
			//copia buffer de comando em buffer de resposta
		 	 memcpy(BUFFRESP,BUFFCOM,strlen(BUFFCOM));
             if(BUFFCOM[1] == (1+'0')){
                   printaDisplay();
             }
			
			// verifica o que deve ser lido
			switch(BUFFCOM[1])
			{
				//altera versao e revisao 02000200
				case 0:
				
					versao[1] = BUFFCOM[2]+48;
					versao[4] = BUFFCOM[3]+48;
				break;			
				
				case 1:
                    printaDisplay();
                    data_horario.data[0] =  ((BUFFCOM[2]/10));
                    data_horario.data[1] =  ((BUFFCOM[2]%10));
                    data_horario.data[3] =  ((BUFFCOM[3]/10));
                    data_horario.data[4] =  ((BUFFCOM[3]%10));
                    data_horario.ano = BUFFCOM[4] + 2000;
                    data_horario.dias = BUFFCOM[2];
                    data_horario.mes_atual = (BUFFCOM[3]-1); 
				break;
				
				case 2:
				 data_horario.horario[0] = ((BUFFCOM[2]/10));
				 data_horario.horario[1] = ((BUFFCOM[2]%10));  
				 data_horario.horario[3] = ((BUFFCOM[3]/10));
				 data_horario.horario[4] = ((BUFFCOM[3]%10));
                 
				 
				break;
				case 3:
                    horarioDespertador[0] =  ((BUFFCOM[2]/10));
                    horarioDespertador[1] =  ((BUFFCOM[2]%10));
                    horarioDespertador[3] =  ((BUFFCOM[3]/10));
                    horarioDespertador[4] =  ((BUFFCOM[3]%10));
                   
				break;
				default:
				break;
				
			}
				
					
		break;		
	}



	calculaCRCBUFFRESP();	
}

void verificaCRCBUFFCOM(){
	
	//ajudar no entendimento, deve ser retirado depois
	char CRC_OK =0;
	unsigned short crc, crc1, crc2;

	crc =  crc16(BUFFCOM,5);
	crc1 = crc >> 8;
	crc2 = crc & 0xFF;

	if(BUFFCOM[5] == crc1 && BUFFCOM[6] == crc2){
		CRC_OK = 1;
	}

	
	
	//indica que CRC esta correto
	if (CRC_OK == 1)
	{
		estado = ENVIAACK;	
	}
	else{
		
		//CRC nao esta correto
		estado = ENVIANACK;
			
	}
		
}

void maquinaEstado(){
	
	switch(estado){
		
		case VERIFICACRC:
			verificaCRCBUFFCOM();
			
		break;
		
		case COMRECEBIDO:
			
			trataComando();
			
		break;
		
		case ENVIARESPOSTA:
			tx_data();	
		break;	
		
		case ENVIAACK:
			enviaByte(ACK);
			
		break;
		
		case ENVIANACK:
			enviaByte(NACK);
			
		break;
		
		default:
			
		break;
		
		
	}
}


void main(void) {
      ADCON1 = 0x0F;
      TRISB = 0x00;
      TRISD = 0x00;
      //TRISC = 0x01;
      SPBRG=spbrg_value;                                // Fill the SPBRG register to set the Baud Rate
    //RCSTA.SPEN=1;                                     // To activate Serial port (TX and RX pins)
   // TXSTA.TXEN=1;                                     // To enable transmission
   // RCSTA.CREN=1;                                     // To enable continuous reception
    
    
    RCSTA = 0b10010000; // 0x90 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
    TXSTA = 0b00100000; // 0x20 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
    
     TRISCbits.RC0 = 1;
     TRISCbits.RC1 = 1;     
     TRISCbits.RC2 = 1;
             
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    
        //compare with the table above
    RCIF = 0; //reset RX pin flag
    RCIP = 0; //Not high priority
    RCIE = 1; //Enable RX interrupt
    PEIE = 1; //Enable pheripheral interrupt (serial port is a pheripheral)
      char botao;
      
    
      
      data_horario.horario[0] = 2;
      data_horario.horario[1] = 3;
      data_horario.horario[3] = 5;
      data_horario.horario[4] = 9;
      data_horario.horario[6] = 5;
      data_horario.horario[7] = 0;
      data_horario.horario[8] = '\0';
       
      data_horario.data[0] = 3;
      data_horario.data[1] = 1;
      data_horario.data[3] = 1;
      data_horario.data[4] = 2;
      data_horario.data[6] = '\0';
      data_horario.ano = 2018;
      data_horario.dias = 31;
      data_horario.mes_atual = 11;
      
      horarioDespertador[0] = 0;
      horarioDespertador[1] = 0;
      horarioDespertador[3] = 0;
      horarioDespertador[4] = 1;
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
       maquinaEstado();
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

