#include <xc.h>
#include "Menu.h"
#include "LcTLCD.h"
#include "TiTTimer.h"
#include "Usuaris.h"
#include "Ssms.h"
#include "GestioLCD.h"
#include "Hora.h"

unsigned char tmp = 0;
signed char NovaTecla = -1;
signed char novaLletra = -1;
signed char novaDireccio = -1;
char horaTmp[4];

unsigned char loginText[8] = "1.LOGIN";
unsigned char registerText[11] = "2.REGISTER";

char timerMenu;
__bit loginNOTRegister;
unsigned char menuDalt = 0;
signed char indexUsuari;

void Minit(void){
    timerMenu = TiGetTimer();
}
void MsetNovaTecla(char tecla){
    NovaTecla = tecla;
}

void MNovaLletra(char lletra){
    novaLletra = lletra;
}
void MNouJoystick(signed char posicio){
    novaDireccio = posicio;
}

void menu(void) {
	static char state = 0;

	switch(state) {
		case 0:
			LcClear();
			LcNewString(loginText);
			state = 1;
		break;
		case 1:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString(registerText);
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
				indexUsuari = UcheckExistsGetError();
				state = 11;
			}
		break;
		case 11:
            if (1 == 1) {
				state = 12;
			}
			else if (!loginNOTRegister && indexUsuari == -1) {
				URegister();
				loginNOTRegister = 1;
				state = 3;
			}
			else if (loginNOTRegister == 1 && indexUsuari) {
				state = 12;
			}
			else if ((loginNOTRegister && indexUsuari == -1) || (!loginNOTRegister && indexUsuari)) {
				state = 0;
			}
		
		break;
		case 12:
			LcClear();
			SiFerMenu();
			GLCDMostraMenu(menuDalt);
			novaDireccio = -1;
			NovaTecla = -1;
			state = 13;
		break;
		case 13:
			if (novaDireccio == 8) {
				if(menuDalt < 4) menuDalt++;
				state = 12;
			}
			else if (novaDireccio == 2) {
				if(menuDalt > 0) menuDalt--;
				state = 12;
			}
			else if (NovaTecla == 11 && menuDalt == 3) {
				NoFerMenu();
				LcClear();
				LcNewString("BYE BYE ");
				state = 14;
			}
			else if (NovaTecla == 11 && menuDalt == 1) {
				LcClear();
				NoFerMenu();
				LcNewString("Modify Time:");
				NovaTecla = -1;
				state = 16;
			}
		break;
		case 14:
			if (LcLliure()) {
				LcNewString(UgetUserName(indexUsuari));
				TiResetTics(timerMenu);
				state = 15;
			}
		break;
		case 15:
			if (TiGetTics(timerMenu) > 2410) {
				state = 0;
			}
		break;
		case 16:
			if (LcLliure() && NovaTecla > -1 && NovaTecla < 10) {
				LcGotoXY(0,1);
				horaTmp[0] = NovaTecla+48;
				LcPutChar(horaTmp[0]);
				NovaTecla = -1;
				state = 17;
			}
			else if (NovaTecla == 10) {
				state = 12;
			}
		break;
		case 17:
			if (NovaTecla > -1 && NovaTecla < 10) {
				horaTmp[1] = NovaTecla+48;
				LcPutChar(horaTmp[1]);
				NovaTecla = -1;
				LcPutChar(':');
				state = 18;
			}
			else if (NovaTecla ==10) {
				state = 12;
			}
		break;
		case 18:
			if (NovaTecla > -1 && NovaTecla < 10) {
				horaTmp[2] = NovaTecla+48;
				LcPutChar(horaTmp[2]);
				NovaTecla = -1;
				state = 19;
			}
			else if (NovaTecla == 10) {
				state = 12;
			}
		break;
		case 19:
			if (NovaTecla > -1 && NovaTecla < 10) {
				horaTmp[3] = NovaTecla+48;
				LcPutChar(horaTmp[3]);
				NovaTecla = -1;
				state = 20;
			}
			else if (NovaTecla ==10) {
				state = 12;
			}
		break;
		case 20:
			if (NovaTecla == 11) {
				HActualitzaHora(horaTmp);
				NovaTecla = -1;
				state = 12;
			}
			else if (NovaTecla == 10) {
				state = 12;
			}
		break;
	}
}