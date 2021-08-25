/*
 * File:   main.c
 * Author: Anderson e Guilherme
 *
 * Created on 24 de Maio de 2018, 14:08
 */




#include <xc.h>


#include <plib/usart.h>
#include <p18f4550.h>

#define _XTAL_FREQ 12000000 //The speed of your internal(or)external oscillator


void tx_data();
void enviaByte(char BYTE);

unsigned char rx_data(void);
unsigned char serial_data;
unsigned int i=0;


#define IDLE 0
#define VERIFICACRC 1
#define COMRECEBIDO 2
#define ENVIARESPOSTA 3
#define ENVIAACK 4
#define ENVIANACK 5	

 

#define ALTERACAO 2
#define LEITURA 1

#define ACK 	0x05
#define NACK 	0x15


#define FREQ 12000000    // Frequency = 12MHz
#define baud 9600
#define spbrg_value (((FREQ/64)/baud)-1)    // Refer to the formula for Baud rate calculation in Description tab


unsigned char BUFFCOM[7], BUFFRESP[7];

char estado = IDLE; // estado idle

char versao = 1;
char revisao = 2;



void enviaByteTeste(char BYTE)
{

    	while(!TXIF);                            // Wait until RCIF gets low
    	TXREG = BYTE;
}

void calculaCRCBUFFRESP(){
	
	//implementar o carcula do CRC da resposta aqui
	
		
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
				
					BUFFRESP[2] = versao;
					BUFFRESP[3] = revisao;
					
					//posicoes vazias
					BUFFRESP[4] = 0xFF;
				break;			
				
				case 1:
				//implementar
				break;
				
				case 2:
				//implementar
				break;
				
				default:
				break;
				
			}
				
		break;
		
		
		case ALTERACAO:
			
			//utilizar funcao memcpy, obrigatorio!
			//copia buffer de comando em buffer de resposta
			BUFFRESP[0] = BUFFCOM[0];
			BUFFRESP[1] = BUFFCOM[1];
			BUFFRESP[2] = BUFFCOM[2];
			BUFFRESP[3] = BUFFCOM[3];
			BUFFRESP[4] = BUFFCOM[4];
			BUFFRESP[5] = BUFFCOM[5];
			BUFFRESP[6] = BUFFCOM[6];

			
			// verifica o que deve ser lido
			switch(BUFFCOM[1])
			{
				//altera versao e revisao
				case 0:
				
					versao  = BUFFCOM[2];
					revisao = BUFFCOM[3];
				break;			
				
				case 1:
				//implementar
				break;
				
				case 2:
				//implementar
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
	char CRC_OK =1;
	
	//codigo para vefificar se CRC recebido esta correto
	//deve ser implementado ainda
	
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

void main()
{
    SPBRG=spbrg_value;                                // Fill the SPBRG register to set the Baud Rate
    //RCSTA.SPEN=1;                                     // To activate Serial port (TX and RX pins)
   // TXSTA.TXEN=1;                                     // To enable transmission
   // RCSTA.CREN=1;                                     // To enable continuous reception
    
    
    RCSTA = 0b10010000; // 0x90 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
    TXSTA = 0b00100000; // 0x20 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
    
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    
        //compare with the table above
    RCIF = 0; //reset RX pin flag
    RCIP = 0; //Not high priority
    RCIE = 1; //Enable RX interrupt
    PEIE = 1; //Enable pheripheral interrupt (serial port is a pheripheral)
    
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    
    while(1)
    {
       maquinaEstado();
    }


}


void enviaByte(char BYTE)
{

    	while(!TXIF);                            // Wait until RCIF gets low
    	TXREG = BYTE;

		if(BYTE==ACK)
			estado = COMRECEBIDO;
		else
			estado = IDLE;

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



void interrupt SerialRxPinInterrupt()
{
    //check if the interrupt is caused by RX pin
    if(PIR1bits.RCIF == 1)
    {
        if(i<7)
        {
        while(!RCIF);                            // Wait until RCIF gets low
            BUFFCOM[i]= RCREG;                                   // Retrieve data from reception register
            enviaByteTeste(BUFFCOM[i]);
            
          i++;  
        }else
        {
            PIR1bits.RCIF = 0; // clear rx flag
            i=0;
            estado = VERIFICACRC;
            
        }
        
        
        
    }

}
