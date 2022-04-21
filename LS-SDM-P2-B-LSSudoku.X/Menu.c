#include <xc.h>
#include "Menu.h"
#include "LcTLCD.h"
#include "TiTTimer.h"
#include "Usuaris.h"
#include "Ssms.h"

unsigned char j;
signed char NovaTecla = -1;
signed char novaLletra = -1;
char timerMenu;
__bit loginNOTRegister;
char username[9];
char password[9];

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
				loginNOTRegister = 1;
				state = 3;
			}
			else if (NovaTecla == 2 && UgetNumUsuaris() < 8) {
				loginNOTRegister = 0;
				state = 3;
			}
		break;
		case 3:
			NovaTecla = -1;
			LcClear();
			LcNewString("USER: ");
			state = 4;
		break;
		case 4:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString("PSWD: ");
				state = 5;
			}
		break;
		case 5:
			if (LcLliure()) {
				LcGotoXY(6,0);
				SMSon();
				j =0;
				state = 6;
			}
		break;
		case 6:
			if (novaLletra > 47 && NovaTecla < 11) {
				LcPutChar(novaLletra);
				username[j] = novaLletra;
				NovaTecla = -1;
				novaLletra = -1;
				j++;
				state = 7;
			}
			else if (NovaTecla == 11) {
				j = 8;
				state = 7;
			}
		break;
		case 7:
			if (j != 8) {
				state = 6;
			}
			else if (j == 8) {
				LcGotoXY(6,1);
				username[j] = novaLletra;
				NovaTecla = -1;
				novaLletra = -1;
				j = 0;
				state = 8;
			}
		break;
		case 8:
			if (novaLletra > 47 && NovaTecla < 11) {
				LcPutChar(novaLletra);
				password[j] = novaLletra;
				NovaTecla = -1;
				novaLletra = -1;
				j++;
				state = 9;
			}
			else if (NovaTecla == 11) {
				j = 8;
				state = 9;
			}
		break;
		case 9:
			if (j != 8) {
				state = 8;
			}
			else if (j == 8) {
				password[j] = novaLletra;
				NovaTecla = -1;
				novaLletra = -1;
				j = 0;
				LcClear();
				state = 10;
			}
		break;
		case 10:

		break;
	}
}