/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */


#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include <plib/usart.h>
#include <p18f4550.h>
#include <xlcd.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POLY 0x8408
#define _XTAL_FREQ 20000000
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
#define FREQ 20000000 
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

horas_datas data_horario;
char pegaHoraData[3];
unsigned char BUFFCOM[7], BUFFRESP[7];
char estado = IDLE; // estado idle
char versao[6] = {'0','1','.','0','0'};
char horarioDespertador[6];


unsigned short crc16(char *data_p, unsigned short length);
void calculaCRCBUFFRESP();
void trataComando();
void verificaCRCBUFFCOM();
void maquinaEstado();
void enviaByte(char BYTE);
void tx_data();




#endif	/* XC_HEADER_TEMPLATE_H */

