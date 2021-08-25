
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define IDLE 0

char estado = IDLE; // estado idle
char pegaHoraData[3];
unsigned char BUFFCOM[7], BUFFRESP[7];
char flagSegundo = 0;
char cont=0;
char flagDespertadorLigado = 0;
char virouDia = 1;
char botaoHora;
char botaoMin;
char tempoDespertador = 0;
char horarioDespertador[6];
char tela_atual = 0;
char versao[6] = {'0','1','.','0','0'};



#endif	

