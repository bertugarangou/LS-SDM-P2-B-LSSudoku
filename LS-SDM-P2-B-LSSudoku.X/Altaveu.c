#include <xc.h>
#include "Altaveu.h"
#include "TiTTimer.h"
char duradaNotes = 168; //128+32+8
__bit reproduir = 0;
unsigned char nota;
char timerAltaveu;
unsigned char copsNota;

void initAltaveu(void){
    timerAltaveu = TiGetTimer(); 
}
void playAltaveu(void){
    reproduir = 1;
}
void motorAltaveu(void){
static char state = 0;

	switch(state) {
		case 0:
			if (reproduir) {
				nota = 1;
				copsNota = 0;
				TiResetTics(timerAltaveu);
				state = 1;
			}
		break;
		case 1:
			if (nota > 5) {
				reproduir = 0;
				state--;
			}
			else if (copsNota == duradaNotes) {
				nota++;
				copsNota = 0;
			}
			else if (TiGetTics(timerAltaveu) > nota) {
				LATBbits.LB0 = 1;
				TiResetTics(timerAltaveu);
				state++;
			}
		break;
		case 2:
			if (TiGetTics(timerAltaveu) > nota) {
				LATBbits.LB0 = 0;
				TiResetTics(timerAltaveu);
				copsNota++;
				state--;
			}
		break;
	}
}