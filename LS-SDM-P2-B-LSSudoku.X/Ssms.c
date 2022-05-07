#include <pic18f4321.h>
#include <xc.h>
#include "Ssms.h"
#include "TiTTimer.h"
#include "Menu.h"
#define neg -1

signed char lletraASCII = neg;
char lletraInici[8] = "ADGJMPTW";
char sumaPulsacions = 0;
signed char novaTecla = neg;
char timerSMS;
signed char ultimaTecla = neg;
__bit setSMSon = 0;
unsigned char zeroTecla;
char arrayZero[2] = {'0',32};

void Sinit(void){
    timerSMS = TiGetTimer();
}

void SMotor(void) {
	static char state = 0;

	switch(state) {
		case 0:
			if (novaTecla != neg && setSMSon == 1) {
				TiResetTics(timerSMS);
				state = 3;
			}
			else if (novaTecla == neg && TiGetTics(timerSMS)>= TSMS && lletraASCII != neg  && setSMSon == 1) {
				MNovaLletra(lletraASCII);
				lletraASCII = neg;
				ultimaTecla = neg;
				zeroTecla = 0;
			}
		break;
		case 1:
			if (novaTecla == ultimaTecla && TiGetTics(timerSMS) < TSMS) {
				novaTecla = neg;
				sumaPulsacions++;
				lletraASCII++;
				state++;
			}
			else if (novaTecla != neg && novaTecla != ultimaTecla) {
				MNovaLletra(lletraASCII);
				if(novaTecla != 0){
				  lletraASCII = lletraInici[novaTecla-2];
				}else{
				  lletraASCII = '0';
				}
				zeroTecla=0;
				ultimaTecla = novaTecla;
				sumaPulsacions = 0;
				novaTecla = neg;
				state--;
			}
		break;
		case 2:
			if (ultimaTecla != 7 && ultimaTecla != 9 && ultimaTecla != 0) {
				state = 5;
			}
			else if (ultimaTecla == 7 || ultimaTecla == 9) {
				state = 6;
			}
			else if (ultimaTecla == 0) {
				zeroTecla++;
				if(zeroTecla == 2) zeroTecla = 0;
				lletraASCII = arrayZero[zeroTecla];
				state = 0;
			}
		break;
		case 3:
			if ((novaTecla > 1 || novaTecla == 0) && novaTecla < 10) {
				state = 1;
			}
			else if (novaTecla != 0 && novaTecla <= 1 && lletraASCII == neg) {
				MNovaLletra(novaTecla+48);
				novaTecla = neg;
				state = 0;
			}
			else if (novaTecla != 0 && novaTecla <= 1 && lletraASCII != neg) {
				MNovaLletra(lletraASCII);
				lletraASCII = neg;
				state++;
			}
		break;
		case 4:
			MNovaLletra(novaTecla+48);
			novaTecla = neg;
			state = 0;
		break;
		case 5:
			if (sumaPulsacions == 4) {
				lletraASCII = lletraInici[ultimaTecla-2];
				sumaPulsacions = 0;
				state = 0;
			}
			else if (sumaPulsacions == 3) {
				lletraASCII = ultimaTecla+48;
				state = 0;
			}
			else if (sumaPulsacions < 3) {
				state = 0;
			}
		break;
		case 6:
			if (sumaPulsacions == 4) {
				lletraASCII = ultimaTecla+48;
				state = 0;
			}
			else if (sumaPulsacions == 5) {
				lletraASCII = lletraInici[ultimaTecla-2];
				sumaPulsacions = 0;
				state = 0;
			}
			else if (sumaPulsacions < 4) {
				state = 0;
			}
		break;
	}
}

void SsetNovaTecla(char tecla){
    novaTecla = tecla;
}

void SMSon(void){
    setSMSon = 1;
    novaTecla = neg;
}

void SMSoff (void){
    setSMSon = 0;
    novaTecla = neg;
}