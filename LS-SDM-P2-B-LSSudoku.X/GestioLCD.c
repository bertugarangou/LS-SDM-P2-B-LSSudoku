#include <xc.h>
#include "TiTTimer.h"
#include "LcTLCD.h"
#include "GestioLCD.h"
#define one 1
const char menuList[][29] = {
    "1.Play a Game",
    "2.Modify Time",
    "3.Show General Top 5 Scores ",
    "4.Logout",
    "5.Show All Users"
};

char timerGLCD=0;
char menuDaltGLCD=0;
__bit ferMenu = 0;
__bit newString = 0;

void SiFerMenu(void){
    ferMenu = one;
}
void NoFerMenu(void){
    ferMenu = 0;
}

void GLCDInit(void){
    timerGLCD=TiGetTimer();
}

void GLCDMostraMenu(unsigned char num){
    newString = one;
    menuDaltGLCD = num;
    ferMenu = one;
}

void GLCDMotor(void) {
	static char state = 0;
    static char j=0;
    static char max1 = 15;
    static char min1 = 0;
    static char min2 = 0;
    static char max2 = 15;
    static char k1 = 0;
    static char k2 = 0;
    static char length1=0;
    static char length2=0;
    
    switch(state) {
		case 2:
			if (length1 < 17 && ferMenu == one) {
				LcGotoXY(0,0);
				LcNewString(menuList[menuDaltGLCD]);
				state = 5;
			}
			else if (length1 >= 17 && ferMenu == one) {
				state++;
			}
		break;
		case 3:
			if (j>=16) {
				min1++;
				max1++;
				if (menuList[menuDaltGLCD][max1] == '\0') max1=0;
				if (menuList[menuDaltGLCD][min1] == '\0') min1=0;
				j=0;
				k1=min1;
				state = 6;
			}
			else if (j<16) {
				LcGotoXY(j,0);
				LcPutChar(menuList[menuDaltGLCD][k1]);
				j++;
				k1++;
				if(menuList[menuDaltGLCD][k1] == '\0') k1 = 0;
			}
		break;
		case 6:
			if (menuDaltGLCD<4) {
				state++;
			}
			else if (menuDaltGLCD >= 4) {
				state = 0;
			}
		break;
		case 5:
			if (LcLliure() && menuDaltGLCD<4) {
				state = 7;
			}
			else if (LcLliure() && menuDaltGLCD >= 4) {
				state = 0;
			}
		break;
		case 0:
			if (newString != 0 && ferMenu == one) {
				length1 = 0;
				length2 = 0;
				LcGotoXY(0,0);
				min1 = 0;
				max1 = 15;
				min2 = 0;
				max2 = 15;
				j = 0;
				k1 = 0;
				k2 = 0;
				newString = 0;
				state++;
			}
		break;
		case one:
			if (menuList[menuDaltGLCD][length1] == '\0' && menuDaltGLCD>=4) {
				state++;
			}
			else if (menuList[menuDaltGLCD][length1] != '\0') {
				length1++;
			}
			else if (menuDaltGLCD<4 && menuList[menuDaltGLCD][length1] == '\0') {
				state = 13;
			}
		break;
		case 7:
			if (length2 < 17 && ferMenu == one) {
				LcGotoXY(0,one);
				LcNewString(menuList[menuDaltGLCD+one]);
				state++;
			}
			else if (length2 >= 17 && ferMenu == one) {
				state = 9;
			}
		break;
		case 9:
			if (j<16) {
				LcGotoXY(j,one);
				LcPutChar(menuList[menuDaltGLCD+one][k2]);
				j++;
				k2++;
				if(menuList[menuDaltGLCD+one][k2] == '\0') k2 = 0;
			}
			else if (j>=16) {
				min2++;
				max2++;
				if (menuList[menuDaltGLCD+one][max2] == '\0') max2=0;
				if (menuList[menuDaltGLCD+one][min2] == '\0') min2=0;
				j=0;
				k2=min2;
				state++;
			}
		break;
		case 11:
			if (TiGetTics(timerGLCD)>= 1205 && newString == 0 && ferMenu == one) {
				LcClear();
				state = 2;
			}
			else if (newString != 0 && ferMenu == one) {
				LcClear();
				state = 0;
			}
		break;
		case 10:
			TiResetTics(timerGLCD);
			state++;
		break;
		case 8:
			if (LcLliure()) {
				TiResetTics(timerGLCD);
				state = 11;
			}
		break;
		case 13:
			if (menuList[menuDaltGLCD+one][length2] != '\0') {
				length2++;
			}
			else if (menuList[menuDaltGLCD+one][length2] == '\0' ) {
				state = 2;
			}
		break;
	}
}