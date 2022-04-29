#include <xc.h>
#include "Menu.h"
#include "LcTLCD.h"
#include "TiTTimer.h"
#include "Usuaris.h"
#include "Ssms.h"
#include "GestioLCD.h"

unsigned char tmp = 0;
signed char NovaTecla = -1;
signed char novaLletra = -1;
char timerMenu;
__bit loginNOTRegister;
unsigned char menuDalt = 0;

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
				tmp =0;
				state = 6;
			}
		break;
		case 6:
			if (novaLletra > 47 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaChar(novaLletra,tmp);
				state = 7;
			}
			else if (NovaTecla == 11) {
				state = 7;
			}
		break;
		case 7:
			if (tmp != 7 && NovaTecla != 11) {
				tmp++;
				NovaTecla = -1;
				novaLletra = -1;
				state = 6;
			}
			else if (tmp == 7 || NovaTecla == 11) {
				LcGotoXY(6,1);
				UenviaChar('\0',tmp);
				NovaTecla = -1;
				novaLletra = -1;
				tmp = 0;
				state = 8;
			}
		break;
		case 8:
			if (novaLletra > 47 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaPas(novaLletra,tmp);
				state = 9;
			}
			else if (NovaTecla == 11) {
				state = 9;
			}
		break;
		case 9:
			if (tmp != 7 && NovaTecla != 11) {
				tmp++;
				NovaTecla = -1;
				novaLletra = -1;
				state = 8;
			}
			else if (tmp == 7 || NovaTecla == 11) {
				UenviaPas('\0',tmp);
				NovaTecla = -1;
				novaLletra = -1;
				tmp = 0;
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
            if(1==1){
                state = 12;
            }
            else if (!loginNOTRegister && UcheckExistsGetError() == -1) {
				URegister();
				loginNOTRegister = 1;
				state = 3;
			}
			else if (loginNOTRegister == 1 && UcheckExistsGetError()) {
				state = 12;
			}
			else if (loginNOTRegister && UcheckExistsGetError() == -1 || !loginNOTRegister && UcheckExistsGetError()) {
				state = 0;
			}
		break;
		case 12:
			LcClear();
			SiFerMenu();
			GLCDMostraMenu(menuDalt);
			NovaTecla = -1;
			state = 13;
		break;
		case 13:
			if (NovaTecla == 8) {
				if(menuDalt < 4) menuDalt++;
				state = 12;
			}
			else if (NovaTecla == 2) {
				if(menuDalt > 0) menuDalt--;
				state = 12;
			}
		break;
	}
}