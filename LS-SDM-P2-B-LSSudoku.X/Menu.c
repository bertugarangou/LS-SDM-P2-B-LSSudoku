#include <xc.h>
#include "Menu.h"
#include "LcTLCD.h"
#include "TiTTimer.h"
#include "Usuaris.h"
#include "Ssms.h"
#include "GestioLCD.h"
#include "Hora.h"
#include "Joc.h"
#include "SIO.h"
#include "CtoA.h"

#define neg -1
char loginText[8] = "1.LOGIN";
char registerText[11] = "2.REGISTER";
unsigned char tmp = 0;
signed char NovaTecla = neg;
signed char novaLletra = neg;
signed char novaDireccio = neg;
char horaTmp[4];

char timerMenu;
__bit loginNOTRegister;
unsigned char menuDalt = 0;
signed char indexUsuari;
unsigned char pos;

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
				NovaTecla = neg;
				state++;
			}
		break;
		case 2:
			if (NovaTecla == 1) {
				loginNOTRegister = 1;
				state++;
			}
			else if (NovaTecla == 2) {
				loginNOTRegister = 0;
				state++;
			}
		break;
		case 3:
			NovaTecla = neg;
			novaLletra = neg;
			LcClear();
			LcNewString("USER: ");
			state++;
		break;
		case 4:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString("PSWD: ");
				state++;
			}
		break;
		case 5:
			if (LcLliure()) {
				LcGotoXY(6,0);
				SMSon();
				tmp =0;
				state++;
			}
		break;
		case 6:
			if (novaLletra > 31 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaChar(novaLletra,tmp);
				state++;
			}
			else if (NovaTecla == 11) {
				state++;
			}
		break;
		case 7:
			if (tmp != 7 && NovaTecla != 11) {
				tmp++;
				NovaTecla = neg;
				novaLletra = neg;
				state--;
			}
			else if (tmp == 7 || NovaTecla == 11) {
				LcGotoXY(6,1);
				UenviaChar('\0',tmp);
				NovaTecla = neg;
				novaLletra = neg;
				tmp = 0;
				state++;
			}
		break;
		case 8:
			if (novaLletra > 31 && NovaTecla != 10) {
				LcPutChar(novaLletra);
				UenviaPas(novaLletra,tmp);
				state++;
			}
			else if (NovaTecla == 11) {
				state++;
			}
		break;
		case 9:
			if (tmp != 7 && NovaTecla != 11) {
				tmp++;
				NovaTecla = neg;
				novaLletra = neg;
				state--;
			}
			else if (tmp == 7 || NovaTecla == 11) {
				UenviaPas('\0',tmp);
				NovaTecla = neg;
				novaLletra = neg;
				tmp = 0;
				UcheckExists();
				SMSoff();
				state++;
			}
		break;
		case 10:
			if (!UcheckExistsNotFinished()) {
				indexUsuari = UcheckExistsGetError();
				state++;
			}
		break;
		case 11:
			if (!loginNOTRegister && indexUsuari == neg) {
				URegister();
				state = 0;
			}
			else if (loginNOTRegister == 1 && indexUsuari > neg && miraPassword()) {
				state++;
			}
			else if ((loginNOTRegister && indexUsuari == neg) || (!loginNOTRegister && indexUsuari > neg) || !miraPassword()) {
				state = 0;
			}
		break;
		case 12:
			LcClear();
			SiFerMenu();
			GLCDMostraMenu(menuDalt);
			novaDireccio = neg;
			NovaTecla = neg;
			state++;
		break;
		case 13:
			if (novaDireccio == 8) {
				if(menuDalt < 4) menuDalt++;
				state--;
			}
			else if (novaDireccio == 2) {
				if(menuDalt > 0) menuDalt--;
				state--;
			}
			else if (NovaTecla == 11 && menuDalt == 3) {
				NoFerMenu();
				LcClear();
				LcNewString("BYE BYE");
				state++;
			}
			else if (NovaTecla == 11 && menuDalt == 1) {
				LcClear();
				NoFerMenu();
				LcNewString("Modify Time:");
				NovaTecla = neg;
				state = 16;
			}
			else if (menuDalt == 0 && NovaTecla == 11) {
				NoFerMenu();
				NovaTecla = neg;
				LcClear();
				LcNewString("TIME REMAINING:");
				JJuguem(indexUsuari);
				state = 21;
			}
			else if (NovaTecla == 11 && menuDalt == 4) {
				NoFerMenu();
				LcClear();
				menuDalt = 0;
				NovaTecla = neg;
				calculateShowUsers();
				state = 30;
			}
			else if (NovaTecla == 11 && menuDalt == 2 && UgetTop5(0) != neg) {
				NoFerMenu();
				LcClear();
				menuDalt = 0;
				NovaTecla = neg;
				state = 31;
			}
		break;
		case 14:
			if (LcLliure()) {
				LcNewString(UgetUserName(indexUsuari));
				TiResetTics(timerMenu);
				state++;
			}
		break;
		case 15:
			if (TiGetTics(timerMenu) > 2410) {
				state = 0;
			}
		break;
		case 16:
			if (LcLliure() && NovaTecla > neg && NovaTecla < 10) {
				LcGotoXY(0,1);
				horaTmp[0] = NovaTecla+48;
				LcPutChar(horaTmp[0]);
				NovaTecla = neg;
				state++;
			}
			//else if (NovaTecla == 10) {
			//	state = 12;
			//}
		break;
		case 17:
			if (NovaTecla > neg && NovaTecla < 10) {
				horaTmp[1] = NovaTecla+48;
				LcPutChar(horaTmp[1]);
				NovaTecla = neg;
				LcPutChar(':');
				state++;
			}
			//else if (NovaTecla ==10) {
			//	state = 12;
			//}
		break;
		case 18:
			if (NovaTecla > neg && NovaTecla < 10) {
				horaTmp[2] = NovaTecla+48;
				LcPutChar(horaTmp[2]);
				NovaTecla = neg;
				state++;
			}
			//else if (NovaTecla == 10) {
			//	state = 12;
			//}
		break;
		case 19:
			if (NovaTecla > neg && NovaTecla < 10) {
				horaTmp[3] = NovaTecla+48;
				LcPutChar(horaTmp[3]);
				NovaTecla = neg;
				state++;
			}
			//else if (NovaTecla ==10) {
			//	state = 12;
			//}
		break;
		case 20:
			if (NovaTecla == 11) {
				HActualitzaHora(horaTmp);
				NovaTecla = neg;
				state = 12;
			}
			else if (NovaTecla == 10) {
				state = 12;
			}
		break;
		case 21:
			if ((NovaTecla == 10 && LcLliure()) || checkHoraAcabat()) {
				LcClear();
				JendGame();
				state++;
			}
			else if (NovaTecla > 0 && NovaTecla < 10) {
				JnovaTecla(NovaTecla);
				NovaTecla = neg;
				
			}
			else if (novaDireccio > neg) {
				JnovaDireccio(novaDireccio);
				novaDireccio = neg;
				
			}
		break;
		case 22:
			if (SIOHaAcabatPuntuacions() && NovaTecla == 11) {
				state = 12;
			}
		break;
		case 23:
			if (LcLliure()) {
				LcGotoXY(0,0);
				LcNewString(getArrayShowUsers(menuDalt));
				menuDalt++;
				state++;
			}
		break;
		case 24:
			if (LcLliure()) {
				if (menuDalt < UgetNumUsuaris()){
				  LcGotoXY(0,1);
				  LcNewString(getArrayShowUsers(menuDalt));
				  menuDalt++;
				} else {
				  menuDalt = 0;
				}
				state++;
			}
		break;
		case 25:
			if (LcLliure()) {
				if (menuDalt == UgetNumUsuaris()){
				  menuDalt = 0;
				}
				LcGotoXY(16,0);
				LcNewString(getArrayShowUsers(menuDalt));
				state++;
			}
		break;
		case 26:
			if (LcLliure()) {
				if (menuDalt+1 < UgetNumUsuaris()){
				  LcGotoXY(16,1);
				  LcNewString(getArrayShowUsers(menuDalt+1));
				}
				state++;
			}
		break;
		case 28:
			if (TiGetTics(timerMenu) > 1205) {
				state++;
			}
			else if (NovaTecla == 11) {
				menuDalt = 0;
				state = 12;
			}
		break;
		case 29:
			if (pos > 15) {
				LcClear();
				state = 23;
			}
			else if (pos < 16) {
				TiResetTics(timerMenu);
				LcScroll();
				pos++;
				state--;
			}
		break;
		case 27:
			if (LcLliure()) {
				TiResetTics(timerMenu);
				pos = 0;
				state++;
			}
		break;
		case 30:
			if (UshowUsersCalculat()) {
				state = 23;
			}
		break;
		case 31:
			if (LcLliure()) {
				LcNewString(UgetUserName(UgetTop5(menuDalt)));
				CToAConverteix(UgetScore(UgetTop5(menuDalt)));
				state++;
			}
		break;
		case 32:
			if (CToAHaAcabat() == 250 && LcLliure()) {
				LcPutChar(':');
				LcGotoXY(0,1);
				LcNewString(CToAobtenir());
				menuDalt++;
				if(menuDalt == 5 || UgetTop5(menuDalt) == neg) menuDalt = 0;
				state = 34;
			}
		break;
		case 34:
			if ((UgetTop5(menuDalt)  != neg || menuDalt != 5) && LcLliure()) {
				LcGotoXY(16,0);
				LcNewString(UgetUserName(UgetTop5(menuDalt)));
				CToAConverteix(UgetScore(UgetTop5(menuDalt)));
				state++;
			}
		break;
		case 35:
			if (LcLliure() && CToAHaAcabat() == 250) {
				LcPutChar(':');
				LcGotoXY(16,1);
				LcNewString(CToAobtenir());
				pos = 0;
				TiResetTics(timerMenu);
				state = 37;
			}
		break;
		case 37:
			if (NovaTecla == 11 && LcLliure()) {
				menuDalt = 0;
				state = 12;
			}
			else if (TiGetTics(timerMenu) > 400 && LcLliure()) {
				state++;
			}
		break;
		case 38:
			if (pos < 16) {
				TiResetTics(timerMenu);
				LcScroll();
				pos++;
				state--;
			}
			else if (pos > 15) {
				LcClear();
				state = 31;
			}
		break;
	}
}