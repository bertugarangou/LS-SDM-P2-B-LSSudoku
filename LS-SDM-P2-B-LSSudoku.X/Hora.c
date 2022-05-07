#include <xc.h>
#include "Altaveu.h"
#include "TiTTimer.h"
char timerHora;
unsigned char segons = 0;
char hora[6] = "30:00";
__bit jugant = 0;
__bit nouSegon = 0;
__bit tempsAcabat;

void initHora(void){
    timerHora = TiGetTimer();
}

void HActualitzaHora(char nova[]){
    hora[0] = nova[0];
    hora[1] = nova[1];
    hora[3] = nova[2];
    hora[4] = nova[3];
}

__bit HNouSegon(void){
    return nouSegon;
}

void HClearNouSegon(void){
    nouSegon = 0;
}

char* HGetTime(void){    
    return hora;
}

void HnoJugant(void){
    jugant = 0;
}
void HJugant(void){
    jugant = 1;
    tempsAcabat = 0;
}
__bit checkHoraAcabat(void){
    return tempsAcabat;
}

void motorHora(void) {
	static char state = 0;

	switch(state) {
		case 0:
			if (TiGetTics(timerHora) >= 1205) {
				TiResetTics(timerHora);
				if(jugant){
						nouSegon = 1;
					//segons;
					hora[4]--;
					if(hora[4] == ('0'-1)){
						hora[4] = '9';
						hora[3]--;
					}
					//minsBaix;
					if(hora[3] == ('0'-1)){
						hora[3] = '5';
						hora[1]--;
					}
					//minsAlt;
					if(hora[1] == ('0'-1)){
						hora[1] = '9';
						hora[0]--;
					}
					//final;
					if(hora[0] == '0' && hora[1] == '0' && hora[3] == '0' && hora[4] == ('0')){
						tempsAcabat = 1;
					}
					//reproducció;
					segons++;
					if(segons > 59){
						//playAltaveu();
						segons = 0;
					}
				}
				state = 0;
			}
		break;
	}
}