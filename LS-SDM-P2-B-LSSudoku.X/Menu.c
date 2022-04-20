#include <xc.h>
#include "Menu.h"
#include "LcTLCD.h"
#include "TiTTimer.h"
#include "Usuaris.h"

unsigned char menuDalt = 0;
signed char NovaTecla = -1;
signed char novaLletra = -1;
char timerMenu;

void Minit(void){
    timerMenu = TiGetTimer();
}
void MsetNovaTecla(char tecla){
    NovaTecla = tecla;
}

void MNovaLletra(char lletra){
    novaLletra = lletra;
}

void menu(void) {
	static char state = 0;

	switch(state) {
		case 0:
			LcClear();
			LcNewString("1.LOGIN");
			state = 1;
		break;
		case 1:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString("2.REGISTER");
				NovaTecla = -1;
				state = 2;
			}
		break;
		case 2:
			if (NovaTecla == 1 && UgetNumUsuaris() > 0) {
				NovaTecla = -1;
				LcClear();
				LcNewString("L-USER: ");
				state = 3;
			}
			else if (NovaTecla == 2 && UgetNumUsuaris() < 8) {
				NovaTecla = -1;
				LcClear();
				LcNewString("R-USER: ");
				state = 4;
			}
		break;
		case 3:
			if (LcLliure()) {
				state = 5;
			}
		break;
		case 4:
			if (LcLliure()) {
				state = 6;
			}
		break;
		case 5:

		break;
		case 6:

		break;
	}
}