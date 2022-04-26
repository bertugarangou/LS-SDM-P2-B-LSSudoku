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
			else if (NovaTecla == 2) {
				loginNOTRegister = 0;
				state = 3;
			}
		break;
		case 3:
			NovaTecla = -1;
			novaLletra = -1;
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
			if (novaLletra > 47 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaChar(novaLletra,j);
				state = 7;
			}
			else if (NovaTecla == 11) {
				state = 7;
			}
		break;
		case 7:
			if (j != 7 && NovaTecla != 11) {
				j++;
				NovaTecla = -1;
				novaLletra = -1;
				state = 6;
			}
			else if (j == 7 || NovaTecla == 11) {
				LcGotoXY(6,1);
				UenviaChar('\0',j);
				NovaTecla = -1;
				novaLletra = -1;
				j = 0;
				state = 8;
			}
		break;
		case 8:
			if (novaLletra > 47 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaPas(novaLletra,j);
				state = 9;
			}
			else if (NovaTecla == 11) {
				state = 9;
			}
		break;
		case 9:
			if (j != 7 && NovaTecla != 11) {
				j++;
				NovaTecla = -1;
				novaLletra = -1;
				state = 8;
			}
			else if (j == 7 || NovaTecla == 11) {
				UenviaPas('\0',j);
				NovaTecla = -1;
				novaLletra = -1;
				j = 0;
				LcClear();
				UcheckExists();
				SMSoff();
				state = 10;
			}
		break;
		case 10:
			if (!UcheckExistsNotFinished()) {
				state = 11;
			}
		break;
		case 11:
			if (loginNOTRegister == 0 && UcheckExistsGetError() == 0) {
				state = 12;
			}
			else if (loginNOTRegister == 1 && UcheckExistsGetError() == 1) {
				state = 13;
			}
			else if (loginNOTRegister ^ UcheckExistsGetError()) {
				state = 0;
			}
		break;
		case 12:

		break;
		case 13:

		break;
	}
}